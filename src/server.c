#include "server.h"

#define SA struct sockaddr

/*
SERVER NEEDS TO SEND:
ACCOUNT_INFO (4) - After clients receive this message, they do nothing.
BALANCE (5)
CASH (7)
ERROR (8) - Sending this message to a client, when the server receives a message not defined in enum. Clients do nothing when they receive ERROR.
HISTORY (11)
*/

int balancesSize = 0;
pthread_mutex_t balancesSizeLock = PTHREAD_MUTEX_INITIALIZER;

// Logic for sending ACCOUNT_INFO
void account_info(int connfd, char username[MAX_STR], char name[MAX_STR]){
    printf("%s Account_info protocol\n", serverStr);
}

// Logic for sending BALANCE
void balance(int connfd, int accountNumber, float balance){
    printf("%s Balance protocol\n", serverStr);
}

// Logic for sending CASH
void cash(int connfd, float cash){
    printf("%s Cash protocol\n", serverStr);
}

// Logic for sending ERROR
void error(int connfd){
    printf("%s Error protocol\n", serverStr);
}

// Logic for sending HISTORY
void history(int connfd){
    printf("%s History protocol\n", serverStr);
}

void printSyntax(){
    printf("incorrect usage syntax! \n");
    printf("usage: $ ./server server_addr server_port num_workers\n");
}

// Continuously running thread that will log accounts and their information every LOGGER_SLEEP seconds
void logThread(){
    while(1){
        sleep(LOGGER_SLEEP);
        FILE* logFile;
        logFile = fopen(logFileName, "w+");
        for(int i = 0; i < balancesSize; i++){
            pthread_mutex_lock(&balances[i].accountLock);
            fprintf(logFile, "%d,%.2f,%s,%s,%ld\n", 
                    balances[i].accountNumber, balances[i].balance,
                    balances[i].name, balances[i].username, balances[i].birthday);
            pthread_mutex_unlock(&balances[i].accountLock);
        }
        fclose(logFile);
    }
}

void* requestHandler(void* connfd){
    int size = 0;
    int msg;
    int connectionfd = *((int*) connfd);

    while(1){
        if((size = read(*connectionfd, &msg, sizeof(int))) != sizeof(int)){
            printf("%s failed to read message type from client\n", serverStr);
            printf("It read %d bytes.\n", size);
            exit(1);
        }

        if(msg == REGISTER){
            
        } else if(msg == GET_ACCOUNT_INFO){

        } else if(msg == TRANSACT){

        } else if(msg == GET_BALANCE){

        } else if(msg == REQUEST_CASH){

        } else if(msg == ERROR){    

        } else if(msg == TERMINATE){
            close(connfd);
            break;
        } else if(msg == REQUEST_HISTORY){

        } else {
            printf("%s Invalid message type read from client!\n", serverStr);
        }
    }
    return NULL;
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

    // Launch log thread
    pthread_t tid;
    pthread_create(&tid, NULL, logThread, NULL);

    for(int i = 0; i < MAX_ACC; i++){
        balances[i].accountLock = PTHREAD_MUTEX_INITIALIZER;
    }

    //printf("%s %s %d %d\n", serverStr, IPaddress, port, numWorkers);

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

    // This is to prevent a bind failing when running the program many times in a row
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        printf("%s setsockopt(SO_REUSEADDR) failed\n", serverStr);
        exit(1);
    }

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
        int* connfd = malloc(sizeof(int));
        *connfd = accept(sockfd, (SA*) &cli, &len);
        if(*connfd < 0){
            printf("%s Server accept failed!\n", serverStr);
            exit(0);
        } else {
            printf("%s Server has accepted the client!\n", serverStr);
        }

        pthread_t worker_tid;
        if(pthread_create(&worker_tid, NULL, requestHandler, (void*) connfd) != 0){
            printf("%s Worker thread failed to create\n", serverStr);
            exit(1);
        }
        pthread_detach(worker_tid);
    }

    close(sockfd);

    return 0; 
}