############################## Text Module #####################################
TEXT_DIR = Source/GUI/Text
TEXT_TEST_DIR = Tests/GUI/Text

TEXT_CHARSET_DIR = $(TEXT_DIR)/CharSet

TEXT_PREFIX = Text_
TEXT_OBJ := $(JUCE_OBJDIR)/$(TEXT_PREFIX)

TEXT_CHARSET_PREFIX = $(TEXT_PREFIX)CharSet_
TEXT_CHARSET_OBJ = $(TEXT_OBJ)CharSet_
OBJECTS_TEXT_CHARSET := \
  $(TEXT_CHARSET_OBJ)Cache.o \
  $(TEXT_CHARSET_OBJ)JSONResource.o \
  $(TEXT_CHARSET_OBJ)ConfigFile.o


OBJECTS_TEXT := \
  $(TEXT_OBJ)BinaryFont.o \
  $(TEXT_OBJ)Painter.o \
  $(TEXT_OBJ)Values.o \
  $(OBJECTS_TEXT_CHARSET)

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

$(TEXT_CHARSET_OBJ)Cache.o: \
	$(TEXT_CHARSET_DIR)/$(TEXT_CHARSET_PREFIX)Cache.cpp
$(TEXT_CHARSET_OBJ)JSONResource.o: \
	$(TEXT_CHARSET_DIR)/$(TEXT_CHARSET_PREFIX)JSONResource.cpp
$(TEXT_CHARSET_OBJ)ConfigFile.o: \
	$(TEXT_CHARSET_DIR)/$(TEXT_CHARSET_PREFIX)ConfigFile.cpp

$(TEXT_OBJ)BinaryFont.o: \
	$(TEXT_DIR)/$(TEXT_PREFIX)BinaryFont.cpp
$(TEXT_OBJ)Painter.o: \
	$(TEXT_DIR)/$(TEXT_PREFIX)Painter.cpp
$(TEXT_OBJ)Values.o: \
	$(TEXT_DIR)/$(TEXT_PREFIX)Values.cpp
