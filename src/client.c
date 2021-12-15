#include "client.h"

#define SA struct sockaddr

// logic for sending REGISTER request
void registerRequest(int sockfd, char username[MAX_STR], char name[MAX_STR], time_t birthday){
    printf("%s Register request\n", clientStr);
    // Hold number of bytes read/written
    int amt = 0;
    // Message type written to socket
    int msg_type = REGISTER;
    // Values to store returned information
    int rsp_type;
    int accountNumber;
    float balance;

    // Send message type
    if((amt=write(sockfd, &msg_type, sizeof(int))) != sizeof(int)){
        printf("%s register failed to write msg_type\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Send username
    if((amt=write(sockfd, username, sizeof(char)*MAX_STR)) != sizeof(char)*MAX_STR){
        printf("%s register failed to write username\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Send name
    if((amt=write(sockfd, name, sizeof(char)*MAX_STR)) != sizeof(char)*MAX_STR){
        printf("%s register failed to write name\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Send birthday
    if((amt=write(sockfd, &birthday, sizeof(time_t))) != sizeof(time_t)){
        printf("%s register failed to write birthday\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }



    // Read in response from server
    if((amt=read(sockfd, &rsp_type, sizeof(int))) != sizeof(int)){
        printf("%s register failed to read rsp_type\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // handle us getting the wrong value back
    else if(rsp_type != BALANCE){
        printf("%s register recieved wrong rsp_type %d\n", clientStr, rsp_type);
        exit(1);
    }
    if((amt=read(sockfd, &accountNumber, sizeof(int))) != sizeof(int)){
        printf("%s register failed to read accountNumber\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    if((amt=read(sockfd, &balance, sizeof(float))) != sizeof(float)){
        printf("%s register failed to read balance\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }

    printf("%s BALANCE FOR %d: %f\n", clientStr, accountNumber, balance);
}

// logic for sending GET_ACCOUNT_INFO request 
void get_account_info(int sock_fd, int accountNumber){
    printf("%s Get_account_info request\n", clientStr);
    // Integer to hold number of bytes read/written
    int amt = 0;

    // Variable to write to the socket
    int msg_type = GET_ACCOUNT_INFO;

    // Variables to write the read response into
    int rsp_type;
    char name[MAX_STR];
    char username[MAX_STR];
    time_t birthday;
    
    // Using read and write aren't the best "real world" call
    // but will be the simplest for you to use
    //
    // Write the message type first
    if((amt=write(sock_fd, &msg_type, sizeof(int))) != sizeof(int)){
        printf("%s get_account_info failed to write msg_type\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write the argument for the message type
    if((amt=write(sock_fd, &accountNumber, sizeof(int))) != sizeof(int)){
        printf("%s get_account_info failed to write accountNumber\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    


    // Read in the response message type first
    // As this tells us what fields we will need
    // To read afterwards and their types
    if((amt=read(sock_fd, &rsp_type, sizeof(int))) != sizeof(int)){
        printf("%s get_account_info failed to read rsp_type\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // Handle us getting the wrong value back
    else if(rsp_type != ACCOUNT_INFO){
        printf("%s get_account_info recieved wrong rsp_type\n", clientStr);
        exit(1);
    }

    // Get the username
    if((amt=read(sock_fd, &username, sizeof(char)*MAX_STR)) < 1){
        printf("%s get_account_info failed to read username\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // Get the name
    if((amt=read(sock_fd, &name, sizeof(char)*MAX_STR)) < 1){
        printf("%s get_account_info failed to read name\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // Get the birthday
    if((amt=read(sock_fd, &birthday, sizeof(time_t))) != sizeof(time_t)){
        printf("%s get_account_info failed to read birthday\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }

    printf("%s Name: %s, Username: %s, Birthday: %ld\n", clientStr, name, username, birthday);
}

// logic for sending REQUEST_CASH request
void request_cash(int sockfd){
    printf("Request_cash request\n");

    // Integer to hold number of bytes read/written
    int amt = 0;

    // Variables to write to socket
    int msg = REQUEST_CASH;
    float cashRequest = CASH_AMOUNT;
    
    // Variables to store response
    int rsp;
    float cashReturned;

    // Write message type
    if((amt=write(sockfd, &msg, sizeof(int))) != sizeof(int)){
        printf("%s request_cash failed to write msg_type\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write argument for message type
    if((amt=write(sockfd, &cashRequest, sizeof(float))) != sizeof(float)){
        printf("%s request_cash failed to write cashRequest\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }



    // Read in response message type
    if((amt=read(sockfd, &rsp, sizeof(int))) != sizeof(int)){
        printf("%s request_cash failed to read rsp\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // What if we get wrong value back?
    else if(rsp != CASH){
        printf("%s request_cash received the wrong response type\n", clientStr);
        exit(1);
    }

    // Read in cash amount returned
    if((amt=read(sockfd, &cashReturned, sizeof(float))) != sizeof(float)){
        printf("%s request_cash failed to read cashReturned\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }

    // Update the global cash variable
    cash += cashReturned;
}

// logic for handling a TRANSACT request
void transact(int sockfd, int accountNumber, float val){
    printf("%s Transact request\n", clientStr);

    // Send a REQUEST_CASH message to server until we have enough money on hand for the transaction
    while(cash < val){
        request_cash(sockfd);
    }

    // Integer to hold number of bytes read/written
    int amt = 0;
    
    // Message to write to the socket
    int msg = TRANSACT;

    // Variables to store the response
    int rsp;
    int retAccountNumber;
    float retBalance;

    // Write message type
    if((amt=write(sockfd, &msg, sizeof(int))) != sizeof(int)){
        printf("%s transact failed to write msg_type\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write account number
    if((amt=write(sockfd, &accountNumber, sizeof(int))) != sizeof(int)){
        printf("%s transact failed to write accountNumber\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write transaction amount
    if((amt=write(sockfd, &val, sizeof(float))) != sizeof(float)){
        printf("%s transact failed to write val\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }



    // Read in response message type
    if((amt=read(sockfd, &rsp, sizeof(int))) != sizeof(int)){
        printf("%s transact failed to read rsp\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // What if we get wrong value back?
    else if(rsp != BALANCE){
        printf("%s transact received the wrong response type\n", clientStr);
        exit(1);
    }

    // Read in the returned account number
    if((amt=read(sockfd, &retAccountNumber, sizeof(int))) != sizeof(int)){
        printf("%s transact failed to read retAccountNumber\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Read in the returned balance
    if((amt=read(sockfd, &retBalance, sizeof(float))) != sizeof(float)){
        printf("%s transact failed to read retBalance\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }

    // add the value of the transaction to the cash
    // variable (deposits increase, withdrawals decrease)
    cash -= val;

    // NOTE: make sure to use proper error handling
}

// logic for sending GET_BALANCE request
void get_balance(int sockfd, int accountNumber){
    printf("%s Get_balance request\n", clientStr);

    // Integer to hold number of bytes read/written
    int amt = 0;

    // Variable to write to socket
    int msg = GET_BALANCE;
    
    // Variables to store response
    int rsp;
    int retAccountNumber;
    float balance;

    // Write message type
    if((amt=write(sockfd, &msg, sizeof(int))) != sizeof(int)){
        printf("%s get_balance failed to write msg_type\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // Write argument for message type
    if((amt=write(sockfd, &accountNumber, sizeof(int))) != sizeof(int)){
        printf("%s get_balance failed to write accountNumber\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }



    // Read in response message type
    if((amt=read(sockfd, &rsp, sizeof(int))) != sizeof(int)){
        printf("%s get_balance failed to read rsp\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // What if we get wrong value back?
    else if(rsp != BALANCE){
        printf("%s get_balance received the wrong response type\n", clientStr, rsp);
        exit(1);
    }

    // Get returned account number
    if((amt=read(sockfd, &retAccountNumber, sizeof(int))) != sizeof(int)){
        printf("%s get_balance failed to read retAccountNumber\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // Get returned balance
    if((amt=read(sockfd, &balance, sizeof(float))) != sizeof(float)){
        printf("%s get_balance failed to read balance\n.", clientStr);
        printf("It read %d bytes\n.", amt);
        exit(1);
    }

    printf("%s BALANCE for %d: %f\n", clientStr, retAccountNumber, balance);
}

// logic for sending ERROR
void error(int sockfd, int msgType){
    printf("Error message\n");
}

// logic for sending TERMINATE
void terminate(int sockfd){
    printf("Terminate request\n");

    // Integer to hold number of bytes read/written
    int amt = 0;

    // Variable to write to the socket
    int msg = TERMINATE;

    // Write TERMINATE message to server
    if((amt=write(sockfd, &msg, sizeof(int))) != sizeof(int)){
        printf("%s get_account_info failed to write msg_type\n.", clientStr);
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
}

// // logic for sending REQUEST_HISTORY
// void request_history(int sockfd, int accountNumber, int numTransactions){
//     printf("Request_history request\n");
// }

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

    struct timespec* start;
    struct timespec* end;
    clock_gettime(CLOCK_REALTIME, start);

    // Read in command-line arguments
    char* inputFileName = argv[1];
    char* serverIP = argv[2];
    int serverPort = atoi(argv[3]);

    // Set up socket variables
    int sockfd;
    struct sockaddr_in servaddr, cli;
    int noConnection = 1;
    bzero(&servaddr, sizeof(servaddr));

    // Assign IP and port number
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(serverIP);
    servaddr.sin_port = htons(serverPort);

    // Set up file I/O to be done in while loop
    FILE* fp;
    char *line = (char *)malloc(sizeof(char) * MAX_STR);
    size_t len = MAX_STR;
    ssize_t nread;
    if((fp = getFilePointer(inputFileName)) == NULL) {
        printf("Unable to open file: %s\n Exiting...\n", inputFileName);
        exit(EXIT_FAILURE);
    }
    
    // Set up variables to be read in during each read from the file
    int msg;
    int accountNumber;
    char name[MAX_STR];
    char username[MAX_STR];
    time_t birthday;
    float amount;
    int numTransactions;

    // Read from input file
    while((nread = getLineFromFile(fp, line, len)) != -1) {
        if(noConnection){
            // Create socket and validate
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if(sockfd == -1){
                printf("%s Socket creation failed!\n", clientStr);
                exit(0);
            } else {
                printf("%s Socket successfully created!\n", clientStr);
            }

            // Connect client socket to server port
            if(connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) != 0){
                printf("%s Connection with the server failed!\n", clientStr);
                exit(0);
            } else {
                printf("%s Connected to the server!\n", clientStr);
            }
            noConnection = 0;
        }

        sscanf(line, "%d,%d,%[^,],%[^,],%ld,%f,%d", &msg, &accountNumber, name, username, &birthday, &amount, &numTransactions);
        printf("%d,%d,%s,%s,%ld,%f,%d\n", msg, accountNumber, name, username, birthday, amount, numTransactions);

        // Depending on which command was read in, make request to server
        if(msg == REGISTER){
            registerRequest(sockfd, username, name, birthday);
        } else if(msg == GET_ACCOUNT_INFO){
            get_account_info(sockfd, accountNumber);
        } else if(msg == TRANSACT){
            continue; // TODO: REMOVE THIS 
            transact(sockfd, accountNumber, amount);
        } else if(msg == GET_BALANCE){
            get_balance(sockfd, accountNumber);
        } else if(msg == REQUEST_CASH){
            request_cash(sockfd);
        } else if(msg == ERROR){
            error(sockfd, msg);
        } else if(msg == TERMINATE){
            terminate(sockfd);
            close(sockfd);
            noConnection = 1;
        } else if(msg == REQUEST_HISTORY){
            request_history(sockfd, accountNumber, numTransactions);
        } else {
            printf("%s Invalid message type read. Continuing...\n", clientStr);
            continue;
        }
    }

    // If a connection still exists when done reading from file,
    // send a final terminate message and close the socket file descriptor
    if(!noConnection){
        terminate(sockfd);
        close(sockfd);
        noConnection = 1;
    }

    // Print out elapsed time
    clock_gettime(CLOCK_REALTIME, end);
    float timeDiff = (end->tv_sec - start->tv_sec) + 1e-9*(end->tv_nsec - start->tv_nsec);
    printf("Elapsed Time: %.2f\n", timeDiff);

    // Free up variables
    free(line);
    fclose(fp);

    return 0; 
}
