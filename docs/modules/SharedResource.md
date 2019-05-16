# SharedResource Module Documentation
The SharedResource module provides a framework for implementing threadsafe, reference counted singleton classes.

Shared resources are implemented as subclasses of SharedResource\::Resource. Each Resource subclass has at most one object instance. Resources may only be directly interacted with through subclasses of the SharedResource\::Handler class. Sharing a single object like this ensures that concurrent access to system resources can be controlled, and expensive objects are not duplicated unnecessarily.

Each Resource subclass has at least one SharedResource\::Handler class assigned to it. These handlers control all access to the Resource object instance, and control its lifecycle. The Resource object is only created when one of its Handler objects is created, and it is only destroyed when all of its handlers have been destroyed. This allows SharedResource initialization and destruction to be controlled using RAII techniques, by declaring a Handler at the highest scope where the resource will be needed.

Each Resource object instance can only be accessed by its handlers while it is locked, either for reading or writing, as defined by juce\::ReadWriteLock. This ensures that Resource data will not be changed while it is being read, or while another thread is already making changes. This allows Handler objects across multiple threads to share a single resource object safely. To prevent deadlocks, Handler objects with different resources should interact with each other very carefully, taking extra care not to introduce circular dependencies.

Resources are able to selectively apply functions to all of their Handler objects, or to only Handler objects with a specific subclass. This allows Handler objects to be implemented as Listener objects, receiving updates whenever the Resource requires.

#### [SharedResource\::Resource](../../Source/Framework/SharedResource/SharedResource_Resource.h)
The Resource class provides an abstract basis for shared, threadsafe, reference counted singleton classes.

#### [SharedResource\::Handler](../../Source/Framework/SharedResource/SharedResource_Handler.h)
The Handler class provides an abstract basis for classes that safely access a single Resource subclass object instance. Resources objects are constructed when their first Handler object is constructed, and destroyed when their last Handler object is destroyed.

#### [SharedResource\::LockedPtr](../../Source/Framework/SharedResource/SharedResource_LockedPtr.h)
LockedPtr objects are used by Handler objects to access their resource. As long as the LockedPtr object is in scope, the resource it accesses will remain locked. LockedPtr objects may be created to lock the resource for either reading or writing.

## Modular Resources
Some system resources need to be shared within the application, but are too complex to reasonably manage from within a single Resource class. Modular resources allow one Resource object to divide its data and responsibilities between any number of unique, specialized Module objects. Module objects may freely access other Module objects that belong to the same resource. Handler objects may be created that may only access a specific Module of a resource.

#### [SharedResource\::Modular\::Resource](../../Source/Framework/SharedResource/Modular/SharedResource_Modular_Resource.h)
Modular\::Resource is an abstract Resource subclass that holds a predetermined set of Module objects. Modular resources must create all of their modules on construction, and destroy all of their modules only on their own destruction. Modular resource classes must implement normal and const specializations of the getModule template function for each Module subclass they manage.

#### [SharedResource\::Modular\::Module](../../Source/Framework/SharedResource/Modular/SharedResource_Modular_Module.h)
Module objects handle a set of specialized responsibilities for a modular resource. Each module should be a distinct subclass of the Module class. Each module is associated with a single Resource subclass, and may only be created by that specific resource's object instance. Modules may freely access their own resource, or any of the other modules held by the same resource.

#### [SharedResource\::Modular\::Handler](../../Source/Framework/SharedResource/Modular/SharedResource_Modular_Handler.h)
The modular Handler class provides a basis for Handler objects that treat a single resource module as if it was an independant resource. Each modular Handler subclass may only access a single Module instance within a single Resource instance. Modular Handlers otherwise function identically to typical resource Handlers.

#### [SharedResource\::Modular\::LockedPtr](../../Source/Framework/SharedResource/Modular/SharedResource_Modular_LockedPtr.h)
The modular LockedPtr class allows modular Handler objects to securely access their Module the same way SharedResource\::LockedPtr objects provide secure access to a Resource.


## Resource Threads
Dedicated worker threads that take responsibility for a single task can be implemented as shared resources. The Thread submodule provides tools that allow these resources to safely control access to their stored data.

#### [SharedResource\::Thread\::Thread](../../Source/Framework/SharedResource/Thread/SharedResource_Thread_Thread.h)
The Thread class is the basis used to provide juce\::Thread functionality to Resource subclasses. Thread objects may lock themselves while running to prevent concurrent data access, and automatically stop their thread when the application starts to shut down. When inactive, Thread object threads may be configured to either sleep until notified, or to shut down entirely.

#### [SharedResource\::Thread\::Resource](../../Source/Framework/SharedResource/Thread/SharedResource_Thread_Resource.h)
The thread Resource class is an abstract basis for Thread classes that are also Resource classes. All of their functionality is inherited from the Thread and Resource classes.

#### [SharedResource\::Thread\::Module](../../Source/Framework/SharedResource/Thread/SharedResource_Thread_Module.h)
The thread Module class is an abstract basis for Thread classes that are also resource Module classes. All of their functionality is inherited from the Thread and Module classes.

#### [SharedResource\::Thread\::Lock](../../Source/Framework/SharedResource/Thread/SharedResource_Thread_Lock.h)
Lock objects are specialized handler objects created by Resource and Module threads to access their content lock, and prevent themselves from being destroyed while their thread is executing.

#### [SharedResource\::Thread\::ScopedWriteLock](../../Source/Framework/SharedResource/Thread/SharedResource_Thread_ScopedWriteLock.h)
ScopedWriteLock objects prevent any threads from accessing a thread Resource or Module besides the one that holds the write lock. They are identical to juce\::ScopedWriteLock objects, except that they're created to control a Thread\::Lock instead of a juce\::ReadWriteLock.

#### [SharedResource\::Thread\::ScopedReadLock](../../Source/Framework/SharedResource/Thread/SharedResource_Thread_ScopedReadLock.h)
ScopedReadLock objects prevent other threads from locking a thread Resource or Module for writing. They are identical to juce\::ScopedReadLock objects, except that they're created to control a Thread\::Lock instead of a juce\::ReadWriteLock.

## Private Implementation Classes

#### [SharedResource\::Instance](../../Source/Framework/SharedResource/Implementation/SharedResource_Instance.h)
Instance is the basis shared by all Resource classes. The instance class tracks the list of all existing Reference objects that are assigned to it, ensuring that each instance object is destroyed when its last Reference is removed.

#### [SharedResource\::Reference](../../Source/Framework/SharedResource/Implementation/SharedResource_Reference.h)
Reference is the basis shared by all Handler classes. Each Reference connects to a specific Instance type, creating that Instance on construction if it doesn't already exist. Reference objects add themselves to their Instance's internal Reference list on construction, and remove themselves from the same list on destruction. Each Reference may freely access its associated Instance object, along with the juce\::ReadWriteLock used to protect that Instance.

#### [SharedResource\::ReferenceInterface](../../Source/Framework/SharedResource/Implementation/SharedResource_ReferenceInterface.h)
ReferenceInterface is the interface that Instance objects use to store and interact with their Reference objects.

#### [SharedResource\::Holder](../../Source/Framework/SharedResource/Implementation/SharedResource_Holder.h)
The Holder class stores all Instance objects, creating and sharing one juce\::ReadWriteLock per Instance. Reference objects use the Holder object to access their Instance and its lock. The Holder object prevents problems from occurring when Reference objects are created while their Instance is being created or destroyed.

#### [SharedResource\::LockType](../../Source/Framework/SharedResource/Implementation/SharedResource_LockType.h)
LockType lists the two types of locking allowed by juce\::ReadWriteLock objects so that a lock type may be easily requested as a function parameter.

#### [SharedResource\::LockedInstancePtr](../../Source/Framework/SharedResource/Implementation/SharedResource_LockedInstancePtr.h)
LockedInstancePtr is the basis shared by all LockedPtr classes. It provides access to an Instance, while also functioning as a juce\::ScopedReadLock or juce\::ScopedWriteLock.
