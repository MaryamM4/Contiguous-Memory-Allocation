#include "memory.h"

MemBlock *g_head = NULL;
MemBlock *waiting_queue = NULL;

// Alocate <size> bytes for process <p_name> using algorithm <algo>.
void allocate(ProcessName p_name, int size, Algo algo) {
  if (size > MEMSIZE) {
    printf("allocate ERROR: Task too large. May not exceed %d.\n", MEMSIZE);
    return;
  }

  MemBlock *block = initBlock(p_name, size);
  if (!block) {
    printf("allocate ERROR: Failed to initBlock.\n");
    return;
  }

  if (g_head == NULL) {
    g_head = block;

  } else {
    switch (algo) {
    case FIRST_FIT:
      allocateFirstFit(block);
      break;

    case BEST_FIT:
      allocateBestFit(block);
      break;

    case WORST_FIT:
      allocateWorstFit(block);
      break;

    default:
      printf("allocate ERROR: Invalid algo %s\n", algo);
    }
  }
}

// First hole that fits.
void allocateFirstFit(MemBlock *block) {
  MemBlock *it = g_head;

  while (it != NULL) {
    if (it->owner == HOLE && it->size >= block->size) {
      fitIntoHole(it, block);
      return;
    }
    it = it->next;
  }

  // Reaching here means no space was found.
  addToWaitingQueue(block);
}

// Smallest hole that fits.
void allocateBestFit(MemBlock *block) {
  MemBlock *it = g_head;
  MemBlock *smallest_hole = NULL;

  while (it != NULL) {
    if (it->owner == HOLE && it->size >= block->size) { // Valid hole.
      if (it->size == block->size) {                    // Perfect fit.
        it->owner = block->owner;
        free(block);
        return;

      } else if (smallest_hole == NULL || it->size < smallest_hole->size) {
        smallest_hole = it; // Current smallest hole.
      }
    }

    it = it->next; // Loop iteration.
  }

  // Reaching here means no perfect fit. Settle for best.
  if (smallest_hole) {
    fitIntoHole(smallest_hole, block);

  } else { // No fit found.
    addToWaitingQueue(block);
  }
}

// Largest hole that fits.
void allocateWorstFit(MemBlock *block) {
  MemBlock *it = g_head;
  MemBlock *largest_hole = NULL;

  while (it != NULL) {
    if (it->owner == HOLE && it->size >= block->size) { // Valid hole.
      if (largest_hole == NULL || it->size > largest_hole->size) {
        largest_hole = it; // Current (first) largest hole.
      }
    }

    it = it->next;
  }

  if (largest_hole) {
    fitIntoHole(largest_hole, block);
  } else { // No fit found.
    addToWaitingQueue(block);
  }
}

// @TODO handling later.
// Since process memory must remain contigous, cannot split allocation.
// Can instead add to a waiting queue, which we will ignore for now.
void addToWaitingQueue(MemBlock *block) {
  // printf("No holes large enough for %s (size %d). Moving to waiting
  // queue.\n",
  //        block->owner, block->size);

  if (waiting_queue == NULL) {
    waiting_queue = block;
  } else {
    insertEnd(waiting_queue, block);
  }
}

// Free all the allocations owned by process <p_name>.
void free(ProcessName p_name) {
  MemBlock *it = g_head;

  while (it != NULL) {
    if (it->owner == p_name) {
      convertToHole(it);
    }
    it = it->next;
  }
}

// Show the state of the memory pool.
void show_state() {
  MemBlock *it = g_head;

  while (it != NULL) {
    if (it->owner == HOLE) {
      for (int i = 0; i < it->size; i++) {
        printf(".");
      }

    } else {
      printf("%c", it->owner);
    }

    it = it->next;
  }
  printf("\n");
}

// Read the script in the file and execute each command.
// <filename> should end with ".txt", eg: "MEMO.TXT".
void read(char *filename);

// Compact the memory pool, sliding all allocations to lower
// addresses to they become one contigous block,
// and so all free spaces lie the right as one contigous block.
void compact() {
  MemBlock *it = g_head;
  MemBlock *nonhole_tail;

  while (it != NULL) {
    nonhole_tail = it;

    while (it->owner == HOLE) {
    }

    it = it->next;
  }
}

void exit() {
  freeList(g_head);
  freeList(waiting_queue);
}