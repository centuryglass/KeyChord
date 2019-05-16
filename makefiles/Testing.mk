############################### Testing Module #################################
TEST_DIR = Source/Development/Testing

TEST_PREFIX := Testing_
TEST_OBJ := $(JUCE_OBJDIR)/$(TEST_PREFIX)

OBJECTS_TEST := \
  $(TEST_OBJ)StressTest.o \
  $(TEST_OBJ)Window.o \
  $(TEST_OBJ)DelayUtils.o


ifeq ($(BUILD_TESTS), 1)
    DEVELOPMENT_MODULES := $(DEVELOPMENT_MODULES) testing
    OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_TEST)
endif

testing : $(OBJECTS_TEST)
	@echo "    Built Testing module"

$(TEST_OBJ)StressTest.o: \
	$(TEST_DIR)/$(TEST_PREFIX)StressTest.cpp
$(TEST_OBJ)Window.o: \
	$(TEST_DIR)/$(TEST_PREFIX)Window.cpp
$(TEST_OBJ)DelayUtils.o: \
	$(TEST_DIR)/$(TEST_PREFIX)DelayUtils.cpp
