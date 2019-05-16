############################  Windows Module  ##################################
WINDOW_DIR = Source/System/Windows
WINDOW_TEST_DIR = Tests/System/Windows

WINDOW_PREFIX = Windows_
WINDOW_OBJ := $(JUCE_OBJDIR)/$(WINDOW_PREFIX)
OBJECTS_WINDOW := \
  $(WINDOW_OBJ)Info.o \
  $(WINDOW_OBJ)Alert.o \
  $(WINDOW_OBJ)MainWindow.o \
  $(WINDOW_OBJ)FocusListener.o \
  $(WINDOW_OBJ)FocusTracker.o \
  $(WINDOW_OBJ)FocusedTimer.o \
  $(WINDOW_OBJ)XInterface.o

WINDOW_TEST_PREFIX := $(WINDOW_PREFIX)Test_
WINDOW_TEST_OBJ := $(WINDOW_OBJ)Test_
OBJECTS_WINDOW_TEST := \
  $(WINDOW_TEST_OBJ)XInterfaceTest.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WINDOW := $(OBJECTS_WINDOW) $(OBJECTS_WINDOW_TEST)
endif

SYSTEM_MODULES := $(SYSTEM_MODULES) window

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WINDOW)

window : $(OBJECTS_WINDOW)
	@echo "    Built Window module"

$(WINDOW_OBJ)Info.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)Info.cpp

$(WINDOW_OBJ)Alert.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)Alert.cpp

$(WINDOW_OBJ)MainWindow.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)MainWindow.cpp

$(WINDOW_OBJ)FocusListener.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)FocusListener.cpp

$(WINDOW_OBJ)FocusTracker.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)FocusTracker.cpp

$(WINDOW_OBJ)FocusedTimer.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)FocusedTimer.cpp

$(WINDOW_OBJ)XInterface.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)XInterface.cpp

$(WINDOW_TEST_OBJ)XInterfaceTest.o : \
    $(WINDOW_TEST_DIR)/$(WINDOW_TEST_PREFIX)XInterfaceTest.cpp
