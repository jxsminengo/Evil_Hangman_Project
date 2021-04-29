#include <stdio.h>
#include <stdlib.h>
#include "generic_vector.h"
#include "my_string.h"
#include <ctype.h>
#include "tree.h"

GENERIC_VECTOR* open_dictioanry(void);
Boolean continue_game(void);
Boolean running_total(void);
int wordLength(void);
int numGuesses(void);
char getChar(void);
void clear_keyboard_buffer(void);
void string_driver(void);

int main(int argc, char* argv[]) {
  //start game                                                                                                                           
  do{
    string_driver();
  } while(continue_game());

  return 0;
}

void clear_keyboard_buffer(void) {
  char c;
  do {
    scanf("%c", &c);
  } while (c != '\n');
  return;
}

GENERIC_VECTOR* open_dictionary(void) {
  GENERIC_VECTOR*  hgVector = (GENERIC_VECTOR*)malloc(sizeof(GENERIC_VECTOR) * 30);
  MY_STRING hString = NULL;
  FILE *fp;
  int i;
  //open dictionary to have words for the game                                                                                           
  fp = fopen("dictionary.txt", "r");
  if(fp == NULL) {
    printf("Failed to open the dictionary file.\n");
    exit(1);
  }

  for(i = 0; i < 30; i++) {
    hgVector[i] = generic_vector_init_default(my_string_assignment, my_string_destroy);
    if(hgVector[i] == NULL) {
      exit(1);
    }
  }

  hString = my_string_init_default();
  if(hString == NULL) {
    exit(1);
  }

  //get word into string from the dictionary                                                                                             
  while(my_string_extraction(hString, fp)) {
    if(my_string_get_size(hString) < 30) {
      if(!generic_vector_push_back(hgVector[my_string_get_size(hString)], (void*)hString)) {
        exit(1);
      }
    }
  }

  my_string_destroy(&hString);
  fclose(fp);
  return hgVector;
}

//To ask the player if they want to continue the game                                                                                    
Boolean continue_game(void) {
  char c;
  printf("Would you like to play again? y or n\n");
  scanf("%c", &c);
  clear_keyboard_buffer();

  c = tolower(c);

  if(!isalpha(c) || (isalpha(c) && c!= 'y'  && c != 'n')) {
    return continue_game();
  }

  if(c == 'y') {
    return TRUE;
  }

  if(c == 'n') {
    return FALSE;
  }

  return FALSE;
}

//To ask the player what word length they want                                                                                           
int wordLength(void) {
  int x;

  do {
    printf("what length word do you want to play with?(Choose value 2-29)\n");
    scanf("%d", &x);
    clear_keyboard_buffer();
  } while(x < 2 || x > 30);
  return x;
}

//To ask the player how many guesses they want                                                                                           
int numGuesses(void) {
  int x;

  do {
    printf("How many guesses would you like to have?(Enter positive value less than 26)\n");
    scanf("%d", &x);
    clear_keyboard_buffer();
  } while (x <= 0 || x >= 26);

  return x;
}

//Get the player's guess                                                                                                                 
char getChar(void) {
  char c;

  printf("\nEnter guess: ");
  scanf("%c", &c);
  clear_keyboard_buffer();
  printf("\n");
  //Just to make the game screen neater--can distinguish each round                                                                      

  if(isalpha(c)) {
    c = tolower(c);
  } else{
    return getChar();
  }

  return c;
}

//Ask if the player wants a running total                                                                                                
Boolean running_total(void) {
  char c;
  printf("Do you want a running total of the number of words remaining in the word list? y or n\n");
  scanf("%c", &c);
  clear_keyboard_buffer();
  c = tolower(c);

  if (!isalpha(c) || (c != 'y' && c != 'n')) {
    return running_total();
  }

  if (c == 'y') {
    return TRUE;
  }

  if(c == 'n') {
    return FALSE;
  }

  return TRUE;
}

//The Evil Hangman Game                                                                                                                  
void string_driver(void) {
  MY_STRING word_key = my_string_init_default();
  GENERIC_VECTOR* dictionary = open_dictionary();
  GENERIC_VECTOR largest_family = NULL;
  void* hTree = avl_tree_init_default();
  MY_STRING guesses = my_string_init_c_string(" ");
  MY_STRING word_family = my_string_init_default();
  int wordLen = wordLength();
  int numberGuesses = numGuesses();
  int totalRemain =  running_total();
  int i;
  char current;

  for(i = 0; i < wordLen; i++) {
    my_string_push_back(word_key, '-');
  }
  
   do{
    printf("\nYou have %d guesses remaining\n", numberGuesses);
    printf("Used letters: ");
    my_string_insertion(guesses, stdout);

    printf("\nword: ");
    my_string_insertion(word_key,stdout);

    do{
      current = getChar();
    } while(guessed_already(guesses, current));

    my_string_push_back(guesses, current);
    my_string_push_back(guesses, ',');
    my_string_push_back(guesses, ' '); //to separate guesses                                                                                                                                                

    if(totalRemain) {
      printf("Running total of the number of words remaining:\n");
    }
     
    for(i = 0; i < generic_vector_get_size(dictionary[wordLen]); i++) {
      get_word_key_value(word_key, word_family, generic_vector_at(dictionary[wordLen], i), current);
      avl_tree_push(hTree, word_family, generic_vector_at(dictionary[wordLen], i));
    }

    largest_family = avl_tree_get_largest_family(hTree, totalRemain);
    generic_vector_destroy(&(dictionary[wordLen]));
    dictionary[wordLen] = generic_vector_init_default(my_string_assignment, my_string_destroy);

    for(i= 0; i < generic_vector_get_size(largest_family); i++) {
      generic_vector_push_back(dictionary[wordLen], generic_vector_at(largest_family, i));
    }

    if(totalRemain) {
      printf("The computer has %d possibilities remaining.\n", generic_vector_get_size(largest_family));
    }

      get_word_key_value(word_key, word_family, generic_vector_at(dictionary[wordLen], 0), current);
    if(my_string_compare(word_key, word_family) == 0) {
      printf("I'm sorry, there were no %c's in the word.", current);
      numberGuesses--;
    }
     
    my_string_assignment(&word_key, word_family);

    generic_vector_destroy(&largest_family);
    avl_tree_destroy(&hTree);
    my_string_destroy(&word_family);
    word_family = my_string_init_default();
    hTree = avl_tree_init_default();

    if(numberGuesses == 0) {
      printf("\nOop! Looks like you lost!\nThe word was: ");
      my_string_insertion(generic_vector_at(dictionary[wordLen], 0), stdout);
      printf("\n\n");
      break;
    }

    if(my_string_compare(generic_vector_at(dictionary[wordLen], 0), word_key) == 0) {
      if(generic_vector_get_size(dictionary[wordLen]) == 1) {
        printf("**********\n");
        printf("\nYAY! YOU WON! The word was: ");
        my_string_insertion(generic_vector_at(dictionary[wordLen], 0), stdout);
        printf("\n\n");
        break;
      }
    }
  } while(numberGuesses > 0);
  
  avl_tree_destroy(&hTree);
  my_string_destroy(&word_key);
  my_string_destroy(&guesses);
  my_string_destroy(&word_family);
  for(i = 0; i < 30; i++) {
    generic_vector_destroy(&dictionary[i]);
  }
  free(dictionary);
}
