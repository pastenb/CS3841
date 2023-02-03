#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "memory_manager.h"

/*
 * Using static causes the compiler to
 * limit visibility of the variables to this file only
 * This can be used to simulate 'private' variables in c
 */
static int allocation_count;

pthread_mutex_t malloc_mutex;

struct block
{
	void* start;
	int size;
	int type; // 0 is free, 1 is allocated
	struct block* next;
} block;

struct block* top;

/* TODO Define additional structure definitions here */

/* mmInit()
 *     Initialize the memory manager to "manage" the given location
 *     in memory with the specified size.
 *         Parameters: start - the start of the memory to manage
 *                     size - the size of the memory to manage
 *         Returns: void
 */
void mmInit(void* start, int size)
{
	struct block* b = malloc(sizeof(block));
	b->start = start;
	b->size = size;
	b->type = 0;
	b->next = NULL;
	top = b;
	pthread_mutex_init(&malloc_mutex, NULL);
	allocation_count = 0;
}

/* mmDestroy()
 *     Cleans up any storage used by the memory manager
 *     After a call to mmDestroy:
 *         all allocated spaces become invalid
 *         future allocation attempts fail
 *         future frees result in segmentation faults
 *     NOTE: after a call to mmDestroy a call to mmInit
 *           reinitializes the memory manager to allow allocations
 *           and frees
 *         Parameters: None
 *         Returns: void
 */
void mmDestroy()
{
	pthread_mutex_lock(&malloc_mutex);
	pthread_mutex_unlock(&malloc_mutex);
}

/* mymalloc_ff()
 *     Requests a block of memory be allocated using
 *         first fit placement algorithm
 *     The memory manager must be initialized (mmInit)
 *         for this call to succeed
 *         Parameters: nbytes - the number of bytes in the requested memory
 *         Returns: void* - a pointer to the start of the allocated space
 */
void* mymalloc_ff(int nbytes)
{
	pthread_mutex_lock(&malloc_mutex);
	struct block *current = top;
	while (current != NULL) {
		if (current->type == 0 && current->size >= nbytes) {
			// Found free block with enough space
			current->type = 1;
			allocation_count++;
			// Check if we need to split the block
			if(current->size != nbytes) {
				// Splitthe block and create a new free block
				struct block *new_block = malloc(sizeof(block));
				new_block->start = current->start + nbytes;
				new_block->size = current->size - nbytes;
				new_block->type = 0;
				new_block->next = current->next;
				current->next = new_block;
				current->size = nbytes;
			}
			pthread_mutex_unlock(&malloc_mutex);
			return current->start;
		}
		current = current->next;
	}
	pthread_mutex_unlock(&malloc_mutex);
	return NULL;
}

/* mymalloc_wf()
 *     Requests a block of memory be allocated using
 *         worst fit placement algorithm
 *     The memory manager must be initialized (mmInit)
 *         for this call to succeed
 *         Parameters: nbytes - the number of bytes in the requested memory
 *         Returns: void* - a pointer to the start of the allocated space
 */
void* mymalloc_wf(int nbytes)
{
	pthread_mutex_lock(&malloc_mutex);

	struct block* curr = top;
	struct block* largest_block = NULL;
	int largest_block_size = 0;
	// Find the largest free block that can fit nbytes
	if(get_fragment_count() == 1) {
		largest_block = top;
		curr = NULL;
	}

	while (curr != NULL) {
		if (curr->type == 0 && curr->size >= nbytes && curr->size > largest_block_size) {
			largest_block = curr;
			largest_block_size = curr->size;
		}
		curr = curr->next;
	}
	// If there's no free block large enough, return NULL
	if (largest_block == NULL) {
		pthread_mutex_unlock(&malloc_mutex);
		return NULL;
	}
	// Allocate the block
	void* start = largest_block->start;
	int size = largest_block->size;
	largest_block->type = 1;
	allocation_count++;
	// Update the size of the block if there's enough space for another block
	if (size != nbytes) {
		struct block* b = malloc(sizeof(block));
		b->start = start + nbytes;
		b->size = size - nbytes;
		b->type = 0;
		b->next = largest_block->next;
		largest_block->size = nbytes;
		largest_block->next = b;
	}

	pthread_mutex_unlock(&malloc_mutex);
	return start;
}

/* mymalloc_bf()
 *     Requests a block of memory be allocated using
 *         best fit placement algorithm
 *     The memory manager must be initialized (mmInit)
 *         for this call to succeed
 *         Parameters: nbytes - the number of bytes in the requested memory
 *         Returns: void* - a pointer to the start of the allocated space
 */
void* mymalloc_bf(int nbytes)
{
	pthread_mutex_lock(&malloc_mutex);

	struct block* curr = top;
	struct block* smallest_block = NULL;
	int smallest_block_size = 100;
	// Find the largest free block that can fit nbytes
	if(get_fragment_count() == 1) {
		smallest_block = top;
		curr = NULL;
	}

	while (curr != NULL) {
		if (curr->type == 0 && curr->size >= nbytes && curr->size < smallest_block_size) {
			smallest_block = curr;
			smallest_block_size = curr->size;
		}
		curr = curr->next;
	}
	// If there's no free block large enough, return NULL
	if (smallest_block == NULL) {
		pthread_mutex_unlock(&malloc_mutex);
		return NULL;
	}
	// Allocate the block
	void* start = smallest_block->start;
	int size = smallest_block->size;
	smallest_block->type = 1;
	allocation_count++;
	// Update the size of the block if there's enough space for another block
	if (size != nbytes) {
		struct block* b = malloc(sizeof(block));
		b->start = start + nbytes;
		b->size = size - nbytes;
		b->type = 0;
		b->next = smallest_block->next;
		smallest_block->size = nbytes;
		smallest_block->next = b;
	}

	pthread_mutex_unlock(&malloc_mutex);
	return start;
}

/* myfree()
 *     Requests a block of memory be freed and the storage made
 *         available for future allocations
 *     The memory manager must be initialized (mmInit)
 *         for this call to succeed
 *         Parameters: ptr - a pointer to the start of the space to be freed
 *         Returns: void
 *         Signals a SIGSEGV if a free is not valid
 *             - memory manager is not initialized
 *             - memory manager has been destroyed
 *             - ptr is not allocated (e.g. double free)
 */
void myfree(void* ptr)
{
	struct block *b = top;
	while(b != NULL) {
		if (b->start == ptr) {
			if(b->type == 0) {
				raise(SIGSEGV);
			} else {
				b->type = 0;
			}
			condense_free();
			return;
		}
		b = b->next;
	}
	raise(SIGSEGV);
}

void condense_free() {
	struct block *curr = top;

	while(curr != NULL && curr->next != NULL) {
		if(curr->type == 0 && curr->next->type == 0) {
			curr->size += curr->next->size;
			curr->next = curr->next->next;
			continue;
		}

		curr = curr->next;
	}
}

/* get_allocated_space()
 *     Retrieve the current amount of space allocated by the memory manager (in bytes)
 *         Parameters: None
 *         Returns: int - the current number of allocated bytes
 */
int get_allocated_space()
{
	struct block* b = top;
	int allocatedSpace = 0;
	while (b != NULL)
	{
		if(b->type == 1) {
			allocatedSpace += b->size;
		}
		b = b->next;
	}
	return allocatedSpace;
}

/* get_remaining_space()
 *     Retrieve the current amount of available space in the memory manager (in bytes)
 *         (e.g. sum of all free blocks)
 *         Parameters: None
 *         Returns: int - the current number of free bytes
 */
int get_remaining_space()
{
	struct block* b = top;
	int freeSpace = 0;
	while (b != NULL)
	{
		if (b->type == 0)
		{
			freeSpace += b->size;
		}
		b = b->next;
	}
	return freeSpace;
}

/* get_fragment_count()
 *     Retrieve the current amount of free blocks (i.e. the count of all the block, not the size)
 *         Parameters: None
 *         Returns: int - the current number of free blocks
 */
int get_fragment_count()
{
	int count = 0;
	struct block* c = top;
	while(c != NULL) {
		count++;
		c = c->next;
	}
	return count;
}

/* get_mymalloc_count()
 *     Retrieve the number of successful malloc calls (for all placement types)
 *         Parameters: None
 *         Returns: int - the total number of successful mallocs
 */
int get_mymalloc_count()
{
	return allocation_count;
}
