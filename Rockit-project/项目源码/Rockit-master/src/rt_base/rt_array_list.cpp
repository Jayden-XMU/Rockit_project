#include "rt_array_list.h"
#include "rt_error.h"
#include "rt_mem.h"

#define INITIAL_CAPACITY 16

RtArrayList* array_list_create() {
    return array_list_create_with_capacity(INITIAL_CAPACITY);
}

RtArrayList* array_list_create_with_capacity(UINT32 min_capacity) {
    RtArrayList* alist = (RtArrayList*)rt_mem_malloc(__FUNCTION__, sizeof(RtArrayList));
    if(RT_NULL == alist) {
        return RT_NULL;
    }
    if(min_capacity <= INITIAL_CAPACITY/2) {
        min_capacity = INITIAL_CAPACITY;
    }
    alist->size         = 0;
    alist->capacity     = min_capacity;
    alist->min_capacity = min_capacity;
    alist->entries      = (RtArrayListEntry*)rt_mem_malloc(__FUNCTION__, sizeof(RtArrayListEntry)*alist->capacity);
    if(RT_NULL == alist->entries) {
        rt_mem_free(__FUNCTION__, alist);
        alist = RT_NULL;
        return RT_NULL;
    }
    return alist;
}

INT8 array_list_add(RtArrayList* self, void* entry) {
    return array_list_insert_at(self, self->size, entry);
}

INT8 array_list_insert_at(RtArrayList* self, UINT32 index, void* element) {
    UINT32 ii = 0;

    // check out of range
    if((index < 0)||(index>self->size)) {
        return RT_ERR_OUTOF_RANGE;
    }

    // increase capacity
    if((self->size+1) >= self->capacity) {
        // shrink array memory
        UINT32 new_capacity = self->capacity*2;
        self->entries = (RtArrayListEntry*)rt_mem_realloc(__FUNCTION__,
                                        self->entries, sizeof(RtArrayListEntry)*new_capacity);
        if(RT_NULL == self->entries) {
            self->size     = 0;
            self->capacity = INITIAL_CAPACITY;
            return RT_ERR_NOMEM;
        } else {
            self->capacity = new_capacity;
        }
    }

    for (ii = self->size; ii > index; ii--) {
        self->entries[ii].data = self->entries[ii - 1].data;
        self->entries[ii].heat = self->entries[ii - 1].heat;
    }
    self->entries[index].data = element;
    self->entries[index].heat = 0;
    self->size++;

    return RT_OK;
}
INT8 array_list_remove(RtArrayList* self, void* element) {
    for(UINT32 ii = 0; ii < self->size; ii++) {
        if(self->entries[ii].data == element) {
            return array_list_remove_at(self, ii);
        }
    }
    return RT_ERR_OUTOF_RANGE;
}

INT8 array_list_remove_at(RtArrayList* self, UINT32 index) {
    for(UINT32 ii = index; ii < self->size; ii++) {
       self->entries[ii].data = self->entries[ii+1].data;
       self->entries[ii].heat = self->entries[ii+1].heat;
    }
    if(self->size>0){
       self->entries[self->size-1].data = NULL;
       self->entries[self->size-1].heat = 0;
       self->size--;
    }

    if (self->capacity > self->min_capacity && self->size <= self->capacity / 4) {
        /* shrink array memory */
        UINT32 new_capacity = self->capacity/2;
        self->entries = (RtArrayListEntry*)rt_mem_realloc(__FUNCTION__,
                                        self->entries, sizeof(RtArrayListEntry)*new_capacity);
        if(RT_NULL == self->entries) {
            self->size     = 0;
            self->capacity = INITIAL_CAPACITY;
            return RT_ERR_NOMEM;
        } else {
            self->capacity = new_capacity;
        }
    }
    return RT_OK;
}
INT8 array_list_remove_all(RtArrayList* self) {
    self->size     = 0;

    /* shrink array memory to min_capacity*/
    self->entries  = (RtArrayListEntry*)rt_mem_realloc(__FUNCTION__,
                                        self->entries, sizeof(RtArrayListEntry)*self->min_capacity);
    self->capacity = self->min_capacity;
    return RT_OK;
}
RT_BOOL array_list_contains(RtArrayList* self, void* element) {
    for(UINT32 ii = 0; ii < self->size; ii++) {
        if(self->entries[ii].data == element) {
            return RT_TRUE;
        }
    }
    return RT_FALSE;
}
UINT32 array_list_get_size(RtArrayList* self) {
    return self->size;
}
void* array_list_get(RtArrayList* self, UINT32 index) {
    if(index < self->size) {
        return self->entries[index].data;
    }
    return RT_NULL;
}
INT8 array_list_set(RtArrayList* self, UINT32 index, void* element) {
    if(index < self->size) {
        self->entries[index].data = element;
        return RT_OK;
    }
    return RT_ERR_OUTOF_RANGE;
}

INT8 array_list_destroy(RtArrayList* self) {
    rt_mem_free(__FUNCTION__, self->entries);
    self->entries = RT_NULL;
    rt_mem_free(__FUNCTION__, self);
    self = RT_NULL;
    return RT_OK;
}
