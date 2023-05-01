
#include "struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Implémentation des fonctions
noeud * initArbre(){

    noeud * n = malloc(sizeof(struct noeud));
    if(n == NULL){
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    n -> est_dossier = true;
    n -> pere = n;
    n -> racine = n;
    n -> fils = NULL;

    return n;
}