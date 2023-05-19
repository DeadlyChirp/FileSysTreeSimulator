// main.c
#include "src/struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){

    noeud * root;
    root = initArbre();
    TraiterFichier(root, "testfiles/test1.txt");

    return EXIT_SUCCESS;
}
