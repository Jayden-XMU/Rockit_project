#include "rt_mem_service.h"
#include "rt_os_mem.h"
#include "rt_mem.h"
#include "rt_log.h"
#include "rt_kernel_list.h"
#include <string.h>
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "rt_mem_service"

#define MEM_NODE_MAX            (1024)

rt_mem_service::rt_mem_service() {
    nodes_max = MEM_NODE_MAX;
    INT32 size = sizeof(MemNode)*nodes_max;
    rt_os_malloc((void **)&mem_nodes, MEM_ALIGN, size);
    RT_ASSERT(RT_NULL != mem_nodes);

    rt_memset(mem_nodes, 0, sizeof(MemNode)*nodes_max);
    add_node(__FUNCTION__, mem_nodes, size);
    add_node(__FUNCTION__, this, sizeof(rt_mem_service));
}

rt_mem_service::~rt_mem_service(){
}

void rt_mem_service::add_node(const char *caller, void* ptr, UINT32 size) {
    RT_ASSERT(RT_NULL != mem_nodes);

    MemNode *node = mem_nodes;
    for (UINT32 i = 0; i < nodes_max; i++, node++) {
        if(node->size == 0) {
            node->caller = caller;
            node->ptr    = ptr;
            node->size   = size;
            nodes_cnt++;
            total_size += size;
            break;
        }
    }
}

void rt_mem_service::remove_node(void* ptr, size_t *size) {
    RT_ASSERT(RT_NULL != mem_nodes);

    MemNode *node = mem_nodes;
    for (UINT32 i = 0; i < nodes_max; i++, node++) {
        if((node->size > 0)&&(node->ptr == ptr)) {
            *size = node->size;
            nodes_cnt--;
            total_size -= node->size;
            node->size = 0;
            break ;
        }
    }
}

void rt_mem_service::reset(){
    RT_ASSERT(RT_NULL != mem_nodes);

    MemNode *node = mem_nodes;
    for (UINT32 i = 0; i < nodes_max; i++, node++) {
       node->size   = 0;
       node->ptr    = RT_NULL;
       node->caller = RT_NULL;
    }
    nodes_cnt  = 0;
    total_size = 0;
}

void rt_mem_service::dump(){
    RT_ASSERT(RT_NULL != mem_nodes);

    RT_LOGE("======= Rockit Memory Summary =======");
    MemNode *node = mem_nodes;
    RT_LOGE("Memory Tatal:%dk, nodes:%d/%d", total_size/1024, nodes_cnt, nodes_max);
    for (UINT32 i = 0; i < nodes_max; i++, node++) {
        if( node->size > 0) {
            RT_LOGE("Memory Node:No is %d,Ptr:%p; Size=%04d; caller=%s",i,
                     node->ptr, node->size, node->caller);
        }
    }
}

INT32 rt_mem_service::find_node(const char *caller, void* ptr, UINT32*size) {
    UINT32 result = 100;
    MemNode *node = mem_nodes;
    if(node->size  == 0) RT_LOGE("the node you search is NULL!");

    for(UINT32 i = 0; i < nodes_max; i++, node++) {
        if(node->ptr == ptr) {
            result = i ;
            RT_LOGE("find the node ,and the node_No is %d",result);
            return result;
        }
    }
    RT_LOGE("the node you search doesn't exist!");
    return -1 ;
}
/*
typedef struct RtMemSnapshotImpl {
    struct list_head    list;
    UINT64              total_size;
    UINT64              total_count;
} RtMemSnapshotImpl;

static pthread_mutex_t mem_list_lock;
static struct list_head mem_list;

INT8  rt_mem_get_snapshot(RtMemSnapshotImpl *hnd) {

    MemNode *pos ,*n;

    RtMemSnapshotImpl *p = (RtMemSnapshotImpl *)malloc(sizeof(RtMemSnapshotImpl));
    INIT_LIST_HEAD(&p->list);
    p->total_size = 0;
    p->total_count = 0;

    pthread_mutex_lock(&mem_list_lock);
    list_for_each_entry_safe(pos, n, &mem_list, list) {
        MemNode *node = ( MemNode *)malloc(sizeof(MemNode));
        RT_ASSERT_RESULT(node);

        memcpy(node, pos, sizeof(*pos));
        p->total_size += pos->size;
        p->total_count++;
    }
    *hnd = p;
    pthread_mutex_unlock(&mem_list_lock);
    return RT_OK;
}
*/
