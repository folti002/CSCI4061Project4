#ifndef SERVER_H
#define SERVER_H

#include "utils.h"

// Account struct to store all information for an account in the bank system
typedef struct account
{
  int accountNumber;
  float balance;
  char username[MAX_STR];
  char name[MAX_STR];
  time_t birthday;
  pthread_mutex_t accountLock;
} account_t;

// Global balances array structure (will be malloc'd in server.c)
account_t* balances;
// Global integer to hold size of balances array currently
int balancesSize = 0;
// Global lock to protect the current size of the balances array
pthread_mutex_t balancesSizeLock = PTHREAD_MUTEX_INITIALIZER;


// String to print before all prints in server
char* serverStr = "(SERVER)";
// Log file name
char* logFileName = "output/balances.csv";

void printSyntax();

#endif

