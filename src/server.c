#include "server.h"

#define SA struct sockaddr

account_t* balances;
int balancesSize = 0;
pthread_mutex_t balancesSizeLock = PTHREAD_MUTEX_INITIALIZER;

// Logic for registering an account
void registerAccount(int connfd){
    if(balancesSize == MAX_ACC){
        printf("%s ERROR: max account number reached", serverStr);
        return;
    }
    int amt = 0;

    // Set up variables to be read in from socket
    char username[MAX_STR];
    memset(username, 0, MAX_STR);
    char name[MAX_STR];
    memset(name, 0, MAX_STR);
    time_t birthday;

    // Get username for new account
    if((amt=read(connfd, username, sizeof(char)*MAX_STR)) < 0){
        printf("%s registerAccount failed to read username\n.", serverStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // Get name for new account
    if((amt=read(connfd, name, sizeof(char)*MAX_STR)) < 0){
        printf("%s registerAccount failed to read name\n.", serverStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // Get birthday for new account
    if((amt=read(connfd, &birthday, sizeof(time_t))) != sizeof(time_t)){
        printf("%s registerAccount failed to read birthday\n.", serverStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }

    //printf("%s Received %s, %s, %ld\n", serverStr, username, name, birthday);

    // Lock the shared balance structures
    pthread_mutex_lock(&balancesSizeLock);
    pthread_mutex_lock(&balances[balancesSize].accountLock);

    // Register a new account
    strcpy(balances[balancesSize].username, username);
    strcpy(balances[balancesSize].name, name);
    balances[balancesSize].birthday = birthday;
    balances[balancesSize].balance = 0.0;
    balances[balancesSize].accountNumber = balancesSize;

    // Return BALANCE message to client
    int msg = BALANCE;

    // Write message type
    if((amt=write(connfd, &msg, sizeof(int))) != sizeof(int)){
        printf("%s registerAccount failed to write msg\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write account number
    if((amt=write(connfd, &balancesSize, sizeof(int))) != sizeof(int)){
        printf("%s registerAccount failed to write accountNumber\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write balance for this customer (will be 0.0 because this account was just inititalized)
    if((amt=write(connfd, &(balances[balancesSize].balance), sizeof(float))) != sizeof(float)){
        printf("%s registerAccount failed to write balance\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }

    // Increment size of balances structure and unlock shared data
    pthread_mutex_unlock(&balances[balancesSize].accountLock);
    balancesSize++;
    pthread_mutex_unlock(&balancesSizeLock);
}

// Logic for sending ACCOUNT_INFO
void accountInfo(int connfd){
    printf("%s Account_info protocol\n", serverStr);

    int amt = 0;

    // Variable to store account number sent by client
    int accountNumber;

    // Get accountNumber from client
    if((amt=read(connfd, &accountNumber, sizeof(int))) != sizeof(int)){
        printf("%s accountInfo failed to read accountNumber\n.", serverStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }

    // Check if valid accountNumber
    if(accountNumber > balancesSize || accountNumber < 0){
        printf("%s Invalid account number inputted! Continuing...\n", serverStr);
        return;
    }

    // Lock the current account while editing
    pthread_mutex_lock(&balances[accountNumber].accountLock);

    // Return BALANCE message to client
    int msg = ACCOUNT_INFO;

    // Write message type
    if((amt=write(connfd, &msg, sizeof(int))) != sizeof(int)){
        printf("%s accountinfo failed to write msg\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Send username
    if((amt=write(connfd, balances[accountNumber].username, sizeof(char)*MAX_STR)) != sizeof(char)*MAX_STR){
        printf("%s accountInfo failed to write username\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Send name
    if((amt=write(connfd, balances[accountNumber].name, sizeof(char)*MAX_STR)) != sizeof(char)*MAX_STR){
        printf("%s accountInfo failed to write name\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Send birthday
    if((amt=write(connfd, &balances[accountNumber].birthday, sizeof(time_t))) != sizeof(time_t)){
        printf("%s accountInfo failed to write birthday\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }

    // Unlock the current account
    pthread_mutex_unlock(&balances[accountNumber].accountLock);
}

// Logic for handling a TRANSACT request
void handleTransaction(int connfd){
    printf("%s Transfer protocol\n", serverStr);
    
}

// Logic for sending BALANCE
void balanceHandler(int connfd){
    printf("%s Balance protocol\n", serverStr);

    int amt = 0;

    // Variable to store account number sent by client
    int accountNumber;

    // Get accountNumber from client
    if((amt=read(connfd, &accountNumber, sizeof(int))) != sizeof(int)){
        printf("%s balanceHandler failed to read accountNumber\n.", serverStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }

    // Check if valid accountNumber
    if(accountNumber > balancesSize || accountNumber < 0){
        printf("%s Invalid account number inputted! Continuing...\n", serverStr);
        return;
    }

    // Lock the current account while editing
    pthread_mutex_lock(&balances[accountNumber].accountLock);

    // Return BALANCE message to client
    int msg = BALANCE;

    // Write message type
    if((amt=write(connfd, &msg, sizeof(int))) != sizeof(int)){
        printf("%s balanceHandler failed to write msg\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write account number
    if((amt=write(connfd, &accountNumber, sizeof(int))) != sizeof(int)){
        printf("%s balanceHandler failed to write accountNumber\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write balance for this customer (will be 0.0 because this account was just inititalized)
    if((amt=write(connfd, &(balances[accountNumber].balance), sizeof(float))) != sizeof(float)){
        printf("%s balanceHandler failed to write balance\n.", serverStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }

    // Unlock the account we were just accessing
    pthread_mutex_unlock(&balances[accountNumber].accountLock);
}

// Logic for sending CASH
void cash(int connfd){
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
void* logThread(){
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

    // Keep reading from client until we encounter a TERMINATE message
    while(1){
        if((size = read(connectionfd, &msg, sizeof(int))) != sizeof(int)){
            printf("%s failed to read message type from client\n", serverStr);
            printf("It read %d bytes.\n", size);
            exit(1);
        }

        if(msg == REGISTER){
            registerAccount(connectionfd);
        } else if(msg == GET_ACCOUNT_INFO){
            accountInfo(connectionfd);
        } else if(msg == TRANSACT){
            handleTransaction(connectionfd);
        } else if(msg == GET_BALANCE){
            balanceHandler(connectionfd);
        } else if(msg == REQUEST_CASH){
            cash(connectionfd);
        } else if(msg == ERROR){    
            // FOR NOW DO NOTHING
            continue;
        } else if(msg == TERMINATE){
            close(connectionfd);
            break;
        } else if(msg == REQUEST_HISTORY){
            // EXTRA CREDIT
            continue;
        } else {
            printf("%s Invalid message type read from client!\n", serverStr);
            exit(1);
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

    // Set up the global balances structure
    balances = (account_t*) malloc(sizeof(account_t) * MAX_ACC);

    // Launch log thread
    pthread_t tid;
    pthread_create(&tid, NULL, logThread, NULL);

    for(int i = 0; i < MAX_ACC; i++){
        balances[i].accountLock = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
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

    // Continue accepting clients forever until launcher kills server.c
    while(1){
        // Accept an incoming request
        int* connfd = malloc(sizeof(int));
        *connfd = accept(sockfd, (SA*) &cli, &len);
        if(*connfd < 0){
            printf("%s Server accept failed!\n", serverStr);
            exit(0);
        } else {
            printf("%s Server has accepted the client!\n", serverStr);
        }

        // Spawn a worker thread and detach it to handle the requests of the client
        pthread_t worker_tid;
        if(pthread_create(&worker_tid, NULL, requestHandler, (void*) connfd) != 0){
            printf("%s Worker thread failed to create\n", serverStr);
            exit(1);
        }
        pthread_detach(worker_tid);
    }

    // Clean up (this will actually never happen since the above while loop never breaks)
    free(balances);
    close(sockfd);

    return 0; 
}