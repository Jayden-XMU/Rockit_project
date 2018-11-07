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
 *   date: 20180704
 *    ref: https://yq.aliyun.com/articles/31977
 *    ref: https://zhuanlan.zhihu.com/p/38176759
 */

#ifndef __RT_DEQUEUE_H__
#define __RT_DEQUEUE_H__

#define ENTRY_FLAG_USE   1
#define ENTRY_FLAG_UNUSE 0

#include "rt_header.h"

 typedef struct rt_deque_entry {
    void                   *data;
    struct rt_deque_entry  *prev;
    struct rt_deque_entry  *next;
    UINT8                   flag;
} RT_DequeEntry;

typedef struct rt_deque {
    UINT8             size;
    UINT8             max_size;
    RT_DequeEntry     *head;
    RT_DequeEntry     *tail;
    RT_DequeEntry     *entries;
} RT_Deque;

RT_Deque*      deque_create();
RT_Deque*      deque_create(UINT8 max_size);
void           deque_destory(RT_Deque *list);
RT_DequeEntry* deque_entry_malloc(RT_Deque *list);
RT_DequeEntry* deque_pop(RT_Deque *list);
void*          deque_get(RT_Deque *list, int index);
INT8           deque_push(RT_Deque *list, const void *data, RT_BOOL header=RT_FALSE);
INT8           deque_push_tail(RT_Deque *list, const void *data);
INT8           deque_push_head(RT_Deque *list, const void *data);

#define deque_size(list) ((list)->size)
#define deque_head(list) ((list)->head)
#define deque_tail(list) ((list)->tail)
#define deque_is_head(element) ((element)->prev == NULL ? 1 : 0)
#define deque_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define deque_data(element) ((element)->data)
#define deque_next(element) ((element)->next)
#define deque_prev(element) ((element)->prev)

#endif
