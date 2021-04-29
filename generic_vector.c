#include <stdio.h>
#include <stdlib.h>
#include "generic.h"
#include "generic_vector.h"

GENERIC_VECTOR generic_vector_init_default( void( *assignment )( ITEM* pLeft, ITEM Right ), void( *destroy )( ITEM *pItem ) ) {
  Generic_vector *pgVector ;
  int i;
  pgVector = (Generic_vector*)malloc(sizeof(Generic_vector));

  if (pgVector != NULL) {
    pgVector->size = 0 ;
    pgVector->capacity = 1 ;

    pgVector->data = (ITEM*)malloc(pgVector->capacity * sizeof(ITEM));
    if (pgVector->data == NULL) {
      free(pgVector);
      pgVector = NULL;
    } else{
      for ( i = 0; i < pgVector -> capacity; i++ ) {
	pgVector->data[i] = NULL;
      }
      pgVector->assignment = assignment;
      pgVector->destroy = destroy;
    }
  }
  return pgVector;
}

void generic_vector_destroy(GENERIC_VECTOR *phVector) {
    Generic_vector *pgVector = (Generic_vector*)*phVector;
    int i;

    for (i = 0; i < pgVector -> size; i++) {
      if (pgVector->data[i] != NULL) {
	pgVector->destroy(&(pgVector->data[i]));
      }
    }

    free(pgVector->data);
    free(pgVector);
    *phVector = NULL;
}

int generic_vector_get_capacity(GENERIC_VECTOR hVector) {
    return ((Generic_vector*)hVector)->capacity;
}

int generic_vector_get_size(GENERIC_VECTOR hVector) {
    return ((Generic_vector*)hVector)->size;
}

Status generic_vector_push_back(GENERIC_VECTOR hVector, ITEM value) {
    Generic_vector *pgVector = (Generic_vector*)hVector;
    ITEM *new_data;
    int i;

    if (pgVector->size >= pgVector->capacity) {
      //resize if not enough space                                                                                                       
      new_data = (ITEM*)realloc(pgVector->data, 2 * pgVector->capacity * sizeof(ITEM));
      if (new_data == NULL) {
        return FAILURE;
      }
      pgVector->capacity *= 2;
      for (i = pgVector->size; i < pgVector->capacity; i++) {
        new_data[i] = NULL;
      }
      pgVector->data = new_data;
    }

    pgVector->assignment(&pgVector->data[pgVector->size], value);

    if (pgVector->data[pgVector->size] == NULL) {
      return FAILURE;
    }

    pgVector->size++;
    return SUCCESS;
}

Status generic_vector_pop_back(GENERIC_VECTOR hVector) {
    Generic_vector *pgVector = (Generic_vector*)hVector ;
    if (pgVector->size <= 0) {
      return FAILURE;
    }
    pgVector->size--;

    if (pgVector->data[pgVector -> size] != NULL) {
        pgVector->destroy(&(pgVector->data[pgVector->size]));
    }
    return SUCCESS;
}

ITEM generic_vector_at(GENERIC_VECTOR hVector, int index) {
    Generic_vector *pgVector = (Generic_vector*)hVector ;
    if (index < 0 || index >= pgVector -> size)  {
      return NULL;
    }
    return (pgVector->data[index]);
}

Boolean generic_vector_empty(GENERIC_VECTOR hVector) {
    return (((Generic_vector*)hVector)->size) == 0;
}
