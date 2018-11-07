#include "rt_dequeue.h"
#include "rt_header.h"
#include "rt_mem.h"

RT_Deque* deque_create() {
    RT_Deque* list = (RT_Deque*)rt_mem_malloc(__FUNCTION__, sizeof(RT_Deque));
    if(RT_NULL != list) {
        list->size = 0;
        list->head = RT_NULL;
        list->tail = RT_NULL;
    }
    list->entries   = RT_NULL;
    list->max_size = 0;
    return list;
}

RT_Deque* deque_create(UINT8 max_size) {
    RT_Deque* list  = deque_create();
    RT_ASSERT(RT_NULL != list);
    list->max_size = max_size;
    list->entries   = rt_malloc_array(RT_DequeEntry, max_size);
    RT_ASSERT(RT_NULL != list->entries);
    for(int idx = 0; idx < max_size; idx++) {
        RT_DequeEntry entry = list->entries[idx];
        rt_memset(&entry, RT_NULL, sizeof(RT_DequeEntry));
    }
    return list;
}

void deque_destory(RT_Deque *list) {
    RT_ASSERT(RT_NULL != list);

    RT_DequeEntry* entry = list->head;
    RT_DequeEntry* next;
    while(entry){
        next = entry->next;
        rt_free(entry);
        entry = next;
    }
    rt_memset(list, 0, sizeof(RT_Deque));
}

RT_DequeEntry* deque_entry_malloc(RT_Deque *list) {
    RT_ASSERT(RT_NULL != list);
    RT_DequeEntry *entry = RT_NULL;
    if( RT_NULL != list->entries ) {
        if(list->size < list->max_size) {
            for(int idx = 0; idx < list->max_size; idx++) {
                entry = &(list->entries[idx]);
                // found entry in unused pre-malloc entries
                if((RT_NULL == entry->data)||(ENTRY_FLAG_UNUSE == entry->flag)) {
                    break;
                }
            }
        } else {
            entry = RT_NULL;
        }
    } else {
        entry = rt_malloc(RT_DequeEntry);
    }
    return entry;
}

RT_DequeEntry* deque_pop(RT_Deque *list) {
    RT_ASSERT(RT_NULL != list);
    RT_DequeEntry *entry = RT_NULL;
    if (deque_size(list) > 0) {
        entry      = list->head;
        list->head = entry->next;
        list->size = (list->size>0)?(list->size-1):0;
    }
    return entry;
}

INT8  deque_push(RT_Deque *list, const void *data, RT_BOOL header/*=RT_FALSE*/) {
    RT_ASSERT(RT_NULL != list);

    RT_DequeEntry *entry = deque_entry_malloc(list);
    if (RT_NULL == entry)
       return RT_ERR_BAD;

    entry->data = (void *)data;
    entry->flag = ENTRY_FLAG_USE;
    entry->next = RT_NULL;
    if (deque_size(list) == 0) {
        // insert header, when list is RT_NULL
        entry->prev = RT_NULL;
        list->head = entry;
        list->tail = entry;
    } else {
        if(RT_TRUE==header){
            RT_DequeEntry* head = list->head;
            head->prev         = entry;
            entry->next        = head;
            list->head         = entry;
        }else{
            RT_DequeEntry* tail = list->tail;
            tail->next         = entry;
            entry->prev        = tail;
            list->tail         = entry;
        }
    }
    list->size++;
    return RT_OK;
}

INT8  deque_push_tail(RT_Deque *list, const void *data) {
    RT_BOOL header =  RT_FALSE;
    return deque_push(list, data, header);
}

INT8  deque_push_head(RT_Deque *list, const void *data) {
    RT_BOOL header =  RT_TRUE;
    return deque_push(list, data, header);
}

void*  deque_get(RT_Deque *list, int index) {
    RT_DequeEntry* entry = list->head;
    while(RT_NULL != entry){
        if(index==0) break;
        index--;
        entry = entry->next;
    }
    if(RT_NULL == entry) {
        return RT_NULL;
    }
    return entry->data;
}
