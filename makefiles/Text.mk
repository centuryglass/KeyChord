############################## Text Module #####################################
TEXT_DIR = Source/GUI/Text
TEXT_TEST_DIR = Tests/GUI/Text

TEXT_PREFIX = Text_
TEXT_OBJ := $(JUCE_OBJDIR)/$(TEXT_PREFIX)

OBJECTS_TEXT := \
  $(TEXT_OBJ)BinaryFont.o

TEXT_TEST_PREFIX := $(TEXT_PREFIX)Test_
TEXT_TEST_OBJ := $(TEXT_OBJ)Test_
OBJECTS_TEXT_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_TEXT := $(OBJECTS_TEXT) $(OBJECTS_TEXT_TEST)
endif

text : $(OBJECTS_TEXT)
	@echo "    Built Text module"

GUI_MODULES := $(GUI_MODULES) text

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_TEXT)

$(TEXT_OBJ)BinaryFont.o: \
	$(TEXT_DIR)/$(TEXT_PREFIX)BinaryFont.cpp
