#include "mem_list.h"

MemBlock *initBlock(char owner, int size) {
  MemBlock *newBlock = (MemBlock *)malloc(sizeof(MemBlock));
  if (newBlock == NULL) {
    printf("initBlock ERROR: Failed to allocate memory for MemBlock.\n");
    return NULL;
  }

  newBlock->owner = owner;
  newBlock->size = size;
  newBlock->next = NULL;

  return newBlock;
}

void insertAfter(MemBlock *prevBlock, MemBlock *newBlock) {
  newBlock->next = prevBlock->next;
  prevBlock->next = newBlock;
}

bool insertAfterIfFit(MemBlock *prevBlock, MemBlock *newBlock) {
  if (!prevBlock->next || prevBlock->next->owner != NULL) {
    return false; // Next space DNE or is owned by a process.
  }
  if (prevBlock->next->size < newBlock->size) {
    return false; // Not enough free space
  }

  prevBlock->next->size -= newBlock->size;
  insertAfter(prevBlock, newBlock);
  return true;
}

void freeAll(MemBlock *head) {
  MemBlock *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}