############################ Output Module ####################################
OUTPUT_DIR = Source/GUI/Output
OUTPUT_TEST_DIR = Tests/GUI/Output

OUTPUT_PREFIX = Output_
OUTPUT_OBJ := $(JUCE_OBJDIR)/$(OUTPUT_PREFIX)

OBJECTS_OUTPUT := \
  $(OUTPUT_OBJ)Buffer.o \
  $(OUTPUT_OBJ)Modifiers.o \
  $(OUTPUT_OBJ)Sending.o

OUTPUT_TEST_PREFIX := $(OUTPUT_PREFIX)Test_
OUTPUT_TEST_OBJ := $(OUTPUT_OBJ)Test_
OBJECTS_OUTPUT_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_OUTPUT := $(OBJECTS_OUTPUT) $(OBJECTS_OUTPUT_TEST)
endif

output : $(OBJECTS_OUTPUT)
	@echo "    Built Output module"

GUI_MODULES := $(GUI_MODULES) output

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_OUTPUT)

$(OUTPUT_OBJ)Buffer.o: \
	$(OUTPUT_DIR)/$(OUTPUT_PREFIX)Buffer.cpp
$(OUTPUT_OBJ)Modifiers.o: \
	$(OUTPUT_DIR)/$(OUTPUT_PREFIX)Modifiers.cpp
$(OUTPUT_OBJ)Sending.o: \
	$(OUTPUT_DIR)/$(OUTPUT_PREFIX)Sending.cpp
