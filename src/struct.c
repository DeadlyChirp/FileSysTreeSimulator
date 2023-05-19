#include "struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


noeud *noeudCourant;


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

char *strdup(const char *str) {
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup != NULL) {
        strcpy(dup, str);
    }
    return dup;
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



liste_noeud *initListeNoeud(noeud *n) {
    liste_noeud *liste = malloc(sizeof(*liste));

    if (liste == NULL || n == NULL) {
        exit(EXIT_FAILURE);
    }
    liste->no = n;
    liste->succ = NULL;

    return liste;
}
noeud *trouverRacine(noeud *n) {
    if (n->pere == n) {
        return n;
    } else {
        return trouverRacine(n->pere);
    }
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

    bool found;
    noeud *n;

    if (path[0] == '/') {
        n = trouverRacine(noeudCourant);
    } else {
        n = noeudCourant;
    }

    if(path[0] == '\0'){
        n = trouverRacine(n);
        return n;
    }

    int i = 0;
    while(pathFolders[i] != NULL){
        found = false;
        if(n->est_dossier && n->fils != NULL){
            liste_noeud *liste = n->fils;
            while(liste != NULL){
                if(strcmp(pathFolders[i],"..")  == 0){
                    if(n->pere != n ) {
                        n = n->pere;
                        found = true;
                    } else {
                        printf("Le noeud courant est déjà la racine\n");
                        // returning NULL but also need to free pathFolders before doing so
                        for(int j = 0; j < count; j++)
                            free(pathFolders[j]);
                        free(pathFolders);
                        return NULL;
                    }
                }
                else if(strcmp(pathFolders[i],".")  == 0){
                    found = true;
                }
                else if(strcmp(liste->no->nom, pathFolders[i]) == 0){
                    n = liste->no;
                    found = true;
                }

                if (found)
                    break;
                else
                    liste = liste->succ;
            }
            if(!found){
//                printf("Le noeud %s n'existe pas\n", pathFolders[i]);
                for(int j = 0; j < count; j++)
                    free(pathFolders[j]);
                free(pathFolders);
                return NULL;
            }
        } else {
//            printf("un des noeuds du chemin n'est pas un dossier\n");
            for(int j = 0; j < count; j++)
                free(pathFolders[j]);
            free(pathFolders);
            return NULL;
        }
        i++;
    }

    for(int j = 0; j < count; j++)
        free(pathFolders[j]);
    free(pathFolders);
    return n;
}
void ChangerDossier(noeud *n){
    if(n == NULL) return;
    printf("Changement de dossier courant\n");
    noeudCourant = n;
    printf("Dossier courant : %s\n", noeudCourant->nom);
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
void copierNoeud(noeud *n, noeud *nouveau){
    if(n == NULL || nouveau == NULL || n == nouveau || n -> pere == nouveau || n -> pere == n || nouveau -> est_dossier == false){
        printf("t1\n");
        return;
    }
    if (n->est_dossier) {
        if(n->fils != NULL){
            liste_noeud *liste = n->fils;
            while(liste != NULL){
                noeud *copie = creerNoeud(liste->no->nom, nouveau, liste->no->est_dossier);
                if(liste->no->est_dossier) {
                    copierNoeud(liste->no, copie);
                }
                liste = liste->succ;
            }
        }
    }
}


//faire fonction Estdescendant
bool estDescendant(noeud * racine, noeud * n){
    if(n == NULL){
        return false;
    }
    if (n == racine){
        return true;
    }
    return estDescendant(racine,n->pere);
}


void copierEtCreer(noeud *n, const char *path){
    // If the source node does not exist, return immediately
    if (n == NULL) {
        printf("Source does not exist.\n");
        return;
    }

    char * copiepath = malloc(sizeof (char )* strlen(path)+sizeof (char)) ;
    strcpy(copiepath , path) ;

    // Attempt to find the node at the specified path
    noeud *nouveau = trouverNoeud(path);

    // If the node does not exist, create it
    if (nouveau == NULL) {
        // Get parent directory
        char* lastSlash = strrchr(copiepath, '/');
        noeud *parent = NULL;

        if (lastSlash != NULL) {
            *lastSlash = '\0';  // Temporarily ends the string at the last slash

            if (path[0] != '\0') {
                parent = trouverNoeud(copiepath);  // Use root as the parent
            } else {
                parent = n->racine;  // If path is empty, use the current node as the parent
            }

            *lastSlash = '/';  // Restores the original string
        } else {
            // If path does not contain a slash, use the current node as the parent
            parent = noeudCourant;
        }
        if (parent == NULL) {
            printf("Parent directory does not exist.\n");
            free(copiepath) ;
            return;
        }

        if (!parent->est_dossier) {
            //
            free(copiepath) ;
            return;
        }
        if(estDescendant(n,parent)){
            printf("\nOn ne peut pas copier le parent du dossier/fichier\n");
            free(copiepath);
        }
        // Create the new node under the parent
        const char* nodeName = NULL ;
        if (lastSlash == NULL) {
            nodeName = path ;
        } else{
            nodeName = lastSlash+1;
        }
        nouveau = creerNoeud(nodeName, parent, true);
    }else{
        //
        free(copiepath) ;
        return;
    }

    // verifier que la source n'est pas un ancetre de parent copie (estParent)

    // If both nodes are directories, copy the contents of the source to the destination
    if (n->est_dossier && nouveau->est_dossier) {
        copierNoeud(n, nouveau);
    }
        // If the source is a file and the destination is a directory, create a copy of the file in the destination
    else if (!n->est_dossier && nouveau->est_dossier) {
        creerNoeud(n->nom, nouveau, false);
    }
        // If the source is a file and the destination is a file, overwrite the destination file with the source
    else if (!n->est_dossier && !nouveau->est_dossier) {
        // Simply renaming the destination node would be equivalent to overwriting the file
        strcpy(nouveau->nom, n->nom);
    }
    free(copiepath) ;
}






bool supprimer(noeud *n){
    if(n == noeudCourant) {
        printf("Vous ne pouvez pas supprimer le dossier dans le quel vous vous situez\n");
        exit(EXIT_FAILURE);
    }
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
    if (node->pere != NULL && node->pere != node) {
        ImprimerDossierCourantHelper(node->pere);
        printf("/%s", node->nom);  // Only print '/' before non-root nodes
    }
    else if (node->nom[0] != '\0') { // If the node name is not empty, print it
        printf("%s", node->nom);
    }
}


void ImprimerPWD() { // Equivalent à pwd
    printf("----------------- pwd -----------------\n");
    if (noeudCourant->nom[0] == '\0') {
        printf("/");
    } else {
        ImprimerDossierCourantHelper(noeudCourant);
    }
    printf("\n--------------------------------------\n");
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
//PROF PRINT
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

    if (noeud->est_dossier && noeud->fils != NULL) {
        liste_noeud* liste = noeud->fils;
        while (liste != NULL) {
            ImprimerArbreAide(liste->no, profondeur + 1);
            liste = liste->succ;
        }
    }
}

void resetNoeudCourant(){
    noeudCourant = NULL;
}

void ImprimerArbre(){
    printf("\n---------------- print ---------------\n");
    ImprimerArbreAide(trouverRacine(noeudCourant),0);
    //afficher(trouverRacine(noeudCourant), 0);
    printf("\n--------------------------------------\n");
}


void TraiterFichier(noeud * racine, char* nomFichier) {
    FILE *file = fopen(nomFichier, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        char* token = strtok(line, " ");
        if (strcmp(token, "mkdir") == 0) {
            token = strtok(NULL, " ");
            creerNoeud(token, noeudCourant, true);
        } else if (strcmp(token, "cd") == 0) {
            token = strtok(NULL, " ");

            if (token == NULL) {
                noeudCourant = racine;
            } else {
                noeud *nouveau = trouverNoeud(token);
                if (nouveau != NULL) {
                    noeudCourant = nouveau;
                }
            }
        } else if (strcmp(token, "touch") == 0) {
            token = strtok(NULL, " ");
            creerNoeud(token, noeudCourant, false);
        } else if (strcmp(token, "cp") == 0) {
            token = strtok(NULL, " ");
            char* token2 = strtok(NULL, " ");
            noeud *noeudCopier = trouverNoeud(token);
            copierEtCreer(noeudCopier, token2);
        } else if (strcmp(token, "mv") == 0) {
            token = strtok(NULL, " ");
            char* token2 = strtok(NULL, " ");
            noeud *noeudBouger = trouverNoeud(token);
            noeud *nouveauPere = trouverNoeud(token2);
            bougerNoeud(noeudBouger, nouveauPere);
        } else if (strcmp(token, "rm") == 0) {
            token = strtok(NULL, " ");
            noeud *noeudSupprimer = trouverNoeud(token);
            supprimer(noeudSupprimer);
        } else if (strcmp(token, "ls") == 0) {
            ImprimerDossier(noeudCourant);
        } else if (strcmp(token, "pwd") == 0) {
            ImprimerPWD();
            printf("\n");
        } else if (strcmp(token, "print") == 0) {
            ImprimerArbre();
        }
    }

    fclose(file);
}



