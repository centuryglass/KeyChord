############################ Input Module ####################################
INPUT_DIR = Source/GUI/Input
INPUT_TEST_DIR = Tests/GUI/Input

INPUT_KEY_DIR := $(INPUT_DIR)/Key

INPUT_PREFIX = Input_
INPUT_OBJ := $(JUCE_OBJDIR)/$(INPUT_PREFIX)

INPUT_KEY_PREFIX := $(INPUT_PREFIX)Key_
INPUT_KEY_OBJ := $(INPUT_OBJ)Key_
OBJECTS_INPUT_KEY := \
  $(INPUT_KEY_OBJ)Alphabet.o \
  $(INPUT_KEY_OBJ)AlphabetFactory.o

OBJECTS_INPUT := \
  $(INPUT_OBJ)ChordReader.o \
  $(INPUT_OBJ)Buffer.o \
  $(INPUT_OBJ)Controller.o \
  $(OBJECTS_INPUT_KEY)

INPUT_TEST_PREFIX := $(INPUT_PREFIX)Test_
INPUT_TEST_OBJ := $(INPUT_OBJ)Test_
OBJECTS_INPUT_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_INPUT := $(OBJECTS_INPUT) $(OBJECTS_INPUT_TEST)
endif

input : $(OBJECTS_INPUT)
	@echo "    Built Input module"

GUI_MODULES := $(GUI_MODULES) input

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_INPUT)

$(INPUT_KEY_OBJ)Alphabet.o: \
	$(INPUT_KEY_DIR)/$(INPUT_KEY_PREFIX)Alphabet.cpp
$(INPUT_KEY_OBJ)AlphabetFactory.o: \
	$(INPUT_KEY_DIR)/$(INPUT_KEY_PREFIX)AlphabetFactory.cpp

$(INPUT_OBJ)Buffer.o: \
	$(INPUT_DIR)/$(INPUT_PREFIX)Buffer.cpp
$(INPUT_OBJ)ChordReader.o: \
	$(INPUT_DIR)/$(INPUT_PREFIX)ChordReader.cpp
$(INPUT_OBJ)Controller.o: \
	$(INPUT_DIR)/$(INPUT_PREFIX)Controller.cpp
