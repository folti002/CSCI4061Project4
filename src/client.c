#include "client.h"

#define SA struct sockaddr

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
    printf("usage: $ ./client input_filename server_addr server_port\n");
}

int main(int argc, char *argv[]){
    // Argument handling
    if(argc != 4){
        printSyntax();
        return 0;
    }

    // Read in command-line arguments
    char* inputFileName = argv[1];
    char* serverIP = argv[2];
    int serverPort = atoi(argv[3]);

    char* clientStr = "(CLIENT)";
    //printf("%s %s %s %d\n", clientStr, inputFileName, serverIP, serverPort);

    // Create socket and validate
    int sockfd, connfd;
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

    // INTERIM SUBMISSION
    // Send each enum to the server
    for(int i = REGISTER; i <= TERMINATE; i++){
        // Write current enum to the server
        char enumAsStr[sizeof(int)];
        sprintf(enumAsStr, "%d", i);
        if(write(sockfd, enumAsStr, strlen(enumAsStr)) < 0){
            perror("Failed to write");
            exit(1);
        }
        // printf("%s Enum %d sent to server.\n", clientStr, i);

        // Read server response
        char recv[sizeof(int)];
        memset(recv, 0, sizeof(int));
        if(read(sockfd, recv, sizeof(int)) < 0){
            perror("Failed to read");
            exit(1);
        }
        int enumVal = atoi(recv);
        printf("%s %s: %d\n", clientStr, msg_str[enumVal], enumVal);
    }

    close(sockfd);

    return 0; 
}
