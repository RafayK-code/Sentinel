#pragma once
#ifndef _THREAD_UTIL_H
#define _THREAD_UTIL_H

#include <mutex>
#include <shared_mutex>

#ifndef SNTL_THREAD_SAFE
#define SNTL_MUTEX std::mutex sntlMtx
#define SNTL_SHARED_MUTEX std::shared_mutex sntlSharedMtx

#define SNTL_MUTABLE_MUTEX mutable std::mutex sntlMtx
#define SNTL_MUTABLE_SHARED_MUTEX mutable std::shared_mutex sntlSharedMtx

#define SNTL_LOCK_GUARD_SCOPE std::lock_guard<std::mutex> sntlLockGuard(sntlMtx)
#define SNTL_UNIQUE_LOCK_SCOPE std::unique_lock<std::shared_mutex> sntlUniqueLock(sntlSharedMtx)
#define SNTL_SHARED_LOCK_SCOPE std::shared_lock<std::shared_mutex> sntlSharedLock(sntlSharedMtx)

#define SNTL_UNIQUE_LOCK_LOCK sntlUniqueLock.lock()
#define SNTL_UNIQUE_LOCK_UNLOCK sntlUniqueLock.unlock()

#define SNTL_SHARED_LOCK_LOCK sntlSharedLock.lock()
#define SNTL_SHARED_LOCK_UNLOCK sntlSharedLock.unlock()

#else
#define SNTL_MUTEX
#define SNTL_SHARED_MUTEX

#define SNTL_LOCK_GUARD_SCOPE
#define SNTL_UNIQUE_LOCK_SCOPE
#define SNTL_SHARED_LOCK_SCOPE

#define SNTL_UNIQUE_LOCK_LOCK
#define SNTL_UNIQUE_LOCK_UNLOCK

#define SNTL_SHARED_LOCK_LOCK
#define SNTL_SHARED_LOCK_UNLOCK
#endif

#endif