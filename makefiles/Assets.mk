############################ Assets Module  ####################################
ASSETS_DIR = Source/Files/Assets
ASSETS_TEST_DIR = Tests/Files/Assets

ASSETS_PREFIX = Assets_
ASSETS_OBJ := $(JUCE_OBJDIR)/$(ASSETS_PREFIX)

OBJECTS_ASSETS := \
  $(ASSETS_OBJ)Assets.o \
  $(ASSETS_OBJ)JSONFile.o \
  $(ASSETS_OBJ)XDGDirectories.o \
  $(ASSETS_OBJ)XPMLoader.o

ASSETS_TEST_PREFIX := $(ASSETS_PREFIX)Test_
ASSETS_TEST_OBJ := $(ASSETS_OBJ)Test_
OBJECTS_ASSETS_TEST := $(ASSETS_OBJ)XpmTest.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_ASSETS := $(OBJECTS_ASSETS) $(OBJECTS_ASSETS_TEST)
endif

FILE_MODULES := $(FILE_MODULES) assets

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_ASSETS)

assets : $(OBJECTS_ASSETS)
	@echo "    Built Assets module"

$(ASSETS_OBJ)Assets.o : \
    $(ASSETS_DIR)/Assets.cpp
$(ASSETS_OBJ)JSONFile.o : \
    $(ASSETS_DIR)/$(ASSETS_PREFIX)JSONFile.cpp
$(ASSETS_OBJ)XDGDirectories.o : \
    $(ASSETS_DIR)/$(ASSETS_PREFIX)XDGDirectories.cpp
$(ASSETS_OBJ)XPMLoader.o : \
    $(ASSETS_DIR)/$(ASSETS_PREFIX)XPMLoader.cpp

$(ASSETS_OBJ)XpmTest.o: \
	$(ASSETS_TEST_DIR)/$(ASSETS_PREFIX)XpmTest.cpp
