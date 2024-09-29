#include <assert.h>
#include <stdlib.h>
#include "gc.h"

/*
 *      This reference counting garbage collector works by simply storing a
 *      reference count next to the allocated memory in the following way:
 *
 *          [reference count][allocated memory]
 *
 *      When it gets a call to gc_malloc it simply allocates the give size plus
 *      sizeof(int), and whenever it gets a call to gc_register or gc_free it
 *      shifts the address over by the size of an int to inspect the reference
 *      count.
 *
 */

// Allocates a contiguous piece of garbage-collected memory of a certain size
void *gc_malloc(size_t size) {
    void *allocated = malloc(size + sizeof(int));

    *(int *) allocated = 1;

    return allocated + sizeof(int);
}

// Registers use of a certain piece of memory. IE takes a pointer to an
// already-allocated piece of memory and increments the reference counter for
// this memory.
void gc_register(void *address) {
    address = address - sizeof(int);
    int *reference_count = (int *) address;
    assert(*reference_count > 0);

    (*reference_count)++;
}

// Unregisters use of the memory pointed to by ptr. If the reference counter is
// zero at this point the memory can be safely freed, otherwise it must be kept
// intact. 

typedef struct gc_object {
    /*
    Chatgpt implemented this structure and the comments:
    https://chatgpt.com/share/66f67229-5dfc-800d-839a-613eefefb883
    */
    int ref_count; // The reference count
    void *data;    // The actual data stored in the object
} gc_object;


void gc_free(void *address) {
    /*
    chatgpt made this function and all the comments:
    https://chatgpt.com/share/66f68252-d11c-800d-b6af-b6f4f1507144
    */
    if (address == NULL) return; // Safety check
    
    gc_object *obj = (gc_object *)address;

    // Decrease the reference count
    obj->ref_count--;

    // If the reference count reaches zero, free the memory
    if (obj->ref_count == 0) {
        // Free the data held in the object
        if (obj->data != NULL) {
            free(obj->data);
            obj->data = NULL; // Prevent dangling pointers
        }

        // Free the gc_object itself
        free(obj);
        obj = NULL; // Prevent dangling pointers
    }
}

