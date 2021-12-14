#include "server.h"

#define SA struct sockaddr

/*
SERVER NEEDS TO SEND:
ACCOUNT_INFO (4) - After clients receive this message, they do nothing.
BALANCE (5)
CASH (7)
ERROR (8) - Sending this message to a client, when the server receives a message not defined in enum. Clients do nothing when they receive ERROR.
- HISTORY (11)
*/



const char* const msg_str[] = 
{
	"REGISTER",
	"GET_ACCOUNT_INFO",
	"TRANSACT",
	"GET_BALANCE",
	"ACCOUNT_INFO",
	"BALANCE",
	"REQUEST_CASH",
	"CASH",
	"ERROR",
	"TERMINATE",
	/* extra credit messages */
	"REQUEST_HISTORY",
	"HISTORY",
};

void printSyntax(){
    printf("incorrect usage syntax! \n");
    printf("usage: $ ./server server_addr server_port num_workers\n");
}

void logThread(){
    while(1){


        sleep(LOGGER_SLEEP);
    }
}

int main(int argc, char *argv[]){
    // Argument handling
    if(argc != 4){
        printSyntax();
        return 0;
    }

    // Create empty output folder
    bookeepingCode();

    // Read in command-line arguments
    char* IPaddress = argv[1];
    int port = atoi(argv[2]);
    int numWorkers = atoi(argv[3]);

    char* serverStr = "(SERVER)";
    //printf("%s %s %d %d\n", serverStr, IPaddress, port, numWorkers);

    // Create empty output folder
    bookeepingCode();

    // Create socket and validate
    int sockfd, len;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("%s Socket creation failed!\n", serverStr);
        exit(0);
    } else {
        printf("%s Socket successfully created!\n", serverStr);
    }
    bzero(&servaddr, sizeof(servaddr));

    // Assign IP and port number
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(IPaddress);
    servaddr.sin_port = htons(port);

    // Bind newly created socket to the inputted IP address
    if((bind(sockfd, (SA*) &servaddr, sizeof(servaddr))) != 0){
        printf("%s Socket bind failed!\n", serverStr);
        exit(0);
    } else {
        printf("%s Socket successfully binded!\n", serverStr);
    }

    // Now the server is ready to listen to incoming requests
    if((listen(sockfd, 5)) != 0){
        printf("%s Listen failed!\n", serverStr);
    } else {
        printf("%s Server listening...\n", serverStr);
    }
    len = sizeof(cli);

    // FOR INTERIM SOLUTION
    pthread_t pid;
    int connfd;

    while(1){
        connfd = accept(sockfd, (SA*) &cli, &len);
        if(connfd < 0){
            printf("%s Server accept failed!\n", serverStr);
            exit(0);
        } else {
            printf("%s Server has accepted the client!\n", serverStr);
        }

        char msg[sizeof(int)];
        memset(msg, 0, sizeof(int));
        int size = read(connfd, msg, sizeof(msg));
        if(size < 0){
            perror("Read error");
            exit(1);
        }
    }

    // while(1){
    //     char msg[sizeof(int)];
    //     memset(msg, 0, sizeof(int));
    //     int size = read(connfd, msg, sizeof(msg));
    //     if(size < 0){
    //         perror("Read error");
    //         exit(1);
    //     }
    //     int enumVal = atoi(msg);
    //     printf("%s %s: %d\n", serverStr, msg_str[enumVal], enumVal);
    //     char enumAsStr[sizeof(int)];
    //     sprintf(enumAsStr, "%d", enumVal);
    //     if(write(connfd, enumAsStr, strlen(enumAsStr)) < 0){
    //         perror("Failed to write");
    //         exit(1);
    //     }
    //     if(strcmp(msg_str[enumVal], "TERMINATE") == 0){
    //         break;
    //     }
    // }

    close(sockfd);

    /*
    while(1){
        connfd[count%]
    }
    */

    return 0; 
}

