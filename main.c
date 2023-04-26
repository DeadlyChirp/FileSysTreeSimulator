// main.c
#include "src/struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function implementations from src/struct.c should be pasted here
// ...
// (Paste all the functions from src/struct.c)

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <commands_file>\n", argv[0]);
        return 1;
    }

    executer_commandes(argv[1]);
    return 0;
}
