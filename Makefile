# Executable name:
JUCE_TARGET_APP = KeyChord
# Version number:
APP_VERSION = 0.0.1
# Version hex.
APP_VERSION_HEX = 0x001

# Temporary hack to fix issue https://github.com/WeAreROLI/JUCE/issues/524
# Remove this once the JUCE library fixes the issue!
JUCE_COMPILER_HEADER=deps/JUCE/modules/juce_core/system/juce_CompilerSupport.h
JUCEFIX=$(shell grep 201402 $(JUCE_COMPILER_HEADER) && \
            sed -i 's/201402/201300/g' $(JUCE_COMPILER_HEADER))

define HELPTEXT
## Help Text Template: ##
# Usage: make [target] [options]
# Typical installation process:
    make
    make install

Update this text with instructions for building your project.

## Main build targets:
   build:             Compile all project code.
   install:           Install compiled binaries and program asset files.
   debug:             Compile debug build, install, and open in gdb.
   release:           Compile and install release build.
   check-pkg-config:  Verify all pkg-config libraries.
   clean:             Remove all compiled binaries.
   strip:             Remove symbols from compiled binaries.
   uninstall:         Uninstall the application.
   help:              Print this help information.

# Main build Options:
# Format:
# OPTION_NAME=(all, permitted, values)
#   Description of what the option controls.

CONFIG=(Debug, Release)
  Selects between debug and release builds. By default, gdb flags, console
  logging, assertions, and tests are enabled only in debug builds, while
  optimization is enabled only in release builds.

V=(1)
  Enable verbose build output.

CHECK_DEPS=(1)
  Find and remove build files that have missing or renamed dependencies before
  compiling.

OPTIMIZE=(0, 1)
  Disable or enable optimization when compiling and linking.

GDB_SUPPORT=(0, 1)
  Disable or enable compiling with gdb debugging flags.

BUILD_TESTS=(0, 1)
  Disable or enable compilation of test classes.
endef
export HELPTEXT

######### Initialize build variables: #########

# Default Options:
# Build type: either Debug or Release
CONFIG ?= Release
# Command used to strip unneeded symbols from object files:
STRIP ?= strip
# Use the build system's architecture by default.
TARGET_ARCH ?= -march=native
# Enable or disable verbose output
V ?= 0
# Skip extra dependency checks by default.
CHECK_DEPS ?= 0

# Build directories:
JUCE_BINDIR := build
JUCE_LIBDIR := build
# Directory where .o files will be created:
JUCE_OBJDIR := build/intermediate
# Directory where executable files will be created:
JUCE_OUTDIR := build
# Data installation directory
DATA_PATH := /usr/share/$(JUCE_TARGET_APP)

# Pkg-config libraries:
PKG_CONFIG_LIBS = freetype2 gio-2.0 x11 xext xinerama xpm
# NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama xpm

# Additional library flags:
LDFLAGS := -ldl -lpthread $(LDFLAGS)
# -lcrypto -ldl -lpthread -lrt $(LDFLAGS)

# Preprocessor values used by the JUCE library:
JUCE_DEFS := -DDONT_SET_USING_JUCE_NAMESPACE=1 \
             -DJUCER_LINUX_MAKE_6D53C8B4=1 \
             -DJUCE_APP_VERSION=$(APP_VERSION) \
             -DJUCE_APP_VERSION_HEX=$(APP_VERSION_HEX) \
             -DJUCE_TARGET_APP=\"$(JUCE_TARGET_APP)\"

JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 \
                     -DJucePlugin_Build_VST3=0 \
                     -DJucePlugin_Build_AU=0 \
                     -DJucePlugin_Build_AUv3=0 \
                     -DJucePlugin_Build_RTAS=0 \
                     -DJucePlugin_Build_AAX=0 \
                     -DJucePlugin_Build_Standalone=0

# Extra compilation flags:
CPPFLAGS := -pthread $(CPPFLAGS)

# Extra compilation flags (C++ only):
CXXFLAGS := -std=gnu++14 $(CXXFLAGS)

# Directories to search for header files:
INCLUDE_DIRS := JuceLibraryCode deps/JUCE/modules

# Directories to recursively search for header files:
RECURSIVE_INCLUDE_DIRS := Source Tests

#### Setup: ####

# build with "V=1" for verbose builds
ifeq ($(V), 1)
    V_AT =
else
    V_AT = @
endif

# Disable dependency generation if multiple architectures are set
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

# Generate the list of directory include flags:
DIR_FLAGS := $(shell echo $(INCLUDE_DIRS) | xargs printf " -I'%s'") \
             $(shell find $(RECURSIVE_INCLUDE_DIRS) -type d \
                     -printf " -I'%p'")

# Generate the build label from the /etc/os-release file:
BUILD_NAME := $(shell ./project-scripts/BuildLabel.sh)

# Keep debug and release build files in separate directories:
JUCE_OBJDIR := $(JUCE_OBJDIR)/$(CONFIG)
JUCE_OUTDIR := $(JUCE_OUTDIR)/$(CONFIG)

ifeq ($(CONFIG),Debug)
    # Disable optimization and enable gdb flags and tests unless otherwise
    # specified:
    OPTIMIZATION ?= 0
    GDB_SUPPORT ?= 1
    BUILD_TESTS ?= 1
    # Debug-specific preprocessor definitions:
    JUCE_CONFIG_FLAGS = -DDEBUG=1 -D_DEBUG=1
endif

ifeq ($(CONFIG),Release)
    # Enable optimization and disable gdb flags and tests unless otherwise
    # specified:
    OPTIMIZATION ?= 1
    GDB_SUPPORT ?= 0
    BUILD_TESTS ?= 0
    # Release-specific preprocessor definitions:
    JUCE_CONFIG_FLAGS = -DNDEBUG=1
endif

# Set optimization level flags:
ifeq ($(OPTIMIZATION), 1)
    CONFIG_CFLAGS := $(CONFIG_CFLAGS) -O3 -flto
    CONFIG_LDFLAGS := $(CONFIG_LDFLAGS) -flto
else
    CONFIG_CFLAGS := $(CONFIG_CFLAGS) -O0
endif

# Set debugging flags:
ifeq ($(GDB_SUPPORT), 1)
    CONFIG_CFLAGS := $(CONFIG_CFLAGS) -g -ggdb
else
    CONFIG_LDFLAGS := $(CONFIG_LDFLAGS) -fvisibility=hidden
endif

# Add preprocessor flags enabling optional features:
ifeq ($(BUILD_TESTS), 1)
    FEATURE_DEFS := $(FEATURE_DEFS) -DINCLUDE_TESTING
endif

JUCE_CPPFLAGS := $(DEPFLAGS) \
                 $(JUCE_CONFIG_FLAGS) \
	             $(JUCE_DEFS)\
	             $(FEATURE_DEFS)\
                 -DBUILD_NAME="\"$(shell ./project-scripts/BuildLabel.sh)\"" \
	             $(shell pkg-config --cflags $(PKG_CONFIG_LIBS)) \
	             $(DIR_FLAGS)

JUCE_CFLAGS   += $(JUCE_CPPFLAGS) \
				 $(TARGET_ARCH) \
				 $(CONFIG_CFLAGS) \
				 $(CFLAGS)

JUCE_CXXFLAGS += $(JUCE_CFLAGS) \
				 $(CXXFLAGS)

JUCE_LDFLAGS  += $(TARGET_ARCH) \
	             -L$(JUCE_BINDIR) \
	             -L$(JUCE_LIBDIR) \
	             $(shell pkg-config --libs $(PKG_CONFIG_LIBS)) \
	             $(CONFIG_LDFLAGS) \
	             $(LDFLAGS)

CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)


.PHONY: build install debug release clean strip uninstall help
build : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

# Split modules up by module groups:
MODULES = framework development system file gui main

# Include makefiles defining each module:
include $(shell pwd)/JuceLibraryCode/Makefile
include $(shell pwd)/makefiles/*.mk


OBJECTS_MAIN := \
  $(JUCE_OBJDIR)/Main.o \
  $(JUCE_OBJDIR)/HomeApplication.o \
  $(JUCE_OBJDIR)/HomeWindow.o \
  $(JUCE_OBJDIR)/Chord.o


OBJECTS_MAIN_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_MAIN := $(OBJECTS_MAIN) $(OBJECTS_MAIN_TEST)
endif

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_MAIN)

# Module group build targets:
framework : $(FRAMEWORK_MODULES)
	@echo "Built Framework modules"

development : $(DEVELOPMENT_MODULES)
	@echo "Built Development modules"

system : $(SYSTEM_MODULES)
	@echo "Built System modules"

file : $(FILE_MODULES)
	@echo "Built File modules"

gui : $(GUI_MODULES)
	@echo "Built GUI modules"

main : $(OBJECTS_MAIN)
	@echo "Built $(JUCE_TARGET_APP)"

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : check-pkg-config $(MODULES) $(RESOURCES)
	@echo Linking "$(JUCE_TARGET_APP)"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) \
		             $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) \
					 $(TARGET_ARCH)

$(OBJECTS_APP) :
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "      Compiling: $(<F)"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
		-o "$@" -c "$<"

install:
	killall $(JUCE_TARGET_APP);\
	sudo cp build/$(CONFIG)/$(JUCE_TARGET_APP) /usr/bin/$(JUCE_TARGET_APP) && \
	if [ ! -d $(DATA_PATH) ]; then \
		sudo mkdir $(DATA_PATH) ; \
	fi && \
	sudo cp -R assets/* $(DATA_PATH)

debug:
	$(MAKE) CONFIG=Debug
	reset
	$(MAKE) install CONFIG=Debug
	gdb $(JUCE_TARGET_APP)

release:
	$(MAKE) CONFIG=Release
	reset
	$(MAKE) install CONFIG=Release

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 \
		"pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors $(PKG_CONFIG_LIBS); \
    if [ $(CHECK_DEPS) == 1 ]; then \
 		echo "Checking for outdated dependencies:"; \
        ./project-scripts/DepClean.sh $(JUCE_OBJDIR); \
	fi
clean:
	@echo Cleaning $(JUCE_TARGET_APP)
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping $(JUCE_TARGET_APP)
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

uninstall:
	@echo Uninstalling $(JUCE_TARGET_APP)
	killall $(JUCE_TARGET_APP);\
	sudo rm /usr/bin/$(JUCE_TARGET_APP) && \
	sudo rm -r  $(DATA_PATH) ; \

help:
	@echo "$$HELPTEXT"

-include $(OBJECTS_APP:%.o=%.d)

$(JUCE_OBJDIR)/Main.o: \
	Source/Main.cpp
$(JUCE_OBJDIR)/HomeApplication.o: \
	Source/HomeApplication.cpp
$(JUCE_OBJDIR)/HomeWindow.o: \
	Source/HomeWindow.cpp
$(JUCE_OBJDIR)/Chord.o: \
	Source/Chord.cpp
