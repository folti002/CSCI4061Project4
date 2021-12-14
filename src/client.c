#include "client.h"

#define SA struct sockaddr


/*
CLIENT NEEDS TO SEND:
REGISTER (0)
GET_ACCOUNT_INFO (1)
TRANSACT (2)
GET_BALANCE (3)
REQUEST_CASH (6)
ERROR (8) - sent when client receives an unexpected response from the server. After sending this message to server, clients close the connection. 
TERMINATE (9) : Sending this message type (4 bytes) without any additional data
REQUEST_HISTORY (10)
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

// logic for handling TRANSACT
void transact(int acc_num, float val){
    // send a GET_BALANCE message to the server to ensure
    // that the account will not go negative

    // send GET_CASH to server until the cash variable 
    // (include/client.h) will not go negative

    // send TRANSACTION to the server

    // add the value of the transaction to the cash
    // variable (deposites increase, withdrawls decrease)

    // NOTE: make sure to use proper error handling
}

// and then reading a server's responding message
void get_account_info(int sock_fd){
    // integer to hold number of bytes read/written
    int amt = 0;

    // variables to write to the socket
    msg_enum msg_type = GET_ACCOUNT_INFO;
    int acc_num = 0;

    // variables to write the read response into
    msg_enum rsp_type;
    char name[MAX_STR];
    char username[MAX_STR];
    time_t birthday;
    
    // using read and write aren't the best "real world" call
    // but will be the simplest for you to use
    //
    // write the message type first
    if((amt=write(sock_fd, &msg_type, sizeof(msg_enum))) != sizeof(msg_enum)){
        printf("get_account_info failed to write msg_type\n.");
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    // write the arguments for the message type
    if((amt=write(sock_fd, &acc_num, sizeof(int))) != sizeof(int)){
        printf("get_account_info failed to write acc_num\n.");
        printf("It wrote %d bytes\n.", amt);
        exit(1);
    }
    
    // read in the response message type first
    // as this tells us what fields we will need
    // to read afterwards and their types
    if((amt=read(sock_fd, &rsp_type, sizeof(msg_enum))) != sizeof(msg_enum)){
        printf("get_account_info failed to read rsp_type\n.");
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // handle us getting the wrong value back
    else if(rsp_type != ACCOUNT_INFO){
        printf("get_account_info recieved wrong rsp_type\n");
        exit(1);
    }
    // get the username
    if((amt=read(sock_fd, &username, sizeof(char)*MAX_STR)) < 1){
        printf("get_account_info failed to read username\n.");
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // get the name
    if((amt=read(sock_fd, &name, sizeof(char)*MAX_STR)) < 1){
        printf("get_account_info failed to read name\n.");
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
    // get the birthday
    if((amt=read(sock_fd, &birthday, sizeof(time_t))) != sizeof(time_t)){
        printf("get_account_info failed to read birthday\n.");
        printf("It read %d bytes\n.", amt);
        exit(1);
    }
}

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

    // TODO: RECORD TIME OF START
    // time = clock.now; or something like that

    // Read in command-line arguments
    char* inputFileName = argv[1];
    char* serverIP = argv[2];
    int serverPort = atoi(argv[3]);

    char* clientStr = "(CLIENT)";

    // Create socket and validate
    int sockfd;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("%s Socket creation failed!\n", clientStr);
        exit(0);
    } else {
        printf("%s Socket successfully created!\n", clientStr);
    }
    bzero(&servaddr, sizeof(servaddr));

    // Assign IP and port number
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(serverIP);
    servaddr.sin_port = htons(serverPort);

    // Connect client socket to server port
    if(connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) != 0){
        printf("%s Connection with the server failed!\n", clientStr);
        exit(0);
    } else {
        printf("%s Connected to the server!\n", clientStr);
    }


    // Open a file and return file pointer to the file
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

    // Start reading from input file
    while((nread = getLineFromFile(fp, line, len)) != -1) {
        sscanf(line, "%d,%d,%s,%s,%ld,%f,%d\n", 
                &msg, &accountNumber, name, username, 
                &birthday, &amount, &numTransactions);
        printf("%d,%d,%s,%s,%ld,%f,%d\n", 
                msg, accountNumber, name, username, 
                birthday, amount, numTransactions);

        // Depending on which command was read in, make request to server
        if(msg == REGISTER){

        } else if(msg == GET_ACCOUNT_INFO){

        } else if(msg == TRANSACT){

        } else if(msg == GET_BALANCE){

        } else if(msg == REQUEST_CASH){

        } else if(msg == ERROR){

        } else if(msg == TERMINATE){

        } else if(msg == REQUEST_HISTORY){

        } else {
            // ERROR
        }
    }

    // TODO: PRINT OUT ELAPSED TIME AFTER CLIENT IS DONE RUNNING
    //printf("%s Elapsed Time: %.2f\n", clientStr, time);

    free(line);

    // INTERIM SUBMISSION
    // Send each enum to the server
    // for(int i = REGISTER; i <= TERMINATE; i++){
    //     // Write current enum to the server
    //     char enumAsStr[sizeof(int)];
    //     sprintf(enumAsStr, "%d", i);
    //     if(write(sockfd, enumAsStr, strlen(enumAsStr)) < 0){
    //         perror("Failed to write");
    //         exit(1);
    //     }
    //     // printf("%s Enum %d sent to server.\n", clientStr, i);
    //     // Read server response
    //     char recv[sizeof(int)];
    //     memset(recv, 0, sizeof(int));
    //     if(read(sockfd, recv, sizeof(int)) < 0){
    //         perror("Failed to read");
    //         exit(1);
    //     }
    //     int enumVal = atoi(recv);
    //     printf("%s %s: %d\n", clientStr, msg_str[enumVal], enumVal);
    // }

    close(sockfd);

    return 0; 
}
