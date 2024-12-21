#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>

void handleGetRequest(char *filePath) {
    char tempBuffer[1024];
    if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL) {
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }

    char tempChar;
    int itemsScanned = sscanf(tempBuffer, "%c", &tempChar);

    if (itemsScanned > 0 && (tempChar != '\n' && !isspace(tempChar))) {
        // Found a non-whitespace character before newline.
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }
    if (strlen(filePath) >= PATH_MAX) {
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }

    if (access(filePath, F_OK) != 0) {
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }

    int fileDescriptor = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fileDescriptor < 0) {
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }
    char readBuffer[1024];
    ssize_t bytesToRead;
    while ((bytesToRead = read(fileDescriptor, readBuffer, 1024)) > 0) {
        write(fileDescriptor, readBuffer, bytesToRead);
    }
    close(fileDescriptor);
    exit(0);
}

void handleSetRequest(char *filePath, int dataSize, char *data) {
    int fileDescriptor = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fileDescriptor < 0) {
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }
    if (write(fileDescriptor, data, dataSize) < dataSize) {
        fprintf(stderr, "Invalid Command\n");
        close(fileDescriptor);
        exit(1);
    }
    close(fileDescriptor);
    printf("OK\n");
    exit(0);
}

int main(void) {
    char operation[5], filePath[256];
    int dataSize;

    if (scanf(" %4s", operation) != 1) {
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }

    if (strcmp(operation, "get") == 0) {
        if (scanf(" %255s", filePath) != 1) {
            fprintf(stderr, "Invalid Command\n");
            exit(1);
        }
        handleGetRequest(filePath);
    } else if (strcmp(operation, "set") == 0) {
        if (scanf(" %255s %d ", filePath, &dataSize) != 2) {
            fprintf(stderr, "Invalid Command\n");
            exit(1);
        }

        char *data = malloc(dataSize + 1);
        if (data == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        ssize_t bytesToRead = read(STDIN_FILENO, data, dataSize);
        if (bytesToRead != dataSize) {
            // Handle incomplete read here if needed
        }

        handleSetRequest(filePath, dataSize, data);
        free(data);
    } else {
        fprintf(stderr, "Invalid Command\n");
        exit(1);
    }
    return 0;
}
