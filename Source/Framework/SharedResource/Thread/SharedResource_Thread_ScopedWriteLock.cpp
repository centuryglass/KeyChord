#include "SharedResource_Thread_ScopedWriteLock.h"
#include "SharedResource_Thread_Lock.h"

// Locks a resource lock for writing for as long as this object exists.
SharedResource::Thread::ScopedWriteLock::ScopedWriteLock(Lock& threadLock) :
        juceLock(threadLock.getResourceLock()) { }
