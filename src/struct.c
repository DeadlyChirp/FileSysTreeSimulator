
#include "struct.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Implémentation des fonctions
noeud *creer_noeud(bool est_dossier, const char *nom, noeud *pere) {
    noeud *n = (noeud *)malloc(sizeof(noeud));
    n->est_dossier = est_dossier;
    strncpy(n->nom, nom, sizeof(n->nom));
    n->nom[sizeof(n->nom) - 1] = '\0';
    n->pere = pere ? pere : n;
    n->racine = pere ? pere->racine : n;
    n->fils = NULL;

    if (pere) {
        ajouter_fils(pere, n);
    }

    return n;
}

void ajouter_fils(noeud *parent, noeud *fils) {
    liste_noeud *ln = (liste_noeud *)malloc(sizeof(liste_noeud));
    ln->no = fils;
    ln->succ = NULL;

    if (!parent->fils) {
        parent->fils = ln;
    } else {
        liste_noeud *courant = parent->fils;
        while (courant->succ) {
            courant = courant->succ;
        }
        courant->succ = ln;
    }
}


void afficher_arborescence(noeud *racine, int profondeur) {
    for (int i = 0; i < profondeur; ++i) {
        printf("  ");
    }

    printf("%s%s\n", racine->nom, racine->est_dossier ? "/" : "");

    liste_noeud *fils = racine->fils;
    while (fils) {
        afficher_arborescence(fils->no, profondeur + 1);
        fils = fils->succ;
    }
}

noeud *trouver_noeud(noeud *racine, const char *chemin) {
    if (strcmp(chemin, "/") == 0) {
        return racine;
    }

    char nom[100];
    const char *debut = chemin;
    noeud *noeud_courant = racine;

    while (*debut) {
        const char *fin = strchr(debut, '/');
        size_t longueur = fin ? (size_t)(fin - debut) : strlen(debut);

        strncpy(nom, debut, longueur);
        nom[longueur] = '\0';

        liste_noeud *fils = noeud_courant->fils;
        bool trouve = false;

        while (fils) {
            if (strcmp(fils->no->nom, nom) == 0) {
                noeud_courant = fils->no;
                trouve = true;
                break;
            }
            fils = fils->succ;
        }

        if (!trouve) {
            return NULL;
        }
        debut += longueur;
        if (fin) {
            debut++;
        }
    }

    return noeud_courant;
}

void executer_commandes(const char *chemin_fichier) {
    FILE *fichier = fopen(chemin_fichier, "r");
    if (!fichier) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    noeud *racine = creer_noeud(true, "", NULL);
    noeud *noeud_courant = racine;

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        char commande[10];
        char argument1[100];
        char argument2[100];

        int result = sscanf(ligne, "%s %s %s", commande, argument1, argument2);

        if (strcmp(commande, "mkdir") == 0 && result == 2) {
            creer_noeud(true, argument1, noeud_courant);
        } else if (strcmp(commande, "touch") == 0 && result == 2) {
            creer_noeud(false, argument1, noeud_courant);
        } else if (strcmp(commande, "cd") == 0) {
            if (result == 1) noeud_courant = racine; else if (result == 2) {
                noeud *n = trouver_noeud(noeud_courant, argument1);
                if (n && n->est_dossier) {
                    noeud_courant = n;
                }
            } else if (strcmp(commande, "cp") == 0 && result == 3) {
                noeud *src = trouver_noeud(noeud_courant->racine, argument1);
                noeud *dst = trouver_noeud(noeud_courant->racine, argument2);
                if (src && dst && dst->est_dossier) {
                    creer_noeud(src->est_dossier, src->nom, dst);
                }

            } else if (strcmp(commande, "mv") == 0 && result == 3) {
                noeud *src = trouver_noeud(noeud_courant->racine, argument1);
                noeud *dst = trouver_noeud(noeud_courant->racine, argument2);
                if (src && dst && dst->est_dossier) {
                    src->pere = dst;
                }
            }
            printf("\nArborescence après l'exécution de la commande '%s':\n", ligne);
            afficher_arborescence(racine, 0);
        }
        fclose(fichier);
        afficher_arborescence(racine, 0);
    }
}