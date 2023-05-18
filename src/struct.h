#include <stdbool.h>
#ifndef PROJECT_CCC_STRUCT_H
#define PROJECT_CCC_STRUCT_H

struct noeud ;
struct liste_noeud ;

#define MAX_LONGUEUR_CHEMIN 100

struct noeud {
    bool est_dossier ;
    char nom [100];
    struct noeud * pere ;
    struct noeud * racine ;
    struct liste_noeud * fils ;
};

struct liste_noeud {
    struct noeud * no ;
    struct liste_noeud * succ ;
};

typedef struct noeud noeud ;

typedef struct liste_noeud liste_noeud ;

// Function declarations
noeud *trouverRacine(noeud *n);
noeud *initArbre(); //va initialiser l'arbre en créant le noeud racine et return le noeud racine
noeud *creerNoeud(const char *nom, noeud *pere, bool estDossier);
bool ajouterFils(noeud *parent, noeud *fils);
liste_noeud *initListeNoeud(noeud *n); //mettre le "if == null"

void afficher(noeud *racine, int profondeur); // = imprimerArbre(aide)

noeud *trouverNoeud(const char *path); //mon trouver noeud mais fusionné avec parsePath
void copierNoeud(noeud *n, noeud *nouveau);
bool bougerNoeud(noeud *n, noeud *nouveauPere);
void changerDossier(char *chemin);   
bool supprimer(noeud *n); 

void imprimerDosser(char *chemin); //ls
int longueurListe(liste_noeud *liste);
void imprimerArbreAide(noeud *noeud, int profondeur); //print
void imprimerArbre(); //print
void traiterfichier(noeud *racine, char *nomFichier); //processfile

//trouver racine à voir
#endif //PROJECT_CCC_STRUCT_H
