#pragma once
/**
 * @file  SharedResource_LockType.h
 *
 * @brief  Describes the types of locks that can be applied to a Resource.
 */

namespace SharedResource { enum class LockType; }

enum class SharedResource::LockType
{
    // Allow multiple threads to read from the resource, but keep them
    // from writing to the resource until all read locks are released.
    read,
    // Allows a single thread to read from and write to the resource,
    // blocking all others until the write lock is released.
    write
};
