#include "struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


//Variable global
//à partir du moment où l'arbre est créé noeudCourant = racine
noeud* noeudCourant;

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

    noeudCourant = racine;

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

    if (strlen(nom) < 100 && nom[0] != '\0') {
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
        printf("| ");
    }

    if (courant->nom[0] == '\0'){
        printf("\033[32mRacine\033[0m\n");
    } else {
        if(courant->est_dossier){
            printf("\033[32m%s\033[0m\n", courant->nom);
        } else {
            printf("%s\n", courant->nom);
        }
    }

    if (courant->est_dossier && courant->fils != NULL) {
        liste_noeud* liste = courant->fils;
        while (liste != NULL) {
            afficher(liste->no, profondeur + 1);  
            liste = liste->succ;
        }
    }
}

bool supprimer(noeud *n){
    if(n == noeudCourant) noeudCourant = trouverRacine(noeudCourant);
    if(n == NULL) return false;
    
    if (n->est_dossier){
        while(n -> fils != NULL){
            supprimer(n -> fils -> no);
        }
    }

    if(n -> pere != n){
        noeud *pere = n->pere;
        liste_noeud * enfant = pere -> fils;
        liste_noeud * prev = NULL;
        while(enfant != NULL && enfant -> no != n){
            prev = enfant;
            enfant = enfant -> succ;
        }
        if (enfant != NULL) {
            if (prev == NULL) {
                pere->fils = enfant->succ;
            } else {
                prev->succ = enfant->succ;
            }
            free(enfant);
        }
    }

    free(n);
    return true;
}

//check les memory leaks
noeud *trouverNoeud(const char *path){
    bool found = false;
    noeud *n = noeudCourant;
    char **pathTab = parsePath(path);
    if(path[0] == '\0'){
        n = trouverRacine(n);
        found = true;
        return n;
    }
    if(path[0] == '/'){
        n = trouverRacine(noeudCourant);
    }
    int i = 0;
    while(pathTab[i] != NULL){
        if(n->est_dossier && n->fils != NULL){
            liste_noeud *liste = n->fils;
            while(liste != NULL){
                // printf("%s\n", pathFolders[i]);
                if(strcmp(pathTab[i],"..")  == 0){
                    if(n->pere != n ) {
                        n = n->pere;
                        found = true;   
                        i++;
                        break;
                    } else {
                        printf("Le noeud courant est déjà la racine\n");
                        return NULL;
                    }
                }
                if(strcmp(pathTab[i],".")  == 0){
                    i++;
                    found = true;
                    break;
                }   
                if(strcmp(liste->no->nom, pathTab[i]) == 0){
                    n = liste->no;
                    found = true;
                    i++;
                    break;
                } else { 
                    liste = liste->succ;
                }
            }
            if(found == false){
                printf("Le noeud %s n'existe pas\n", pathTab[i]);
                return NULL;
            }
        } else {
            printf("un ndes noeuds du chemin n'est pas un dossier\n");
            return NULL;
        }
    } 
    return n;
}

bool deplacerNoeudCourant(noeud *n){
    if(n == NULL) return false;
    noeudCourant = n;
    printf("Le noeud courant est maintenant %s\n", noeudCourant->nom);
    return true;
}

char **parsePath(const char* path){
    int capacity = 10; // Capacité initiale du tableau
    char** pathFolders = (char**)malloc(capacity * sizeof(char*));
    int count = 0;

    // Copie la chaîne de caractères pour la modification
    char* pathCopy = strdup(path);

    // Découpe la chaîne en utilisant '/' comme délimiteur
    const char *delim = "/";
    char* token = strtok(pathCopy, delim);
    while (token != NULL) {
        // Vérifie si le tableau a atteint sa capacité maximale, le redimensionne si nécessaire
        if (count == capacity) {
            capacity *= 2;
            pathFolders = (char**)realloc(pathFolders, capacity * sizeof(char*));
        }

        // Alloue de la mémoire pour stocker le nom du dossier et le copie
        pathFolders[count] = strdup(token);
        count++;
        // Passe au token suivant
        token = strtok(NULL, "/");
        // printf("while reached after strtok\n");
    }

    // Libère la mémoire utilisée pour la copie de la chaîne
    free(pathCopy);
    
    return pathFolders;
}

bool bougerNoeud(noeud *n, noeud *nouveauPere){
    if(nouveauPere == NULL || n == NULL || n == nouveauPere || n -> pere == nouveauPere || n -> pere == n || nouveauPere -> est_dossier == false) 
        return false;

    liste_noeud *liste = nouveauPere -> fils;
    if(liste == NULL) 
        return false;

    liste_noeud *pre = n->pere->fils;
    if(pre == NULL) 
        return false;
        
    if(pre->no == n) {
        n->pere->fils = NULL;
    } else {
        while(pre->succ != NULL && pre->succ->no != n){
            pre = pre->succ;
        }
        if(pre->succ != NULL) 
            pre->succ = pre->succ->succ;
    }

    while(liste->succ != NULL){
        liste = liste -> succ;
    }

    printf("t14\n");
    liste -> succ = (liste_noeud*)malloc(sizeof(liste_noeud));
    if(liste -> succ == NULL) // Check if malloc was successful
        return false;
        
    liste -> succ -> no = n; 
    printf("t15\n");
    n -> pere = nouveauPere;
    printf("t16\n");
    return true;
}

void printPath(char** path, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s", path[i]);
        if (i < count - 1) {
            printf("/");
        }
    }
    printf("\n");
}



int main() {
    noeud *racine = initArbre();
    
    //création d'un arbre de test de profondeur 5
    noeud *A1 = creerNoeud("A1", racine, true);
    noeud *A2 = creerNoeud("A2", racine, true);
    noeud *A3 = creerNoeud("A3", A1, true);
    noeud *A4 = creerNoeud("A4", A1, true);
    noeud *F1 = creerNoeud("F1", A2, false);
    noeud *F2 = creerNoeud("F2", A2, false);
    noeud *F3 = creerNoeud("F3", A2, false);
    //création fils de A3 et A4
    noeud *A5 = creerNoeud("A5", A3, true);
    noeud *A6 = creerNoeud("A6", A3, true);
    noeud *A7 = creerNoeud("A7", A4, true);
    //fils de A5
    noeud *F4 = creerNoeud("F4", A5, false);
    //fils de A6
    noeud *F5 = creerNoeud("F5", A6, false);

    afficher(racine, 0);

   
    printf("test 1:");
    // deplacerNoeudCourant("A1/A3/");   
    deplacerNoeudCourant(trouverNoeud("/A1/A3/A5"));

    printf("test 2: \n");
    
    bougerNoeud(A2, A3);

    afficher(racine, 0);

    // printf("test 2:");
    // deplacerNoeudCourant("/A2"); 

    // printf("test 3:");
    // deplacerNoeudCourant("/A1/A3/A6");

    // // // deplacerNoeudCourant("/A1/A3/A6"); 
    // printf("test 4:");
    // deplacerNoeudCourant("../../../A2"); 

    // printf("test 5:");
    // deplacerNoeudCourant("../../A2");
    // afficher(noeudCourant, 0);
    // supprimer(noeudCourant);
    // printf("noeud courant : %s\n", noeudCourant->nom);
    // printf("la branche a été supprimé : %d \n", supprimerBranche(A1));
    // supprimerNoeud(A4);
    // afficher(racine, 0);
    // supprimerNoeud(A3);
    // afficher(racine, 0);
    // supprimerNoeud(A1);

    // afficher(racine, 0);
    // supprimerBrancheEtNoeud(A1);

    // printf("la branche a été supprimé : %d \n", supprimer(A1));

    // afficher(racine,0);


    return EXIT_SUCCESS;
}

