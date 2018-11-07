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
 *   date: 2018/07/05
 */

#ifndef __TEST_BASE_REBUST_H__
#define __TEST_BASE_REBUST_H__

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "test_base_rebust"

#include "rt_header.h"
#include "rt_mem_service.h"
//#define memory_test 1 // rt_mem_malloc test 1; rt_mem_calloc test 2 ; rt_mem_realloc 3
#define add_node_test 1
#define remove_node_test 0
#define find_node_test 1
typedef struct _person{
    int   age;
    char *name;
} Person;

INT8 unit_test_memery() {
    UINT32 idx = 0;
    UINT32 memory_test  = 1;//switch for mem_test case here
    rt_mem_record_reset();
    for(idx = 0; idx < 10; idx++) {
        Person* martin = RT_NULL;
        if(memory_test  == 1)
            martin = (Person*)rt_mem_malloc(__FUNCTION__, sizeof(Person)); //test pass
        if(memory_test  == 2)
            martin = (Person*)rt_mem_calloc(__FUNCTION__, sizeof(Person));//test pass
        if(memory_test  == 3)
            martin = (Person*)rt_mem_realloc(__FUNCTION__, martin, sizeof(Person));//test pass

        rt_memset(martin,0,sizeof(martin));//test pass
        if(RT_NULL != martin){
            martin->age  = idx;
            martin->name = (char*)"martin";
            rt_mem_free(__FUNCTION__, martin);
            martin = RT_NULL;
            rt_mem_record_dump();
        }else{
            break;
        }
    }
    if(idx == 10) {
        RT_LOGT("Success to unit_test_memery()");
        return RT_OK;
    }else{
        RT_LOGE("Fail to unit_test_memery()");
    }
    return RT_ERR_MALLOC;
}
INT8 unit_test_node(){
    RT_LOGE("unit_test_node() function enter....");
    UINT32 idx ;
    rt_mem_service * mem_record = new rt_mem_service();
    mem_record->reset();

    if(add_node_test){
        for(idx = 0; idx < 10; idx++) {
            Person* prince = RT_NULL;
            prince = (Person*)rt_mem_malloc(__FUNCTION__, sizeof(Person));
            prince->age  = idx;
            prince->name = (char*)"JAY";
            mem_record->add_node(__FUNCTION__,prince,sizeof(prince));
        }
        mem_record->dump();
        RT_LOGE("now mem_record node num is %d", mem_record->nodes_cnt);
    }
    MemNode * node = mem_record->mem_nodes;
    MemNode * mnode = node ;
   // RT_LOGE("now0 node is %p, mnode is %p", node->ptr, mnode->ptr);
    UINT32 * mnode_size;
    if(remove_node_test) {
        while(mem_record->nodes_cnt > 0) {
            RT_LOGE("now1 node is %p, mnode is %p,nodes_cnt is %d",node->ptr, mnode->ptr, mem_record->nodes_cnt);
            mem_record->remove_node(mnode->ptr, mnode_size);//(mnode->ptr, &mnode_size);
            node++;
            mnode++;
        }
        mem_record->dump();
        RT_LOGE("now mem_record node num1 is %d", mem_record->nodes_cnt);
        return RT_OK;
    }
    else {
        mnode ++;
        UINT32 score = mem_record->find_node(__FUNCTION__, mnode->ptr, mnode_size);
        RT_LOGE("find the node ,and the NO is %d", score);
        return RT_OK;
    }
}
#endif //__TEST_BASE_REBUST_H__
