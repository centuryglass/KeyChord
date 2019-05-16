##########################  Hardware Module  ###################################
HARDWARE_DIR = Source/System/Hardware
HARDWARE_TEST_DIR = Tests/System/Hardware

HARDWARE_PREFIX = Hardware_
HARDWARE_OBJ := $(JUCE_OBJDIR)/$(HARDWARE_PREFIX)

OBJECTS_HARDWARE := \
  $(HARDWARE_OBJ)Audio.o \
  $(HARDWARE_OBJ)Battery.o \
  $(HARDWARE_OBJ)Display.o
ifeq ($(CHIP_FEATURES), 1)
    OBJECTS_HARDWARE := $(OBJECTS_HARDWARE) $(HARDWARE_OBJ)I2CBus.o
endif

HARDWARE_TEST_PREFIX := $(HARDWARE_PREFIX)Test_
HARDWARE_TEST_OBJ := $(HARDWARE_OBJ)Test_
OBJECTS_HARDWARE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_HARDWARE := $(OBJECTS_HARDWARE) $(OBJECTS_HARDWARE_TEST)
endif

SYSTEM_MODULES := $(SYSTEM_MODULES) hardware

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_HARDWARE)

hardware : $(OBJECTS_HARDWARE)
	@echo "    Built Hardware module"

$(HARDWARE_OBJ)Audio.o : \
    $(HARDWARE_DIR)/$(HARDWARE_PREFIX)Audio.cpp
$(HARDWARE_OBJ)Battery.o : \
    $(HARDWARE_DIR)/$(HARDWARE_PREFIX)Battery.cpp
$(HARDWARE_OBJ)Display.o : \
    $(HARDWARE_DIR)/$(HARDWARE_PREFIX)Display.cpp
$(HARDWARE_OBJ)I2CBus.o : \
    $(HARDWARE_DIR)/$(HARDWARE_PREFIX)I2CBus.cpp
