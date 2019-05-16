############################ Theme Module ######################################
THEME_DIR    = Source/GUI/Theme
THEME_COLOUR_DIR   := $(THEME_DIR)/Colour
THEME_IMAGE_DIR   := $(THEME_DIR)/Image

THEME_PREFIX  = Theme_
THEME_OBJ    := $(JUCE_OBJDIR)/$(THEME_PREFIX)

THEME_COLOUR_PREFIX := $(THEME_PREFIX)Colour_
THEME_COLOUR_OBJ    := $(THEME_OBJ)Colour_
OBJECTS_THEME_COLOUR := \
  $(THEME_COLOUR_OBJ)Element.o \
  $(THEME_COLOUR_OBJ)JSONKeys.o \
  $(THEME_COLOUR_OBJ)JSONResource.o \
  $(THEME_COLOUR_OBJ)ConfigFile.o \
  $(THEME_COLOUR_OBJ)ConfigListener.o \
  $(THEME_COLOUR_OBJ)ConfigPage.o

THEME_IMAGE_PREFIX := $(THEME_PREFIX)Image_
THEME_IMAGE_OBJ    := $(THEME_OBJ)Image_
OBJECTS_THEME_IMAGE := \
  $(THEME_IMAGE_OBJ)AssetList.o \
  $(THEME_IMAGE_OBJ)JSONResource.o \
  $(THEME_IMAGE_OBJ)ConfigFile.o \
  $(THEME_IMAGE_OBJ)ConfigListener.o

OBJECTS_THEME := \
  $(OBJECTS_THEME_COLOUR) \
  $(OBJECTS_THEME_IMAGE) \
  $(THEME_OBJ)LookAndFeel.o

OBJECTS_THEME_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_THEME := $(OBJECTS_THEME) $(OBJECTS_THEME_TEST)
endif

GUI_MODULES := $(GUI_MODULES) theme

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_THEME)

theme : $(OBJECTS_THEME)
	@echo "    Built Theme module"

$(THEME_COLOUR_OBJ)Element.o : \
    $(THEME_COLOUR_DIR)/$(THEME_COLOUR_PREFIX)Element.cpp
$(THEME_COLOUR_OBJ)JSONKeys.o : \
    $(THEME_COLOUR_DIR)/$(THEME_COLOUR_PREFIX)JSONKeys.cpp
$(THEME_COLOUR_OBJ)JSONResource.o : \
    $(THEME_COLOUR_DIR)/$(THEME_COLOUR_PREFIX)JSONResource.cpp
$(THEME_COLOUR_OBJ)ConfigFile.o : \
    $(THEME_COLOUR_DIR)/$(THEME_COLOUR_PREFIX)ConfigFile.cpp
$(THEME_COLOUR_OBJ)ConfigListener.o : \
    $(THEME_COLOUR_DIR)/$(THEME_COLOUR_PREFIX)ConfigListener.cpp
$(THEME_COLOUR_OBJ)ConfigPage.o : \
    $(THEME_COLOUR_DIR)/$(THEME_COLOUR_PREFIX)ConfigPage.cpp

$(THEME_IMAGE_OBJ)AssetList.o : \
    $(THEME_IMAGE_DIR)/$(THEME_IMAGE_PREFIX)AssetList.cpp
$(THEME_IMAGE_OBJ)JSONResource.o : \
    $(THEME_IMAGE_DIR)/$(THEME_IMAGE_PREFIX)JSONResource.cpp
$(THEME_IMAGE_OBJ)ConfigFile.o : \
    $(THEME_IMAGE_DIR)/$(THEME_IMAGE_PREFIX)ConfigFile.cpp
$(THEME_IMAGE_OBJ)ConfigListener.o : \
    $(THEME_IMAGE_DIR)/$(THEME_IMAGE_PREFIX)ConfigListener.cpp

$(THEME_OBJ)LookAndFeel.o : \
    $(THEME_DIR)/Theme_LookAndFeel.cpp
