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
bool supprimerNoeud(noeud *n);
bool supprimerBranche(noeud *n);

#endif //PROJECT_CCC_STRUCT_H

//liste de commande à faire trouvé chez rayou
// noeud *trouver_noeud(noeud *racine, const char *chemin);
// void executer_commandes(const char *chemin_fichier);
// Node* CreateNode(bool is_folder, char* name, Node* parent);
// void DeleteNode(Node* root, char* path);
// void CopyNode(Node* root, char* src_path, char* dst_path);
// void MoveNode(Node* root, char* src_path, char* dst_path);
// Node* FindNode(Node* root, char* path);
// void AddChild(Node* parent, Node* child);
// void DeleteChild(Node* parent, Node* child);
// void CreateFile(Node* root, char* path);
// void ProcessFile(Node* root, char* filename);
// void CreateFolder(Node* root, char* path);
// void ChangeDirectory(Node** current_Node, char* path);
// void ProcessInstruction(Node** current_Node, char* instruction);
// void PrintCurrentDirectory(Node* current_Node, int level);
// void PrintDirectory(Node* root, int level);

//liste de commande à faire imaginées/trouvées par moi

//choses à résoudre!!!!!!!!!
//noeud courant vraiment important mais je ne sais pas comment le faire, équivalent d'une variable static?
//mettre en place un système pour lire les chemins
//modifier système d'affichage pour faire la distinction entre les dossiers et les fichiers

//imo dernière commande est parsing de fichier d'input une fois que le reste fonctionne