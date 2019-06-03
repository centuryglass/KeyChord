############################  Windows Module  ##################################
WINDOW_DIR = Source/Framework/Windows
WINDOW_TEST_DIR = Tests/Framework/Windows

WINDOW_PREFIX = Windows_
WINDOW_OBJ := $(JUCE_OBJDIR)/$(WINDOW_PREFIX)
OBJECTS_WINDOW := \
  $(WINDOW_OBJ)XInterface.o \
  $(WINDOW_OBJ)FocusControl.o

WINDOW_TEST_PREFIX := $(WINDOW_PREFIX)Test_
WINDOW_TEST_OBJ := $(WINDOW_OBJ)Test_
OBJECTS_WINDOW_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WINDOW := $(OBJECTS_WINDOW) $(OBJECTS_WINDOW_TEST)
endif

FRAMEWORK_MODULES := $(FRAMEWORK_MODULES) window

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WINDOW)

window : $(OBJECTS_WINDOW)
	@echo "    Built Window module"

$(WINDOW_OBJ)XInterface.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)XInterface.cpp
$(WINDOW_OBJ)FocusControl.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)FocusControl.cpp
