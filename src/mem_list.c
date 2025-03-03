#include "mem_list.h"

// ==============
// Node Functions
// --------------

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

MemBlock *initHole(int size) {
  MemBlock *newBlock = (MemBlock *)malloc(sizeof(MemBlock));
  if (newBlock == NULL) {
    printf("initBlock ERROR: Failed to allocate memory for MemBlock.\n");
    return NULL;
  }

  newBlock->owner = HOLE;
  newBlock->size = size;
  newBlock->next = NULL;

  return newBlock;
}

// Set owner to HOLE.
// If MemBlock after is a hole, merge the two.
void convertToHole(MemBlock *block) {
  block->owner = HOLE;

  if (block->next && block->next->owner == HOLE) {
    block->size += block->next->size;

    MemBlock *to_del = block->next;
    block->next = block->next->next;
    free(to_del);
  }
}

// ==============
// List Functions
// --------------

void insertEnd(MemBlock *head, MemBlock *newBlock) {
  if (!head) {
    printf("insertEnd ERROR: Null list.");
    return;
  }
  if (!newBlock) {
    printf("insertEnd ERROR: Null new block.");
    return;
  }

  MemBlock *it = head;
  while (it->next != NULL) {
    it = it->next;
  }
  it->next = newBlock;
}

void insertAfter(MemBlock *prevBlock, MemBlock *newBlock) {
  newBlock->next = prevBlock->next;
  prevBlock->next = newBlock;
}

bool fitAfter(MemBlock *prevBlock, MemBlock *newBlock) {
  if (!prevBlock->next || prevBlock->next->owner != HOLE) {
    return false; // Next space DNE or is owned by a process.
  }
  if (prevBlock->next->size < newBlock->size) {
    return false; // Not enough free space.
  }

  prevBlock->next->size -= newBlock->size;
  insertAfter(prevBlock, newBlock);
  return true;
}

// "Inserts" new memory block "above" the hole if
// the hole has enough space to fit the new block into.
// If they're the same size, the newBlock pointer will be freed!
bool fitIntoHole(MemBlock *hole, MemBlock *newBlock) {
  if (!hole || hole->owner != HOLE) {
    return false;
  }
  if (!newBlock || newBlock->owner == HOLE) {
    return false;
  }

  if (hole->size < newBlock->size) {
    return false;
  }

  // Swap info, since we can't access node before hole.
  char owner = newBlock->owner;
  int size = newBlock->size;

  newBlock->owner = HOLE;
  newBlock->size = hole->size - size;

  hole->owner = owner;
  hole->size = size;

  if (newBlock->size > 0) {
    newBlock->next = hole->next;
    hole->next = newBlock;

  } else {
    free(newBlock);
  }

  return true;
}

void freeList(MemBlock *head) {
  MemBlock *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}