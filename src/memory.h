#ifndef MEMORY_H
#define MEMORY_H

typedef char ProcessName;

typedef enum { FIRST_FIT = 'F', BEST_FIT = 'B', WORST_FIT = 'W' } Algo;

// Alocate <size> bytes for process <p_name> using algorithm <algo>.
void allocate(ProcessName p_name, int size, Algo algo);

// Free all the allocations owned by process <p_name>.
void free(ProcessName p_name);

// Show the state of the memory pool.
void show_state();

// Read the script in the file and execute each command.
// <filename> should end with ".txt", eg: "MEMO.TXT".
void read(char *filename);

// Compact the memory pool, sliding all allocations to lower
// addresses to they become one contigous block,
// and so all free spaces lie the right as one contigous block.
void compact();

void exit();

#endif // MEMORY_H
