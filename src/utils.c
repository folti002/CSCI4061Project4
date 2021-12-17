#include "utils.h"

// Return a pointer to the given file name
FILE* getFilePointer(char *inputFileName) {
    return fopen(inputFileName, "r");
}

// Get a line from a file
ssize_t getLineFromFile(FILE *fp, char *line, size_t len) {
    memset(line, '\0', len);
    return getline(&line, &len, fp);
}

// Remove the current output folder
void _removeOutputDir() {
    pid_t pid = fork();
    if(pid == 0){
        char *argv[] = {"rm", "-rf", "output", NULL};
        if (execvp(*argv, argv) < 0){
            printf("ERROR: exec failed\n");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);
    }
}

// Create a new output directory
void _createOutputDir(){
    mkdir("output", ACCESSPERMS);
}

// Delete output directory and make a new one
void bookeepingCode(){
    _removeOutputDir();
    sleep(1);
    _createOutputDir();
}