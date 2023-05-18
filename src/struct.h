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
noeud *initArbre(); //va initialiser l'arbre en créant le noeud racine et return le noeud racine
noeud *creerNoeud(const char *nom, noeud *pere, bool estDossier);
noeud *trouverRacine(noeud *n);
bool ajouterFils(noeud *parent, noeud *fils);
liste_noeud *initListeNoeud(noeud *n);
void afficher(noeud *racine, int profondeur);
bool supprimer(noeud *n);
bool deplacerNoeudCourant(noeud *n);
char **parsePath(const char* path);
noeud *trouverNoeud(const char *path);
bool bougerNoeud(noeud *n, noeud *nouveauPere);
void copierNoeud(noeud *n, noeud *nouveau);
#endif //PROJECT_CCC_STRUCT_H

//liste de commande à faire trouvé chez rayou "vidée" de celle déjà faites
// noeud *trouver_noeud(noeud *racine, const char *chemin);
// void executer_commandes(const char *chemin_fichier);
// Node* CreateNode(bool is_folder, char* name, Node* parent);
// void CopyNode(Node* root, char* src_path, char* dst_path);
// void MoveNode(Node* root, char* src_path, char* dst_path);
// Node* FindNode(Node* root, char* path);
// void ProcessFile(Node* root, char* filename);
// void ChangeDirectory(Node** current_Node, char* path);
// void ProcessInstruction(Node** current_Node, char* instruction);
// void PrintCurrentDirectory(Node* current_Node, int level);
// void PrintDirectory(Node* root, int level);

//liste de commande à faire imaginées/trouvées par moi pdt que je bossais donc je sais qu'elles vont être utiles
//trouverNoeud

//choses à résoudre!!!!!!!!!

//imo dernière commande est parsing de fichier d'input une fois que le reste fonctionne

//liste commande nécessaire
// 1. ls
// 2. cd chem
// 3. cd
// 4. cd ..
// 5. pwd
// 6. mkdir nom
// 7. touch nom
// 8. rm chem
// 9. cp chem1 chem2
// 10. mv chem1 chem2
// 11. print

