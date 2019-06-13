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
