############################  Debug Module  ####################################
DEBUG_DIR = Source/Development/Debug
DEBUG_TEST_DIR = Tests/Development/Debug

DEBUG_PREFIX = Debug_
DEBUG_OBJ := $(JUCE_OBJDIR)/$(DEBUG_PREFIX)
OBJECTS_DEBUG := \
  $(DEBUG_OBJ)AddressLog.o \
  $(DEBUG_OBJ)Component.o \
  $(DEBUG_OBJ)ScopeTimer.o \
  $(DEBUG_OBJ)ScopeTimerRecords.o

DEBUG_DEBUG_PREFIX := $(DEBUG_PREFIX)Test_
DEBUG_DEBUG_OBJ := $(DEBUG_OBJ)Test_
OBJECTS_DEBUG_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_DEBUG := $(OBJECTS_DEBUG) $(OBJECTS_DEBUG_TEST)
endif

DEVELOPMENT_MODULES := $(DEVELOPMENT_MODULES) debugging

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_DEBUG)

debugging : $(OBJECTS_DEBUG)
	@echo "    Built Debug module"

$(DEBUG_OBJ)AddressLog.o: \
	$(DEBUG_DIR)/$(DEBUG_PREFIX)AddressLog.cpp
$(DEBUG_OBJ)Component.o: \
	$(DEBUG_DIR)/$(DEBUG_PREFIX)Component.cpp
$(DEBUG_OBJ)ScopeTimer.o: \
	$(DEBUG_DIR)/$(DEBUG_PREFIX)ScopeTimer.cpp
$(DEBUG_OBJ)ScopeTimerRecords.o: \
	$(DEBUG_DIR)/$(DEBUG_PREFIX)ScopeTimerRecords.cpp
