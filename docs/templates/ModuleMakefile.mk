######################## Module Makefile Template: #############################
#  Module names, submodule names, and module directories are all defined as 
# variables, so that they can be easily changed. This template uses 'Example'
# as the name of the module to build, 'ModuleCategory' as the module's category,
# and 'InnerModule' as an example submodule. Valid module categories are
# framework, development, system, file, and gui.
############################ Example Module ####################################
EXAMPLE_DIR = Source/ModuleCategory/Example
EXAMPLE_TEST_DIR = Tests/ModuleCategory/Example

# Add variables for all submodule directories using this format:
EXAMPLE_INNERMODULE_DIR := $(EXAMPLE_DIR)/InnerModule

EXAMPLE_PREFIX = Example_
EXAMPLE_OBJ := $(JUCE_OBJDIR)/$(EXAMPLE_PREFIX)

# Provide variables listing each inner module's object file targets:
EXAMPLE_INNERMODULE_PREFIX := $(EXAMPLE_PREFIX)InnerModule_
EXAMPLE_INNERMODULE_OBJ := $(EXAMPLE_OBJ)InnerModule_
OBJECTS_EXAMPLE_INNERMODULE := \
  $(EXAMPLE_INNERMODULE_OBJ)FirstClass.o \
  $(EXAMPLE_INNERMODULE_OBJ)SecondClass.o

# Declare a variable listing all object file targets in the module, including
# submodules:
OBJECTS_EXAMPLE := \
  $(EXAMPLE_OBJ)ThirdClass.o \
  $(OBJECTS_EXAMPLE_INNERMODULE)

# Add all module test classes if testing is enabled:
EXAMPLE_TEST_PREFIX := $(EXAMPLE_PREFIX)Test_
EXAMPLE_TEST_OBJ := $(EXAMPLE_OBJ)Test_
OBJECTS_EXAMPLE_TEST := \
  $(EXAMPLE_TEST_OBJ)FirstTest.o \
  $(EXAMPLE_TEST_OBJ)SecondTest.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_EXAMPLE := $(OBJECTS_EXAMPLE) $(OBJECTS_EXAMPLE_TEST)
endif

# Add a rule for building the entire module:
example : $(OBJECTS_EXAMPLE)
	@echo "    Built Example module"

# Add the module to the list of targets used by the module's category:
MODULECATEGORY_MODULES := $(MODULECATEGORY_MODULES) example

# Add all module object file rules to the project's object file rule list:
OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_APPMENU)

# Define the object file rules targeting each module source file:

# This rule would build the source file at 
# Source/ModuleCategory/Example/InnerModule/Example_InnerModule_FirstClass.cpp:
$(EXAMPLE_INNERMODULE_OBJ)FirstClass.o: \
	$(EXAMPLE_INNERMODULE_DIR)/$(EXAMPLE_INNERMODULE_PREFIX)FirstClass.cpp
$(EXAMPLE_INNERMODULE_OBJ)SecondClass.o: \
	$(EXAMPLE_INNERMODULE_DIR)/$(EXAMPLE_INNERMODULE_PREFIX)SecondClass.cpp

$(EXAMPLE_OBJ)ThirdClass.o: \
	$(EXAMPLE_DIR)/$(EXAMPLE_PREFIX)ThirdClass.cpp

$(EXAMPLE_TEST_OBJ)FirstTest.o: \
	$(EXAMPLE_TEST_DIR)/$(EXAMPLE_TEST_PREFIX)FirstTest.cpp
$(EXAMPLE_TEST_OBJ)SecondTest.o: \
	$(EXAMPLE_TEST_DIR)/$(EXAMPLE_TEST_PREFIX)SecondTest.cpp
