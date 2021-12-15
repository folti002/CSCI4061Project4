#ifndef CLIENT_H
#define CLIENT_H

#include "utils.h"

// track the cash available to service transactions
float cash = START_CASH;
char* clientStr = "(CLIENT)";

void printSyntax();

#endif
