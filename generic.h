#ifndef GENERIC_H
#define GENERIC_H
#include "status.h"

typedef void *ITEM;

typedef struct generic_vector {
  void(*assignment)(ITEM *pLeft, ITEM Right);
  void(*destroy)(ITEM *pItem);
  int size;
  int capacity;
  ITEM *data;
} Generic_vector;

#endif
