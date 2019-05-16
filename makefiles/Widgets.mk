############################## Widget Module ###################################
WIDGET_ROOT   = Source/GUI/Widgets
WIDGET_PREFIX = Widgets_
WIDGET_OBJ   := $(JUCE_OBJDIR)/$(WIDGET_PREFIX)

OBJECTS_WIDGET := \
  $(WIDGET_OBJ)BoundedLabel.o \
  $(WIDGET_OBJ)TextEditor.o \
  $(WIDGET_OBJ)ColourPicker.o \
  $(WIDGET_OBJ)Counter.o \
  $(WIDGET_OBJ)DrawableImage.o \
  $(WIDGET_OBJ)DrawableImageButton.o \
  $(WIDGET_OBJ)FilePathEditor.o \
  $(WIDGET_OBJ)LayoutContainer.o \
  $(WIDGET_OBJ)PagedList.o \
  $(WIDGET_OBJ)FocusingPagedList.o \
  $(WIDGET_OBJ)NavButton.o \
  $(WIDGET_OBJ)IconSlider.o \
  $(WIDGET_OBJ)DelayedIconSlider.o \
  $(WIDGET_OBJ)PopupEditor.o \
  $(WIDGET_OBJ)ListEditor.o \
  $(WIDGET_OBJ)Spinner.o \
  $(WIDGET_OBJ)OverlaySpinner.o \
  $(WIDGET_OBJ)Switch.o

OBJECTS_WIDGET_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WIDGET := $(OBJECTS_WIDGET) $(OBJECTS_WIDGET_TEST)
endif

GUI_MODULES := $(GUI_MODULES) widget

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WIDGET)

widget : $(OBJECTS_WIDGET)
	@echo "    Built Widget module"

$(WIDGET_OBJ)BoundedLabel.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)BoundedLabel.cpp

$(WIDGET_OBJ)TextEditor.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)TextEditor.cpp

$(WIDGET_OBJ)ColourPicker.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)ColourPicker.cpp

$(WIDGET_OBJ)Counter.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)Counter.cpp

$(WIDGET_OBJ)DrawableImage.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)DrawableImage.cpp

$(WIDGET_OBJ)DrawableImageButton.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)DrawableImageButton.cpp

$(WIDGET_OBJ)FilePathEditor.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)FilePathEditor.cpp

$(WIDGET_OBJ)LayoutContainer.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)LayoutContainer.cpp

$(WIDGET_OBJ)PagedList.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)PagedList.cpp

$(WIDGET_OBJ)FocusingPagedList.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)FocusingPagedList.cpp

$(WIDGET_OBJ)NavButton.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)NavButton.cpp

$(WIDGET_OBJ)IconSlider.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)IconSlider.cpp

$(WIDGET_OBJ)DelayedIconSlider.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)DelayedIconSlider.cpp

$(WIDGET_OBJ)PopupEditor.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)PopupEditor.cpp

$(WIDGET_OBJ)ListEditor.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)ListEditor.cpp

$(WIDGET_OBJ)Spinner.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)Spinner.cpp

$(WIDGET_OBJ)OverlaySpinner.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)OverlaySpinner.cpp

$(WIDGET_OBJ)Switch.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)Switch.cpp
