########################## Utility Module ######################################
UTIL_DIR = Source/Framework/Util
UTIL_TEST_DIR = Tests/Framework/Util

UTIL_PREFIX = Util_
UTIL_OBJ := $(JUCE_OBJDIR)/$(UTIL_PREFIX)

OBJECTS_UTIL := \
  $(UTIL_OBJ)Commands.o \
  $(UTIL_OBJ)TempTimer.o \
  $(UTIL_OBJ)ShutdownListener.o \
  $(UTIL_OBJ)ConditionChecker.o

UTIL_TEST_PREFIX := $(UTIL_PREFIX)Test_
UTIL_TEST_OBJ := $(UTIL_OBJ)Test_
OBJECTS_UTIL_TEST := \
  $(UTIL_TEST_OBJ)ShutdownListenerTest.o \
  $(UTIL_TEST_OBJ)ConditionTest.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_UTIL := $(OBJECTS_UTIL) $(OBJECTS_UTIL_TEST)
endif

FRAMEWORK_MODULES := $(FRAMEWORK_MODULES) util

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_UTIL)

util : $(OBJECTS_UTIL)
	@echo "    Built Utility module"

$(UTIL_OBJ)Commands.o : \
    $(UTIL_DIR)/$(UTIL_PREFIX)Commands.cpp
$(UTIL_OBJ)TempTimer.o : \
    $(UTIL_DIR)/$(UTIL_PREFIX)TempTimer.cpp
$(UTIL_OBJ)ShutdownListener.o : \
    $(UTIL_DIR)/$(UTIL_PREFIX)ShutdownListener.cpp
$(UTIL_OBJ)ConditionChecker.o : \
    $(UTIL_DIR)/$(UTIL_PREFIX)ConditionChecker.cpp

$(UTIL_TEST_OBJ)ShutdownListenerTest.o : \
    $(UTIL_TEST_DIR)/$(UTIL_TEST_PREFIX)ShutdownListenerTest.cpp
$(UTIL_TEST_OBJ)ConditionTest.o : \
    $(UTIL_TEST_DIR)/$(UTIL_TEST_PREFIX)ConditionTest.cpp
