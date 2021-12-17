#ifndef CLIENT_H
#define CLIENT_H

#include "utils.h"

// Track the cash available to service transactions
float cash = START_CASH;

// String to print before all prints in client
char* clientStr = "(CLIENT)";

void printSyntax();

#endif
