########################## Component Module ####################################
COMPONENT_DIR = Source/GUI/Component
COMPONENT_TEST_DIR = Tests/GUI/Component

COMPONENT_PREFIX = Component_
COMPONENT_OBJ := $(JUCE_OBJDIR)/$(COMPONENT_PREFIX)

OBJECTS_COMPONENT := \
  $(COMPONENT_OBJ)MainView.o \
  $(COMPONENT_OBJ)ChordKeyDisplay.o

COMPONENT_TEST_PREFIX := $(COMPONENT_PREFIX)Test_
COMPONENT_TEST_OBJ := $(COMPONENT_OBJ)Test_
OBJECTS_COMPONENT_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_COMPONENT := $(OBJECTS_COMPONENT) $(OBJECTS_COMPONENT_TEST)
endif

component : $(OBJECTS_COMPONENT)
	@echo "    Built Component module"

GUI_MODULES := $(GUI_MODULES) component

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_COMPONENT)

$(COMPONENT_OBJ)MainView.o: \
	$(COMPONENT_DIR)/$(COMPONENT_PREFIX)MainView.cpp
$(COMPONENT_OBJ)ChordKeyDisplay.o: \
	$(COMPONENT_DIR)/$(COMPONENT_PREFIX)ChordKeyDisplay.cpp
