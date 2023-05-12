#include "struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Implémentation des fonctions

//fonction permettant de créer le noeud racine 
noeud *initArbre() {
    noeud *racine = malloc(sizeof(struct noeud));
    if (racine == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    racine->est_dossier = true;
    racine->pere = racine;
    racine->racine = racine;
    racine->fils = NULL;
    racine->nom[0] = '\0';

    return racine;
}

noeud *trouverRacine(noeud *n) {
    if (n->pere == n) {
        return n;
    } else {
        return trouverRacine(n->pere);
    }
}

noeud *creerNoeud(const char *nom, noeud *pere, bool estDossier) {
    noeud *n = malloc(sizeof(struct noeud));
    if (n == NULL) {
        printf("Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    n->est_dossier = estDossier;
    n->pere = pere;
    n->racine = trouverRacine(n);
    n->fils = NULL;

    if (strlen(nom) < 100) {
        strcpy(n->nom, nom);
    } else {
        printf("Erreur: le nom du noeud est trop long\n");
        exit(EXIT_FAILURE);
    }

    ajouterFils(pere, n);

    return n;
}

bool estDossier(noeud *n) {
    return n->est_dossier;
}

liste_noeud *initListeNoeud(noeud *n) {
    liste_noeud *liste = malloc(sizeof(*liste));

    if (liste == NULL || n == NULL) {
        exit(EXIT_FAILURE);
    }
    liste->no = n;
    liste->succ = NULL;

    return liste;
}


bool ajouterFils(noeud *parent, noeud *enfant) {
    if (!estDossier(parent) || parent == NULL) {
        return false;
    }
    
    liste_noeud *ln = initListeNoeud(enfant);
    
    if (parent->fils == NULL) {
        parent->fils = ln;
    } else {
        liste_noeud *tmp = parent->fils;
        while (tmp->succ != NULL) {
            tmp = tmp->succ;
        }
        tmp->succ = ln;
    }
    
    return true;
}

void afficher(noeud* courant, int profondeur) {
    for (int i = 0; i < profondeur; i++) {
        printf("|   ");
    }

    if (courant->nom[0] == '\0'){
        printf("Racine \n");
    } else {
        printf("%s\n", courant->nom);
    }

    if (courant->est_dossier && courant->fils != NULL) {
        liste_noeud* liste = courant->fils;
        while (liste != NULL) {
            afficher(liste->no, profondeur + 1);  
            liste = liste->succ;
        }
    }
}

// // décomposer en plusieurs fonctions:
// // -supprimerBranche (parcours toutes la branche et applique supprimerNoeud)
// // -supprimerNoeud (supprime individuellement le noeud sans chercher les fils)

// bool supprimerNoeud(noeud *n){
//     if(n == NULL) return false;
//     if(n -> pere != n){
//         noeud *pere = n->pere;
//         pere -> fils = NULL;
//     }
//     if (n->est_dossier){
//         if(n->fils != NULL){
//             free(n->fils);
//             n->fils = NULL;
//         }
//     }

//     free(n);
// }

// //supprime le noeud n et tous ses fils
// //A REVOIR NE FONCTIONNE PAS ENCORE
// bool supprimerBranche(noeud* n){
//     if(n == NULL){
//         return false;
//     }
//     if (n -> est_dossier){
//         if(n -> fils != NULL){
//             liste_noeud* liste = n->fils;
//             while (liste -> succ != NULL) {
//                 supprimer_noeud(liste->no);  
//                 liste = liste->succ;
//             }
//         }
//     }
//     if(n -> pere != NULL){
//         noeud* pere = n -> pere;
//         pere -> fils = NULL;  
//     } 
//     free(n);
// }


int main() {
    noeud *racine = initArbre();
    
    noeud *A1 = creerNoeud("A1", racine, true);
    noeud *A2 = creerNoeud("A2", racine, true);
    noeud *A3 = creerNoeud("A3", A1, true);
    noeud *A4 = creerNoeud("A4", A1, true);
    noeud *F1 = creerNoeud("F1", A2, false);

    afficher(racine, 0);

    return EXIT_SUCCESS;
}

