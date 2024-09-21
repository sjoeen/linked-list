#ifndef _GC_H_
#define _GC_H_


// Allocates a contiguous piece of garbage-collected memory of a certain size
void *gc_malloc(size_t size);

// Registers use of a certain piece of memory. IE takes a pointer to an
// already-allocated piece of memory and increments the reference counter for
// this memory.
void gc_register(void *ptr);

// Unregisters use of the memory pointed to by ptr. If the reference counter is
// zero at this point the memory can be safely freed, otherwise it must be kept
// intact.
void gc_free(void *ptr);

#endif /*GC_H*/
