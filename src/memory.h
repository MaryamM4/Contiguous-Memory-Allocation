#ifndef MEMORY_H
#define MEMORY_H

#include "mem_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KByte 1024           // bytes
#define MEMSIZE (80 * KByte) // KBytes

typedef char ProcessName;

typedef enum {
  ALLOCATE = 'A',
  FREE_PROCESS_ALLOCATIONS = 'F',
  SHOW_MEM = 'S',
  COMPACT_MEM = 'C',
  EXIT = 'E'
} Command;
typedef enum { FIRST_FIT = 'F', BEST_FIT = 'B', WORST_FIT = 'W' } Algo;

// Global Head for memory list.
extern MemBlock *g_head;
extern MemBlock *waiting_queue;

// Alocate <size> bytes for process <p_name> using algorithm <algo>.
void allocate(ProcessName p_name, int size, Algo algo);

// ALLOCATION METHODS:
void allocateFirstFit(MemBlock *block); // First hole that fits.
void allocateBestFit(MemBlock *block);  // Smallest hole that fits.
void allocateWorstFit(MemBlock *block); // Largest hole that fits.

// @TODO later.
// Since process memory must remain contigous, cannot split allocation.
// Can instead add to a waiting queue, which we will ignore for now.
void addToWaitingQueue(MemBlock *block); // If no hole fits & size <= MEMSIZE.

// Free all the allocations owned by process <p_name>.
void free(ProcessName p_name);

// Show the state of the memory pool.
void show_state();

// Read the script in the file and execute each command.
// <filename> should end with ".txt", eg: "MEMO.TXT".
void read_script(char *filename);

// Compact the memory pool, sliding all allocations to lower
// addresses to they become one contigous block,
// and so all free spaces lie the right as one contigous block.
void compact();

void exit();

#endif // MEMORY_H
