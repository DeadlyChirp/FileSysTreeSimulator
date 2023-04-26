//
// Created by Admin on 4/26/2023.
//
#include <stdbool.h>
#ifndef PROJECT_CCC_STRUCT_H
#define PROJECT_CCC_STRUCT_H


struct noeud ;
struct liste_noeud ;
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
noeud *creer_noeud(bool est_dossier, const char *nom, noeud *pere);
void ajouter_fils(noeud *parent, noeud *fils);
void afficher_arborescence(noeud *racine, int profondeur);
noeud *trouver_noeud(noeud *racine, const char *chemin);
void executer_commandes(const char *chemin_fichier);
void supprimer_noeud(noeud* n);

#endif //PROJECT_CCC_STRUCT_H