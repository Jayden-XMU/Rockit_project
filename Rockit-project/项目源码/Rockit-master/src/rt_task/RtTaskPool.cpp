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
 *    ref: http://www.cnblogs.com/venow/archive/2012/11/22/2779667.html
 *    ref: https://github.com/media-tm/MediaNode/blob/master/src/MTCommon/MTSystem/SkThreadPool.h
 */

#include "rt_common.h"

#include "rt_cpu_info.h"
#include "rt_mem.h"
#include "rt_mutex.h"

#include "rt_thread.h"
#include "rt_task.h"
#include "rt_taskpool.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "RtTaskPool"

RtTaskPool* rt_taskpool_init(UINT32 max_thread_num, UINT32 max_task_num) {
    RtTaskPool* taskpool = rt_malloc(RtTaskPool);
    RT_ASSERT(RT_NULL != taskpool);

    max_thread_num = (max_thread_num==0)?rt_cpu_count():max_thread_num;
    taskpool->cur_task_num    = 0;
    taskpool->busy_task_num   = 0;
    taskpool->max_thread_num  = max_thread_num;
    taskpool->max_task_num    = max_task_num;
    taskpool->state           = kRunning_State;
    taskpool->task_lock       = new RtMutex();

    RT_LOGT("Create Taskpool(max_thread_num=%d, max_task_num=%d)",
                    max_thread_num,max_task_num);

    // create max_thread_num threads, all running rt_Ftaskpool_loop.
    taskpool->tasks    = deque_create();
    taskpool->pthreads = deque_create();
    for(UINT32 idx = 0; idx < taskpool->max_thread_num; idx++) {
        RtThread* thread = new RtThread(rt_taskpool_loop, (void*)taskpool);
        thread->mIndex = idx;
        deque_push_tail(taskpool->pthreads, (void*)thread);
        thread->start();
    }
    return taskpool;
}

INT8 rt_taskpool_push(RtTaskPool *taskpool, RtTask *task, RT_BOOL header/*=RT_FALSE*/) {
    RT_ASSERT(RT_NULL != taskpool);

    if(kRunning_State != taskpool->state) {
        return RT_ERR_BAD;
    }

    while ((taskpool->cur_task_num == taskpool->max_task_num)) {
        //RT_LOGT("Task Pool is Full...");
        RtTime::sleepUs(5000);
    }

    taskpool->task_lock->lock();
    INT8 err = RT_ERR_BAD;
    err = (RT_TRUE == header)? deque_push_head(taskpool->tasks, (void*)task): \
                              deque_push_tail(taskpool->tasks, (void*)task);
    if(RT_OK == err) {
        taskpool->cur_task_num++;
    }
    taskpool->task_lock->unlock();
    RT_LOGE("Task(%p,id:%02d/busy:%02d/wait:%02d/max:%02d) be pushed to TaskPool",
                 task, task->get_id(), taskpool->busy_task_num,
                 taskpool->cur_task_num, taskpool->max_task_num);
    return err;
}

INT8 rt_taskpool_push_head(RtTaskPool *taskpool, RtTask *task) {
    RT_BOOL header = RT_TRUE;
    return rt_taskpool_push(taskpool, task, header);
}

INT8 rt_taskpool_push_tail(RtTaskPool *taskpool, RtTask *task) {
    RT_BOOL header = RT_FALSE;
    return rt_taskpool_push(taskpool, task, header);
}

void rt_taskpool_pause( RtTaskPool *taskpool) {
    taskpool->state = kPausing_State;
}

void rt_taskpool_resume(RtTaskPool *taskpool) {
    taskpool->state = kRunning_State;
}

void rt_taskpool_wait(  RtTaskPool *taskpool) {
    RT_Deque* list   = taskpool->pthreads;
    taskpool->state = kWaiting_State;
    for(int idx = 0; idx < list->size; idx++) {
        RtThread* thread = (RtThread*)deque_get(list, idx);
        thread->join();
        RT_LOGT("Thread[%p %02d/%02d] joined, then delete", thread, idx+1, list->size);
        delete thread;
    }
    deque_destory(list);
}

void rt_taskpool_dump(  RtTaskPool *taskpool) {
    RT_LOGT("----------------------");
}

void* rt_taskpool_loop(void* args) {
    // The RtTaskPool passes itself as args to each thread as they're created.
    RtTaskPool *taskpool = static_cast<RtTaskPool*>(args);
    RT_ASSERT(RT_NULL != taskpool); // Unreachable.
    INT32 tid = RtThread::get_tid();

    while (true) {
        if ((kWaiting_State == taskpool->state)&&(taskpool->cur_task_num==0)) {
            // no task and taskpool is in waiting state
            return NULL;
        }
        if ((kRunning_State == taskpool->state)&&(taskpool->cur_task_num==0)) {
            RtTime::sleepUs(5000);
            RT_LOGT("Task Pool is Empty...cur_task_num=%d", taskpool->cur_task_num);
            continue;
        }

        // We have to be holding the lock to read the queue and to call wait.
        taskpool->task_lock->lock();
        RtTask*       task  = RT_NULL;
        RT_DequeEntry* entry = deque_pop(taskpool->tasks);
        if(RT_NULL != entry) {
            task = (RtTask*)entry->data;
            entry->data  = RT_NULL;
            entry->flag  = ENTRY_FLAG_UNUSE;
            taskpool->cur_task_num--;
        } else {
            RtTime::sleepUs(5000);
            RT_LOGT("Task Pool is Empty...cur_task_num=%d", taskpool->cur_task_num);
            taskpool->task_lock->unlock();
            continue;
        }

        taskpool->busy_task_num++;
        taskpool->task_lock->unlock();

        // OK, now really do the work.
        UINT64 now = RtTime::getNowTimeMs();
        task->run(RT_NULL);

        UINT64 duration = RtTime::getNowTimeMs() - now;
        RT_LOGE("Task(%p,id:%02d/busy:%02d/wait:%02d/max:%02d) spent %lldms on Thread[%d]",
                 task, task->get_id(),taskpool->busy_task_num,
                 taskpool->cur_task_num, taskpool->max_task_num, duration, tid);

        //taskpool->task_lock->lock();
        taskpool->busy_task_num--;
        //taskpool->task_lock->unlock();

        delete task;
    }

    RT_ASSERT(RTFalse); // Unreachable.
}
