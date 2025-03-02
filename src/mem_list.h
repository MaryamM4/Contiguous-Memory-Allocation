#ifndef MEMLIST_H
#define MEMLIST_H

#include <stdbool.h>
#include <stdio.h>  // printf
#include <stdlib.h> // malloc

typedef struct {
  char owner;
  int size;

  MemBlock *next;
} MemBlock;

MemBlock *initBlock(char owner, int size);

void insertAfter(MemBlock *prevBlock, MemBlock *newBlock);

// Only inserts new memory block after given previous block if
// the next block exists, is free, and has a size >= newBlock.
// If insert is succesful, will subtract from size of orignical nextBlock.
bool insertAfterIfFit(MemBlock *prevBlock, MemBlock *newBlock);

void freeAll(MemBlock *head);

#endif // MEMLIST_H