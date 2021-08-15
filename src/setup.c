// setup.c
// Ashish D'Souza

#include <stdio.h>
#include "python_setup.h"
#include "setup.h"


int main(int argc, char **argv) {
    if (argc != 4) {
        printf("Invalid number of positional arguments");
        return 1;
    }

    if (pythonSetup(argv[1])) return 1;

    return 0;
}

