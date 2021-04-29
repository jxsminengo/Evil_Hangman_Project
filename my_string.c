#include "my_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct my_string
{
    int capacity;
    int size;
    char* data;
};
typedef struct my_string My_String;

MY_STRING my_string_init_default(void) {
    My_String *pMy_string;
    pMy_string = (My_String*)malloc(sizeof(My_String));

    if (pMy_string !=  NULL) {
        pMy_string->size = 0;
        pMy_string->capacity = 7;
        pMy_string->data = (char*)malloc(sizeof(char)*pMy_string->capacity);
    } else if (pMy_string->data == NULL) {
      free(pMy_string);
        pMy_string = NULL;
            printf("empty string, unable to allocate memory.");
        }
    return pMy_string;
}

MY_STRING my_string_init_c_string(const char* c_string) {
  int i = 0;
  int capacity = 0;
  int size = 0;
  char c = 'd';

  for(i = 0; c != '\0'; i++) {
    c = c_string[i];
  }

  size = i - 1;
  capacity = size;

  My_String* pMy_string;
  pMy_string = (My_String*)malloc(sizeof(My_String));
  if(pMy_string != NULL) {
    pMy_string->size = size;
    pMy_string->capacity = capacity;
    pMy_string->data = (char*)malloc(sizeof(char)*pMy_string->capacity);
  } else{
    free(pMy_string);
    pMy_string = NULL;
  }

  for(i = 0; i < capacity; i++) {
    pMy_string->data[i] = c_string[i];
  }

  return (MY_STRING)pMy_string;
}

int my_string_get_size(MY_STRING hMy_string) {
    My_String *pMy_string = (My_String*)hMy_string;
    return pMy_string->size;
}

int my_string_get_capacity(MY_STRING hMy_string) {
  My_String *pMy_string = (My_String*)hMy_string;
  return pMy_string->capacity;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string) {
  My_String *pLeft_string = (My_String*)hLeft_string;
  My_String *pRight_string = (My_String*)hRight_string;
  int i;

  for(i = 0; i < pLeft_string->size && i < pRight_string->size; i++) {
    if (pLeft_string->data[i] < pRight_string->data[i]) {
      return -1;
    } else if(pLeft_string->data[i] > pRight_string->data[i]) {
      return 1;
    }
  }

  return 0;
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp) {
  My_String* pMy_string = (My_String*)hMy_string;
  char c = 'd';
  int i = 0;
  c = fgetc(fp);

  while(isspace(c)) {
    c = fgetc(fp);
    if(c == EOF) {
      return FAILURE;
    }
  }
  pMy_string->size = 0;
  for (i = 0; !isspace(c); i++) {
    if(pMy_string->capacity > i + 1) {
      pMy_string->data[i] = c;
      pMy_string->size = i + 1;
    } else{
      pMy_string->data = (char*)realloc(pMy_string->data, pMy_string->capacity * 2);
      if(pMy_string->data == NULL) {
        return FAILURE;
      }

      pMy_string->capacity *= 2;
      pMy_string->data[i] = c;
      pMy_string->size = i + 1;
    }
    c = fgetc(fp);
    if(c == EOF) {
      return FAILURE;
    }
  }
  ungetc(c, fp);
  return SUCCESS;
}

Status my_string_insertion(MY_STRING hMy_string, FILE* fp) {
  My_String* pMy_string = (My_String*)hMy_string;
  char c = 'd';
  int i = 0;

  if(pMy_string == NULL) {
    return FAILURE;
  } else{
    for(i = 0; i < pMy_string->size; i++) {
      c = pMy_string->data[i];
      fprintf(fp, "%c", c);
    }
    return SUCCESS;
  }
}

char* my_string_at(MY_STRING hMy_string, int index) {
  My_String* pMy_string = (My_String*)hMy_string;

  if(index < pMy_string->size && index >= 0) {
    return (char*)&pMy_string->data[index];
  } else{
    return NULL;
  }
}

Status my_string_push_back(MY_STRING hMy_string, char item) {
  My_String *pMy_string = (My_String*)hMy_string;
  char* temp;
  int i;

  //if there is not enough room in the stack then make room                                                                     
  if(pMy_string->size >= pMy_string->capacity) {
    temp = (char*)malloc(sizeof(char) * pMy_string->capacity * 2);
    if (temp == NULL) {
      return FAILURE;
    }
    for (i = 0; i < pMy_string->size; i++) {
      temp[i] = pMy_string->data[i];
    }
    free(pMy_string->data);
    pMy_string->data = temp;
    pMy_string->capacity *= 2;
  }

  //assume that there is now enough room                                                                                        
  pMy_string->data[pMy_string->size] = item;
  pMy_string->size++;

  return SUCCESS;
}

Status my_string_pop_back(MY_STRING hMy_String) {
  My_String* pMy_string = (My_String*)hMy_String;
  if(pMy_string > 0) {
    pMy_string->size--;
    return SUCCESS;
  }

  return FAILURE;
}


char* my_string_c_str(MY_STRING hMy_string) {
  My_String* pMy_string = (My_String*)hMy_string;
  int i;
  char* temp;

    if(pMy_string->size >= pMy_string->capacity) {
      temp = (char*)malloc(sizeof(char) * pMy_string->capacity * 2);
      if(temp == NULL) {
        printf("Failed to allocate memory in my_string_c_str.\n");
      }

      for(i = 0; i < pMy_string->size; i++) {
        temp[i] = pMy_string->data[i];
      }
      free(pMy_string->data);
      pMy_string->data = temp;
    }

    pMy_string->data[pMy_string->size] = '\0';

    return pMy_string->data;
}

Status my_string_concat(MY_STRING hResult, MY_STRING hAppend) {
  My_String* pResult = (My_String*) hResult;
  My_String* pAppend = (My_String*) hAppend;
  if(pResult->size + pAppend->size > pResult->capacity) {
    char* newData = (char*)malloc(sizeof(char) * (pResult->size + pAppend->size + 2));
    if(newData == NULL) {
      return FAILURE;
    }

    for(int i = 0; i <= pResult->size; i++) {
      newData[i] = pResult->data[i];
    }

    free(pResult->data);
    pResult->data = newData;
    pResult->capacity = (pResult->size + pAppend->size);
  }

  for (int i = 0; i <= pAppend->size; i++){
    pResult->data[i+pResult->size] = pAppend->data[i];
  }

  pResult->size = (pResult->size + pAppend->size) + 1;
  pResult->data[pResult->size] = '\0';

  return SUCCESS;
}

Boolean my_string_empty(MY_STRING hMy_string) {
  My_String* pMy_string = (My_String*) hMy_string;

  if(pMy_string->size > 0) {
    return FALSE;
  }

  return TRUE;
}

/*                                                                                                                                       
Status my_string_assignment(ITEM* pLeft, ITEM Right) {                                                                                   
  My_String* pMy_string_left = (My_String*)*pLeft;                                                                                       
  My_String* pMy_string_right = (My_String*)Right;                                                                                       
  char* temp;                                                                                                                            
  int i;                                                                                                                                 
                                                                                                                                         
  if (pMy_string_left == NULL) {                                                                                                         
    pMy_string_left =(My_String*) my_string_init_default();                                                                              
    if(pMy_string_left == NULL) {                                                                                                        
      return FAILURE;                                                                                                                    
    }                                                                                                                                    
  }                                                                                                                                      
  if (pMy_string_right == NULL) {                                                                                                        
    return FAILURE;                                                                                                                      
  }                                                                                                                                      
                                                                                                                                         
  if(pMy_string_left->capacity < pMy_string_right->size) {                                                                               
    temp = (char*)malloc((pMy_string_right->capacity) * sizeof(char));                                                                   
    if (temp == NULL) {                                                                                                                  
      return FAILURE;                                                                                                                    
    }                                                                                                                                    
    free(pMy_string_left->data);                                                                                                         
    pMy_string_left->data = temp;                                                                                                        
    pMy_string_left->capacity = pMy_string_right->capacity;                                                                              
  }                                                                                                                                      
  pMy_string_left->size = pMy_string_right->size;                                                                                        
  for(i = 0; i <pMy_string_right->size; i++) {                                                                                           
    pMy_string_left->data[i] = pMy_string_right->data[i];                                                                                
  }                                                                                                                                      
  *pLeft = pMy_string_left;                                                                                                              
  return SUCCESS;                                                                                                                        
}*/

//Redid to fit generic files                                                                                                             
void my_string_assignment(ITEM* pLeft, ITEM Right) {
  My_String* pLeft_string = (My_String*)*pLeft;
  My_String* pRight_string = (My_String*)Right;
  int i;
  char* temp;
  temp = my_string_c_str((MY_STRING)Right);
  if(pLeft_string == NULL) {
    pLeft_string = my_string_init_c_string(temp);
  } else{
    pLeft_string->size = 0;
    for(i = 0; i< pRight_string->size + 1; i++) {
      my_string_push_back(pLeft_string, pRight_string->data[i]);
    }
  }

  *pLeft = pLeft_string;
}

void my_string_destroy(ITEM* pItem) {
  My_String* pMy_string  = (My_String*)*pItem;
  free(pMy_string->data);
  free(pMy_string);
  *pItem = NULL;
}

Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess) {
  int i;

  while(!my_string_empty(new_key)) {
    my_string_pop_back(new_key);
  }

  for(i = 0; i < my_string_get_size(word); i++) {
    if(!my_string_push_back(new_key, *my_string_at(word, i) == guess ? guess : *my_string_at(current_word_family, i))){
       return FAILURE;
      }
  }
    return SUCCESS;
}

Boolean guessed_already(MY_STRING guess_list, char c) {
  My_String* pMy_string = (My_String*)guess_list;
  int i;

  for(i = 0; i < my_string_get_size(guess_list); i++) {
    if(pMy_string->data[i] == c) {
      printf("You already guessed this letter! Enter another letter...\n");
      return TRUE;
    }
  }

  return FALSE;
}
