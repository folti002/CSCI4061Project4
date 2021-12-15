#ifndef SERVER_H
#define SERVER_H

#include "utils.h"

// skeleton of a struct to hold account information
// this should contain information passed with each 
// new account that is registered (see REGISTER in 
// the writeup...)
// 
// add any other fields you feel are necessary to 
// for implementing the extra credit history or
// for maintaining synchronization among threads
typedef struct account
{
  int accountNumber;
  float balance;
  char username[MAX_STR];
  char name[MAX_STR];
  time_t birthday;
  pthread_mutex_t accountLock;
} account_t;

// array of structs to store account information
// struct account balances[MAX_ACC];
// ALT: you may declare it as a pointer and malloc
// struct account * balances;

char* serverStr = "(SERVER)";
char* logFileName = "output/balances.csv";

void printSyntax();

#endif

