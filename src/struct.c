
#include "struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Implémentation des fonctions

//fonction permettant de créer le noeud racine 
noeud * initArbre(){

    noeud * racine = malloc(sizeof(struct noeud));
    if(racine == NULL){
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    racine -> est_dossier = true;
    racine -> pere = racine;
    racine -> racine = racine;
    racine -> fils = NULL;

    return racine;
}

noeud *trouverRacine(noeud *n){
    if (n -> pere == n) {
        return n;
    } else {
        return trouverRacine(n->pere);
    }
}

noeud *creerNoeud(const char *nom, noeud *pere,bool estDossier){
    noeud * n = malloc(sizeof(struct noeud));
    if(n == NULL){
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    n -> est_dossier = estDossier;
    n -> pere = pere;
    strcpy(n->nom,nom);
    n -> racine = trouverRacine(n);
    n -> fils = NULL;

    return n;
}

bool estDossier(noeud *n){
    if(n->est_dossier) return true;
    return false;
}

liste_noeud *initListeNoeud(noeud *n){
    liste_noeud *liste = malloc(sizeof(*liste));

    if (liste == NULL || n == NULL){
        exit(EXIT_FAILURE);
    }
    liste -> no = n;
    liste -> succ = NULL;

    return liste;
}

bool ajouterFils(noeud *parent, noeud *enfant){
    if( !estDossier(parent) ) return false;
    if(parent -> fils == NULL){
        liste_noeud *ln = initListeNoeud(enfant);
        parent -> fils = ln;
        if (parent -> fils == NULL){
            return false;
        }
        return true;
    } else {
        noeud *tmp = parent;
        while (tmp -> fils -> succ != NULL){
            tmp -> fils = parent -> fils -> succ;
        }
        liste_noeud *ln = initListeNoeud(enfant);
        tmp -> fils -> succ = ln;
        return true;
    }
    return false;
}

// j'aimerai faire un afficher() qui fonctionne comme :
// racine
// | A1
// | | filsA1-1
// | | filsA1-2
// | | | filsfils...
// | A2 


int main(){
    noeud * nouveau  = initArbre();
    assert(nouveau -> racine == trouverRacine(nouveau));
    creerNoeud("A1", nouveau,true);
    return EXIT_SUCCESS;
}
