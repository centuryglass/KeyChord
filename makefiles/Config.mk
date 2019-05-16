############################## Config Module ###################################
CONFIG_DIR = Source/Files/Config
CONFIG_IMPL_DIR := $(CONFIG_DIR)/Implementation
CONFIG_TEST_DIR = Tests/Files/Config

CONFIG_PREFIX = Config_
CONFIG_OBJ := $(JUCE_OBJDIR)/$(CONFIG_PREFIX)

OBJECTS_CONFIG_IMPL := \
  $(CONFIG_OBJ)AlertWindow.o

OBJECTS_CONFIG := \
  $(OBJECTS_CONFIG_IMPL) \
  $(CONFIG_OBJ)FileResource.o \
  $(CONFIG_OBJ)DataKey.o

CONFIG_TEST_PREFIX := $(CONFIG_PREFIX)Test_
CONFIG_TEST_OBJ := $(CONFIG_OBJ)Test_
OBJECTS_CONFIG_TEST := \
  $(CONFIG_TEST_OBJ)Resource.o \
  $(CONFIG_TEST_OBJ)FileHandler.o \
  $(CONFIG_TEST_OBJ)Listener.o \
  $(CONFIG_TEST_OBJ)ObjectData.o \
  $(CONFIG_TEST_OBJ)FileTest.o \


ifeq ($(BUILD_TESTS), 1)
    OBJECTS_CONFIG := $(OBJECTS_CONFIG) $(OBJECTS_CONFIG_TEST)
endif

FILE_MODULES := $(FILE_MODULES) config

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_CONFIG)

config : $(OBJECTS_CONFIG)
	@echo "    Built Config module"

$(CONFIG_OBJ)AlertWindow.o: \
    $(CONFIG_IMPL_DIR)/$(CONFIG_PREFIX)AlertWindow.cpp
$(CONFIG_OBJ)FileResource.o: \
    $(CONFIG_DIR)/$(CONFIG_PREFIX)FileResource.cpp
$(CONFIG_OBJ)DataKey.o: \
    $(CONFIG_DIR)/$(CONFIG_PREFIX)DataKey.cpp
$(CONFIG_OBJ)MainFile.o: \

$(CONFIG_TEST_OBJ)Resource.o: \
    $(CONFIG_TEST_DIR)/$(CONFIG_TEST_PREFIX)Resource.cpp
$(CONFIG_TEST_OBJ)FileHandler.o: \
    $(CONFIG_TEST_DIR)/$(CONFIG_TEST_PREFIX)FileHandler.cpp
$(CONFIG_TEST_OBJ)Listener.o: \
    $(CONFIG_TEST_DIR)/$(CONFIG_TEST_PREFIX)Listener.cpp
$(CONFIG_TEST_OBJ)ObjectData.o: \
    $(CONFIG_TEST_DIR)/$(CONFIG_TEST_PREFIX)ObjectData.cpp
$(CONFIG_TEST_OBJ)FileTest.o: \
    $(CONFIG_TEST_DIR)/$(CONFIG_TEST_PREFIX)FileTest.cpp
