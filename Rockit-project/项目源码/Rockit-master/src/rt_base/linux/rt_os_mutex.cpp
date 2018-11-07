/*
 * Copyright 2018 The Rockit Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * author: martin.cheng
 *   date: 20180719
 */

#define HAS_PTHREAD
#ifdef HAS_PTHREAD

#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#include "rt_header.h"
#include "rt_mutex.h"
#include "rt_mem.h"
#include "rt_time.h"
/*
typedef struct rt_mutex_data {
    pthread_mutex_t mMutex;
} RtMutexData;

typedef struct rt_condition_data {
    pthread_cond_t mCond;
} RtConditionData;
*/
/*
 * using pthread_mutex api to impliment RtMutex
 */
RtMutex::RtMutex() {
  //  RtMutexData* data = rt_malloc(RtMutexData);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  //  pthread_mutex_init(&(data->mMutex), &attr);
    pthread_mutex_init(&mMutex, &attr);
    pthread_mutexattr_destroy(&attr);
    //mData = (void*)data;
}

RtMutex::~RtMutex() {
   // RtMutexData* data = (RtMutexData*)mData;
   // pthread_mutex_destroy(&(data->mMutex));
   // rt_free(data);
   // mData = RT_NULL;
   pthread_mutex_destroy(&mMutex);
}

void RtMutex::lock() {
   // RtMutexData* data = (RtMutexData*)mData;
   // pthread_mutex_lock(&(data->mMutex));
   pthread_mutex_lock(&mMutex);
}

void RtMutex::unlock() {
  //  RtMutexData* data = (RtMutexData*)mData;
   // pthread_mutex_unlock(&(data->mMutex));
   pthread_mutex_unlock(&mMutex);
}

int RtMutex::trylock() {
   // RtMutexData* data = (RtMutexData*)mData;
   // return pthread_mutex_trylock(&(data->mMutex));
   return pthread_mutex_trylock(&mMutex);
}

/*
 * using pthread_cond api to impliment RtCondition
 */
RtCondition::RtCondition() {
  /*  RtConditionData* data = rt_malloc(RtConditionData);
    pthread_cond_init(&(data->mCond), NULL);
    mData = (void*)data;
  */
    pthread_cond_init(&mCond, NULL);
}

RtCondition::~RtCondition() {
 /*   RtConditionData* data = (RtConditionData*)mData;
    pthread_cond_destroy(&(data->mCond));
    rt_free(data);
    mData = RT_NULL;
  */
    pthread_cond_destroy(&mCond);
}

INT32 RtCondition::wait(RtMutex& RtMutex) {
 /*   RtConditionData* cond_data = (RtConditionData*)mData;
    RtMutexData*     lock_data = (RtMutexData*)(RtMutex.mData);
    return pthread_cond_wait(&(cond_data->mCond), &(lock_data->mMutex));
  */
    return pthread_cond_wait(&mCond, &RtMutex.mMutex);
}

INT32 RtCondition::wait(RtMutex* RtMutex) {
 /*   RtConditionData* cond_data = (RtConditionData*)mData;
    RtMutexData*     lock_data = (RtMutexData*)(RtMutex->mData);
    return pthread_cond_wait(&(cond_data->mCond), &(lock_data->mMutex));
 */
    return pthread_cond_wait(&mCond, &RtMutex->mMutex);
}

INT32 RtCondition::timedwait(RtMutex& RtMutex, UINT64 timeout) {
    return timedwait(&RtMutex, timeout);
}

INT32 RtCondition::timedwait(RtMutex* RtMutex, UINT64 timeout) {
    struct timespec ts;

    UINT64 now_us = RtTime::getNowTimeUs();

    ts.tv_sec = now_us/1000000;
    ts.tv_nsec = (now_us%1000000)*1000;
    ts.tv_sec += timeout / 1000;
    ts.tv_nsec += (timeout % 1000) * 1000000;
    /* Prevent the out of range at nanoseconds field */
    ts.tv_sec += ts.tv_nsec / 1000000000;
    ts.tv_nsec %= 1000000000;

 /*   RtConditionData* cond_data = (RtConditionData*)mData;
    RtMutexData*     lock_data = (RtMutexData*)(RtMutex->mData);
    return pthread_cond_timedwait(&(cond_data->mCond), &(lock_data->mMutex), &ts);
  */
  return pthread_cond_timedwait(&mCond,&RtMutex->mMutex,&ts);
}

INT32 RtCondition::signal() {
 /*   RtConditionData* data = (RtConditionData*)mData;
    return pthread_cond_signal(&(data->mCond));
  */
  return pthread_cond_signal(&mCond);
}

INT32 RtCondition::broadcast() {
/*    RtConditionData* data = (RtConditionData*)mData;
    return pthread_cond_broadcast(&(data->mCond));
*/
    return pthread_cond_broadcast(&mCond);
}

#endif
