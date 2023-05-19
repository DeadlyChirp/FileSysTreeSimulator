#include "struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

noeud *noeudCourant;
// // Function declarations


// void changerDossier(char *chemin);   

// void imprimerDosser(char *chemin); //ls
// int longueurListe(liste_noeud *liste);
// void imprimerArbreAide(noeud *noeud, int profondeur); //print
// void imprimerArbre(); //print
// void traiterfichier(noeud *racine, char *nomFichier); //processfile


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
        printf("Erreur: le nom du noeud est trop long ou pas de nom\n");
        exit(EXIT_FAILURE);
    }

    ajouterFils(pere, n);

    return n;
}


bool ajouterFils(noeud *parent, noeud *enfant) {
    if (!parent -> est_dossier || parent == NULL) {
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

liste_noeud *initListeNoeud(noeud *n) {
    liste_noeud *liste = malloc(sizeof(*liste));

    if (liste == NULL || n == NULL) {
        exit(EXIT_FAILURE);
    }
    liste->no = n;
    liste->succ = NULL;

    return liste;
}

noeud *trouverNoeud(const char *path){
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
    
    bool found = false;
    noeud *n = noeudCourant;
    
    if(path[0] == '\0'){
        n = trouverRacine(n);
        found = true;
        return n;
    }
    if(path[0] == '/'){
        n = trouverRacine(noeudCourant);
    }
    int i = 0;
    while(pathFolders[i] != NULL){
        if(n->est_dossier && n->fils != NULL){
            liste_noeud *liste = n->fils;
            while(liste != NULL){
                // printf("%s\n", pathFolders[i]);
                if(strcmp(pathFolders[i],"..")  == 0){
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
                if(strcmp(pathFolders[i],".")  == 0){
                    i++;
                    found = true;
                    break;
                }   
                if(strcmp(liste->no->nom, pathFolders[i]) == 0){
                    n = liste->no;
                    found = true;
                    i++;
                    break;
                } else { 
                    liste = liste->succ;
                }
            }
            if(found == false){
                printf("Le noeud %s n'existe pas\n", pathFolders[i]);
                return NULL;
            }
        } else {
            printf("un des noeuds du chemin n'est pas un dossier\n");
            return NULL;
        }
    } 
    return n;
}
void ChangerDossier(noeud *n){
    if(n == NULL) return;
    noeudCourant = n;
    printf("Le noeud courant est maintenant %s\n", noeudCourant->nom);
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

    liste -> succ = (liste_noeud*)malloc(sizeof(liste_noeud));
    if(liste -> succ == NULL) // Check if malloc was successful
        return false;
        
    liste -> succ -> no = n; 
    n -> pere = nouveauPere;
    return true;
}

void copierNoeud(noeud *n, noeud * nouveau){
    if(n == NULL 
    || nouveau == NULL 
    || n == nouveau 
    || n -> pere == nouveau 
    || n -> pere == n 
    || nouveau -> est_dossier == false){
        printf("t1\n");
        return;
    } 
    
    noeud *copie = creerNoeud(n->nom, nouveau, n->est_dossier);

    if(copie == NULL){
        return;
    }

    if (n->est_dossier) {
        if(n->fils != NULL){
            liste_noeud *liste = n->fils;
            while(liste != NULL){
                if(!liste->no->est_dossier)
                    ajouterFils(copie,liste->no);
                if(liste->no->est_dossier)
                    copierNoeud(liste->no, copie);
                liste = liste->succ;
            }
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

void ImprimerDossierCourantHelper(noeud * node) {
    if (node->pere != NULL) {
        ImprimerDossierCourantHelper(node->pere);
    }
    printf("/%s", node->nom);
}

void ImprimerPWD() { // Equivalent à pwd
    printf("----------------- pwd -----------------\n");
    if (noeudCourant->nom[0] == '\0') {
        printf("/");
    } else {
        ImprimerDossierCourantHelper(noeudCourant);
    }
    printf("\n----------------- pwd -----------------\n");
}

void ImprimerDossier(noeud * dossier) { // Equivalent à ls qui imprime egalement le dossier courant . ou dossier parent ..
    liste_noeud * currentChild = dossier->fils;
    printf("----------------- ls -----------------\n");
    while (currentChild) {
        printf("%s ", currentChild->no->nom);
        currentChild = currentChild->succ;
    }
    printf("\n--------------------------------------\n");
}

int LongueurListe(liste_noeud * liste) {
    int length = 0;
    while (liste) {
        length++;
        liste = liste->succ;
    }
    return length;
}

//void ImprimerArbreAide(noeud *noeud, int profondeur) {
//    for (int i = 0; i < profondeur; i++) {
//        printf("=");
//    }
//    printf("Niveau %d %s %s (%s), pere: %s, %d fils\n", profondeur, noeud->est_dossier ? "Dossier" : "Fichier", noeud->nom, noeud->est_dossier ? "Dossier" : "Fichier", noeud->pere ? noeud->pere->nom : "", LongueurListe(noeud->fils));
//
//    if (noeud->est_dossier) {
//        liste_noeud *currentChild = noeud->fils;
//        while (currentChild) {
//            ImprimerArbreAide(currentChild->no, profondeur + 1);
//            currentChild = currentChild->succ;
//        }
//    }
//}
//
//
//
//
//void ImprimerArbre() {
//    printf("----------------- print -----------------\n");
//    ImprimerArbreAide(noeudActuel, 0);
//    printf("-----------------------------------------\n");
//}
void ImprimerArbreAide(noeud* noeud, int profondeur) {
    for (int i = 0; i < profondeur; i++) {
        printf("=");
    }

    printf("niveau %d ", profondeur);
    if (noeud->est_dossier) {
        printf("dossier ");
    } else {
        printf("fichier ");
    }
    printf("%s : %d fils -> pere ", noeud->nom, LongueurListe(noeud->fils));
    if (noeud->pere) {
        printf("%s", noeud->pere->nom);
    } else {
        printf("NULL");
    }
    printf("\n");

    if (noeud->est_dossier) {
        liste_noeud *currentChild = noeud->fils;
        while (currentChild) {
            ImprimerArbreAide(currentChild->no, profondeur + 1);
            currentChild = currentChild->succ;
        }
    }
}

void ImprimerArbre() {
    ImprimerArbreAide(noeudCourant, 0);
}


void TraiterFichier(noeud * racine, char* nomFichier) {
    FILE* file = fopen(nomFichier, "r");
    if (!file) {  // Check if the file is opened successfully
        printf("Unable to open file: %s\n", nomFichier);
        return;
    }
    char instruction[MAX_LONGUEUR_CHEMIN];
    char arg1[MAX_LONGUEUR_CHEMIN];
    char arg2[MAX_LONGUEUR_CHEMIN];
    noeudCourant = racine;
    while (fscanf(file, "%s", instruction) != EOF) {
        if (strcmp(instruction, "mkdir") == 0) {
            fscanf(file, "%s", arg1);
            AjouterNoeud(true, arg1);
        } else if (strcmp(instruction, "cd") == 0) {
            if (fscanf(file, "%s", arg1) != EOF) {
                // If the next string is not another command
                if (strcmp(arg1, "mkdir") != 0 && strcmp(arg1, "cd") != 0 && strcmp(arg1, "touch") != 0 &&
                    strcmp(arg1, "cp") != 0 && strcmp(arg1, "mv") != 0 && strcmp(arg1, "rm") != 0 &&
                    strcmp(arg1, "ls") != 0 && strcmp(arg1, "pwd") != 0 && strcmp(arg1, "print") != 0) {
                    ChangerDossier(trouverNoeud(arg1)); //TODO A REVOIR
                } else {
                    // si il n'y a pas d'argument, on revient au dossier racine
                    while (noeudCourant->pere) {
                        noeudCourant = noeudCourant->pere;
                    }
                    // On doit revenir en arrière dans le fichier pour pouvoir traiter la prochaine commande
                    fseek(file, -strlen(arg1), SEEK_CUR);
                }
            }
        } else if (strcmp(instruction, "touch") == 0) {
            fscanf(file, "%s", arg1);
            ajouterFils(false, arg1); //TODO A REVOIR
        } else if (strcmp(instruction, "cp") == 0) {
            fscanf(file, "%s %s", arg1, arg2);
            copierNoeud(trouverNoeud(arg1), trouverNoeud(arg2)); //TODO A REVOIR
        } else if (strcmp(instruction, "mv") == 0) {
            fscanf(file, "%s %s", arg1, arg2);
            noeud *NoeudArg1 = arg1[0] == '/' ? racine : noeudCourant; // si le chemin commence par /, on commence à la racine, sinon on commence au noeud actuel
            noeud *NoeudArg2 = arg2[0] == '/' ? racine : noeudCourant;
            noeud *node1 = trouverNoeud( arg1[0] == '/' ? arg1 + 1 : arg1); // si le chemin commence par /, on ignore le premier caractère, sinon on garde tout le chemin
            noeud *node2 = trouverNoeud( arg2[0] == '/' ? arg2 + 1 : arg2);
            if (node1 && node2) { // si les deux noeuds existent, on peut les bouger
                bougerNoeud(node1, node2);
            }
        } else if (strcmp(instruction, "rm") == 0) {
            fscanf(file, "%s", arg1);
            noeud * startNode = arg1[0] == '/' ? racine : noeudCourant;
            noeud * node = trouverNoeud( arg1[0] == '/' ? arg1 + 1 : arg1);
            if (node) {
                supprimer(node);
            }
        } else if (strcmp(instruction, "ls") == 0) {
            ImprimerDossier(noeudCourant);
        } else if (strcmp(instruction, "pwd") == 0) {
            ImprimerPWD();
            printf("\n");
        } else if (strcmp(instruction, "print") == 0) {
            ImprimerArbre();
        }
    }
    fclose(file);
}

