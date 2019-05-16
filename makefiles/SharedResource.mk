###################### SharedResource Module ###################################
SHARED_DIR = Source/Framework/SharedResource
SHARED_THREAD_DIR = $(SHARED_DIR)/Thread
SHARED_IMPL_DIR = $(SHARED_DIR)/Implementation
SHARED_TEST_DIR = Tests/Framework/SharedResource

SHARED_PREFIX = SharedResource_
SHARED_OBJ := $(JUCE_OBJDIR)/$(SHARED_PREFIX)

OBJECTS_SHARED_IMPL := \
  $(SHARED_OBJ)Holder.o \
  $(SHARED_OBJ)ReferenceInterface.o \
  $(SHARED_OBJ)Instance.o \
  $(SHARED_OBJ)Reference.o \
  $(SHARED_OBJ)LockedInstancePtr.o

SHARED_THREAD_PREFIX := $(SHARED_PREFIX)Thread_
SHARED_THREAD_OBJ := $(SHARED_OBJ)Thread_
OBJECTS_SHARED_THREAD := \
  $(SHARED_THREAD_OBJ)Lock.o \
  $(SHARED_THREAD_OBJ)ScopedReadLock.o \
  $(SHARED_THREAD_OBJ)ScopedWriteLock.o \
  $(SHARED_THREAD_OBJ)Thread.o \
  $(SHARED_THREAD_OBJ)Resource.o

OBJECTS_SHARED_RESOURCE := \
  $(OBJECTS_SHARED_IMPL) \
  $(OBJECTS_SHARED_THREAD) \
  $(SHARED_OBJ)Resource.o

SHARED_TEST_PREFIX := $(SHARED_PREFIX)Test_
SHARED_TEST_OBJ := $(SHARED_OBJ)Test_
OBJECTS_SHARED_TEST := \
  $(SHARED_TEST_OBJ)ModuleTest.o \
  $(SHARED_TEST_OBJ)ModuleTestClasses.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_SHARED_RESOURCE := $(OBJECTS_SHARED_RESOURCE) \
                               $(OBJECTS_SHARED_TEST)
endif

FRAMEWORK_MODULES := $(FRAMEWORK_MODULES) sharedResource

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_SHARED_RESOURCE)

sharedResource : $(OBJECTS_SHARED_RESOURCE)
	@echo "    Built SharedResource module"

$(SHARED_OBJ)Holder.o : \
    $(SHARED_IMPL_DIR)/$(SHARED_PREFIX)Holder.cpp
$(SHARED_OBJ)ReferenceInterface.o : \
    $(SHARED_IMPL_DIR)/$(SHARED_PREFIX)ReferenceInterface.cpp
$(SHARED_OBJ)Instance.o : \
    $(SHARED_IMPL_DIR)/$(SHARED_PREFIX)Instance.cpp
$(SHARED_OBJ)Reference.o : \
    $(SHARED_IMPL_DIR)/$(SHARED_PREFIX)Reference.cpp
$(SHARED_OBJ)LockedInstancePtr.o : \
    $(SHARED_IMPL_DIR)/$(SHARED_PREFIX)LockedInstancePtr.cpp
$(SHARED_OBJ)Resource.o : \
    $(SHARED_DIR)/$(SHARED_PREFIX)Resource.cpp

$(SHARED_THREAD_OBJ)Lock.o : \
    $(SHARED_THREAD_DIR)/$(SHARED_THREAD_PREFIX)Lock.cpp
$(SHARED_THREAD_OBJ)ScopedReadLock.o : \
    $(SHARED_THREAD_DIR)/$(SHARED_THREAD_PREFIX)ScopedReadLock.cpp
$(SHARED_THREAD_OBJ)ScopedWriteLock.o : \
    $(SHARED_THREAD_DIR)/$(SHARED_THREAD_PREFIX)ScopedWriteLock.cpp
$(SHARED_THREAD_OBJ)Thread.o : \
    $(SHARED_THREAD_DIR)/$(SHARED_THREAD_PREFIX)Thread.cpp
$(SHARED_THREAD_OBJ)Resource.o : \
    $(SHARED_THREAD_DIR)/$(SHARED_THREAD_PREFIX)Resource.cpp

$(SHARED_TEST_OBJ)ModuleTest.o : \
    $(SHARED_TEST_DIR)/$(SHARED_TEST_PREFIX)ModuleTest.cpp
$(SHARED_TEST_OBJ)ModuleTestClasses.o : \
    $(SHARED_TEST_DIR)/$(SHARED_TEST_PREFIX)ModuleTestClasses.cpp
