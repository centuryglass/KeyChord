#include "SharedResource_Thread_ScopedReadLock.h"
#include "SharedResource_Thread_Lock.h"

// Locks a resource lock for reading for as long as this object exists.
SharedResource::Thread::ScopedReadLock::ScopedReadLock(Lock& threadLock) :
        juceLock(threadLock.getResourceLock()) { }
