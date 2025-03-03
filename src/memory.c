#include "memory.h"

MemBlock *g_head = NULL;
MemBlock *waiting_queue = NULL;

void init() { g_head = initHole(MEMSIZE); }

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
      printf("allocate ERROR: Invalid algo %c\n", algo);
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
void free_process_allocations(ProcessName p_name) {
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
  char owner;

  while (it != NULL) {
    owner = (it->owner == HOLE) ? '.' : it->owner;

    for (int i = 0; i < it->size; i++) {
      printf("%c", owner);
    }

    it = it->next;
  }
  printf("\n");
}

// Read the script in the file and execute each command.
// <filename> should end with ".txt", eg: "MEMO.TXT".
void read_script(char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("read ERROR: Failed to open file %s.\n", filename);
    return;
  }

  char command;
  char line[10]; // A line shouldn't be > 8

  while (fgets(line, sizeof(line), file)) {
    if (line[0] == '\n' || line[0] == '\r') {
      continue; // Skip if empty..
    }

    if (sscanf(line, " %c", &command) != 1) {
      continue;
    }

    switch (command) {
    case 'A': {
      char p_name, p_algo;
      int count, p_size;

      count = sscanf(line, " %*c %c %d %c", &p_name, &p_size, &p_algo);
      if (count < 3) {
        fprintf(stderr,
                "read_script() Error parsing parameters for command A in line: "
                "%s\n",
                line);
        break;
      }

      allocate(p_name, p_size, p_algo);
      break;
    }

    case 'F': {
      char p_name;
      int count = sscanf(line, " %*c %c", &p_name);

      if (count < 1) {
        fprintf(stderr,
                "read() Error parsing parameters for command A in line: %s\n",
                line);
        break;
      }

      free_process_allocations(p_name);
      break;
    }

    case 'S':
      show_state();
      break;

    case 'C':
      compact();
      break;

    case 'E':
      exit_mem();
      break;

    default:
      fprintf(
          stderr,
          "read_script() Error parsing parameters for command A in line: %s\n",
          line);
      break;
    }

    fprintf(stderr, "Current line: %s", line); // Remove me
    printf("Current state: ");
    show_state();
    printf("\n");
  }

  fclose(file);
}

// Compact the memory pool, sliding all allocations to lower
// addresses to they become one contigous block,
// and so all free spaces lie the right as one contigous block.
void compact() {
  MemBlock *it = g_head;
  MemBlock *prev = NULL;
  int hole_size = 0;

  while (it != NULL) {
    if (it->owner == HOLE) {
      hole_size += it->size; // Remember for end.

      if (prev) {
        prev->next = it->next;

      } else {
        // Handle first node being a hole.
        g_head = it->next;
      }

      // Delete hole.
      MemBlock *to_del = it;
      it = it->next;
      free(to_del);

    } else { // Not a hole. Normal iteration.
      prev = it;
      it = it->next;
    }
  }

  // Add a ode to store hole at end.
  if (prev) {
    prev->next = initHole(hole_size);
  } else {
    g_head = initHole(hole_size);
  }
}

void exit_mem() {
  freeList(g_head);
  freeList(waiting_queue);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }

  init();
  read_script(argv[1]);

  return EXIT_SUCCESS;
}
