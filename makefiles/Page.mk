############################## Page Module #####################################
PAGE_DIR = Source/GUI/Page

PAGE_PREFIX := Page_
PAGE_OBJ := $(JUCE_OBJDIR)/$(PAGE_PREFIX)

PAGE_TYPES_DIR := $(PAGE_DIR)/PageTypes
OBJECTS_PAGE_TYPES :=


PAGE_INTERFACE_DIR := $(PAGE_DIR)/Interface
PAGE_INTERFACE_PREFIX := $(PAGE_PREFIX)Interface_
PAGE_INTERFACE_OBJ := $(PAGE_OBJ)Interface_

OBJECTS_PAGE := \
  $(PAGE_INTERFACE_OBJ)Component.o \
  $(PAGE_INTERFACE_OBJ)FactoryHolder.o \
  $(PAGE_OBJ)Component.o \
  $(PAGE_OBJ)StackComponent.o \
  $(PAGE_OBJ)Factory.o \
  $(OBJECTS_PAGE_TYPES)

OBJECTS_PAGE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PAGE := $(OBJECTS_PAGE) $(OBJECTS_PAGE_TEST)
endif

GUI_MODULES := $(GUI_MODULES) page

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PAGE)

page : $(OBJECTS_PAGE)
	@echo "    Built Page module"

$(PAGE_INTERFACE_OBJ)Component.o : \
    $(PAGE_INTERFACE_DIR)/$(PAGE_INTERFACE_PREFIX)Component.cpp
$(PAGE_INTERFACE_OBJ)FactoryHolder.o : \
    $(PAGE_INTERFACE_DIR)/$(PAGE_INTERFACE_PREFIX)FactoryHolder.cpp

$(PAGE_OBJ)Component.o : \
    $(PAGE_DIR)/$(PAGE_PREFIX)Component.cpp
$(PAGE_OBJ)Factory.o : \
    $(PAGE_DIR)/$(PAGE_PREFIX)Factory.cpp
$(PAGE_OBJ)StackComponent.o : \
    $(PAGE_DIR)/$(PAGE_PREFIX)StackComponent.cpp

