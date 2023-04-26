//
// Created by Admin on 4/26/2023.
//
#include <stdbool.h>
#ifndef PROJECT_CCC_STRUCT_H
#define PROJECT_CCC_STRUCT_H

#endif //PROJECT_CCC_STRUCT_H
typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;
struct noeud {
    bool est_dossier;
    char nom[100];
    noeud *pere;
    noeud *racine;
    liste_noeud *fils;
};

struct liste_noeud {
    noeud *no;
    liste_noeud *succ;
};


// Function declarations
noeud *creer_noeud(bool est_dossier, const char *nom, noeud *pere);
void ajouter_fils(noeud *parent, noeud *fils);
void afficher_arborescence(noeud *racine, int profondeur);
noeud *trouver_noeud(noeud *racine, const char *chemin);
void executer_commandes(const char *chemin_fichier);


#endif //PROJECT_CCC_STRUCT_H