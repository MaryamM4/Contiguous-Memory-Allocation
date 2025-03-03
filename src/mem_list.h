#ifndef MEMLIST_H
#define MEMLIST_H

#include <stdbool.h>
#include <stdio.h>  // printf
#include <stdlib.h> // malloc

#define HOLE '\0'

typedef struct {
  char owner;
  int size;

  MemBlock *next;
} MemBlock;

// ==============
// Node Functions
// --------------

MemBlock *initBlock(char owner, int size);
MemBlock *initHole(int size);

// Set owner to HOLE.
// If MemBlock after is a hole, merge the two.
void convertToHole(MemBlock *block);

// ==============
// List Functions
// --------------

void insertEnd(MemBlock *head, MemBlock *newBlock);

void insertAfter(MemBlock *prevBlock, MemBlock *newBlock);

// Only inserts new memory block after given previous block if
// the next block exists, is free, and has a size >= newBlock.
// If insert is succesful, will subtract from size of original nextBlock.
bool fitAfter(MemBlock *prevBlock, MemBlock *newBlock);

// "Inserts" new memory block "above" the hole if
// the hole has enough space to fit the new block into.
// If they're the same size, the newBlock pointer will be freed!
bool fitIntoHole(MemBlock *hole, MemBlock *newBlock);

void freeList(MemBlock *head);

#endif // MEMLIST_H