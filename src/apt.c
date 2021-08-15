// apt.c
// Ashish D'Souza

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "apt.h"


int updatePackageSourcesList() {
    int status = system("apt-get update -y > /dev/null 2>&1");
    if (status) {
        printf("Failed to update package sources list with \"apt-get update\"\n");
        return 1;
    }
}

int installPackages(int nPackages, char **packages) {
    printf("Installing system packages:");
    char *prefix = "apt-get install -y";
    char *command = malloc(19);
    if (command == NULL) {
        printf("Failed to allocate space for string\n");
        return 1;
    }
    strcpy(command, prefix);

    for (unsigned int i = 0; i < nPackages; i++) {
        printf(" %s", packages[i]);
        char *commandBuffer = (char*) realloc(command, strlen(command) + 2 + strlen(packages[i]));
        if (commandBuffer == NULL) {
            printf("Failed to allocate space for string\n");
            free(command);
            return 1;
        }
        command = commandBuffer;
        strcpy(command + strlen(command) + 1, packages[i]);
        *(command + strlen(command)) = ' ';
    }

    printf("\n");
    fflush(stdout);

    char *commandBuffer = (char*) realloc(command, strlen(command) + 20);
    if (commandBuffer == NULL) {
        printf("Failed to allocate space for string\n");
        free(command);
        return 1;
    }
    command = commandBuffer;
    strcpy(command + strlen(command), " > /dev/null 2>&1");

    int status = system(command);
    if (status) {
        printf("Failed to install system packages\n");
    }
    free(command);
    return status;
}