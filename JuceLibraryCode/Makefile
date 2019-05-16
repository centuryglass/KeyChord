########################## Juce Library Modules ################################
JUCE_PREFIX := $(JUCE_OBJDIR)/juce_
JUCE_ROOT = JuceLibraryCode

OBJECTS_JUCE := \
  $(JUCE_PREFIX)core.o \
  $(JUCE_PREFIX)data_structures.o \
  $(JUCE_PREFIX)events.o \
  $(JUCE_PREFIX)graphics.o \
  $(JUCE_PREFIX)gui_basics.o \
  $(JUCE_PREFIX)BinaryData.o 

MODULES := juce $(MODULES)

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_JUCE)

juce : $(OBJECTS_JUCE)
	@echo "Built JUCE library modules"
    
$(JUCE_PREFIX)BinaryData.o: \
	$(JUCE_ROOT)/BinaryData.cpp
$(JUCE_PREFIX)core.o: \
	$(JUCE_ROOT)/include_juce_core.cpp
$(JUCE_PREFIX)data_structures.o: \
	$(JUCE_ROOT)/include_juce_data_structures.cpp
$(JUCE_PREFIX)events.o: \
	$(JUCE_ROOT)/include_juce_events.cpp
$(JUCE_PREFIX)graphics.o: \
	$(JUCE_ROOT)/include_juce_graphics.cpp
$(JUCE_PREFIX)gui_basics.o: \
	$(JUCE_ROOT)/include_juce_gui_basics.cpp
