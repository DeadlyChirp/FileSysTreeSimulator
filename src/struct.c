
#include "struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Implémentation des fonctions

//fonction permettant de créer le noeud racine 
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

noeud *trouverRacine(noeud *n){
    if (n -> pere == n) {
        return n;
    } else {
        return trouverRacine(n->pere);
    }
}

noeud *creerDossier(const char *nom, noeud *pere){
    noeud * n = malloc(sizeof(struct noeud));
    if(n == NULL){
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    n -> est_dossier = true;
    n -> pere = pere;
    strcpy(n->nom,nom);
    n -> racine = trouverRacine(n);
    n -> fils = NULL;

    return n;
}