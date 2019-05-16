########################## Locale Module ######################################
LOCALE_DIR = Source/Framework/Locale

LOCALE_PREFIX := Locale_
LOCALE_OBJ := $(JUCE_OBJDIR)/$(LOCALE_PREFIX)

OBJECTS_LOCALE := \
  $(LOCALE_OBJ)Time.o \
  $(LOCALE_OBJ)TextUser.o \
  $(LOCALE_OBJ)Locale.o

OBJECTS_LOCALE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_LOCALE := $(OBJECTS_LOCALE) $(OBJECTS_LOCALE_TEST)
endif

FRAMEWORK_MODULES := $(FRAMEWORK_MODULES) locale
OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_LOCALE)

locale : $(OBJECTS_LOCALE)
	@echo "    Built Locale module"

$(LOCALE_OBJ)Time.o: \
	$(LOCALE_DIR)/$(LOCALE_PREFIX)Time.cpp
$(LOCALE_OBJ)TextUser.o: \
	$(LOCALE_DIR)/$(LOCALE_PREFIX)TextUser.cpp
$(LOCALE_OBJ)Locale.o: \
	$(LOCALE_DIR)/Locale.cpp
