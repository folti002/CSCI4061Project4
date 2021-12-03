#include "client.h"

void printSyntax(){
    printf("incorrect usage syntax! \n");
    printf("usage: $ ./client input_filename server_addr server_port\n");
}

int main(int argc, char *argv[]){
    // Argument handling
    if(argc != 4){
        printSyntax();
        return 0;
    }

    clock_t time;
    time = clock();

    return 0; 
}

