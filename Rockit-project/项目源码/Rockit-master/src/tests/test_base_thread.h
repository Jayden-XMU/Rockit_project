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
 *   date: 2018/07/19
 */

#ifndef __TEST_BASE_THREAD_H__
#define __TEST_BASE_THREAD_H__

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "base_thread"

#include <unistd.h> /*rand and usleep*/

#include "rt_mutex.h"
#include "rt_thread.h"
#define autolock_test 1
RtMutex xlock,xlock1;
RtMutex *lock0 = new RtMutex();
RtMutex *lock1 = new RtMutex();
RtCondition * rtcond = new RtCondition();
UINT8 unit_test_mutex() {
   // RT_LOGE("Fail to unit_test_mutex()");
    RtMutex *lock = new RtMutex();
    lock->lock();
    lock->unlock();
    delete lock;
  //  RtAutoMutex _l(xlock);
    RT_LOGE("success to unit_test_mutex()!");
    return RT_OK;
}

void* unit_test_thread_loop(void*) {
   int rand_sleep = (rand()%1000)*10000;
   while(1) {
   RT_LOGE("start");
   usleep(rand_sleep);
   RT_LOGE("done");
   }
   return NULL;
}

UINT8 unit_test_thread() {
    RtThread* td = new RtThread(unit_test_thread_loop, NULL);
    td->start();
    td->join();
    RT_LOGE("done");
    return RT_OK;
}

/********autolock test pass*******/
#ifdef autolock_test
#define AutoMutex 1
UINT32 pthreadlock = AutoMutex ? 0 : 1;

#if AutoMutex
#define condmutex 0
void* function0_print(void*) {
    if(AutoMutex)
        RtAutoMutex autolock(xlock1);
    if(pthreadlock)
        lock1->lock();
    for(UINT32 i = 0; i < 200 ; i++) {
   // while(1){
    //  RT_LOGE("function0_print start and i %d",i);
        RtAutoMutex autolock(xlock);
        //lock0->lock();
        RT_LOGE("function0_print end and pthreadlock %d",pthreadlock);
        //lock0->unlock();
    }
    if(pthreadlock)
        lock1->unlock();
    return NULL;
}

void* function1_print(void*) {
    if(AutoMutex)
        RtAutoMutex autolock(xlock);
    if(pthreadlock)
        lock0->lock();
    for(UINT32 i = 0; i < 200 ; i++) {
     //  while(1) {
        RtAutoMutex autolock(xlock1);
    //  RT_LOGE("function1_print start and i %d");
   // lock1->lock();
        RT_LOGE("function1_print end and AutoMutex %d",AutoMutex);
   // lock1->unlock();
    }
    if(pthreadlock)
        lock0->unlock();
    return NULL;
}
#else
#define condmutex 1
#endif // AutoMutex

#if condmutex
void* function0_print(void*) {
    UINT32 i = 1;
    while(i++){
        lock1->lock();
        if(i % 5 == 0)
            rtcond->signal();
        else
            RT_LOGE("function0_print end and pthreadlock %d",pthreadlock);
        lock1->unlock();
    }
    return NULL;
}

void* function1_print(void*) {
    UINT32 i = 1;
    while(i++) {
        lock1->lock();
        if(i % 5 != 0) {
            RT_LOGE("function1_print start and i %d",i);
            rtcond->wait(lock1);
        }
        lock1->unlock();
        RT_LOGE("function1_print end and i %d",i);
    }
    return NULL;
}
#endif // condmutex



INT32 unit_test_autolock() {
    RtThread* th0 = new RtThread(function0_print, NULL);
    RtThread* th1 = new RtThread(function1_print, NULL);
    th0->start();
    th1->start();
    th1->join();
    RT_LOGE("success to unit_test_mutex()!th0 %d ,th1 %d",th0->get_tid(),th1->get_tid());
    return RT_OK;
}
#endif


#endif
