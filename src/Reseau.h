#ifndef __RESEAU_H__
#define __RESEAU_H__

#include "ArbreQuat.h"
#include "Chaine.h"
#include "Hachage.h"
#include "Noeud.h"

/* Un reseau */
typedef struct {
    int nb_noeuds;             /* Nombre de noeuds du reseau */
    int gamma;                 /* Nombre maximal de fibres par cable */
    CellNoeud *noeuds;         /* Liste des noeuds du reseau */
    CellCommodite *commodites; /* Liste des commodites a relier */
} Reseau;

Noeud *recherche_cree_noeud_liste(Reseau *R, double x, double y);
Reseau *reconstitue_reseau_liste(Chaines *C);

void ecrire_reseau(Reseau *R, FILE *file);
int nb_liaisons(Reseau *R);
int nb_commodites(Reseau *R);
void affiche_reseau_SVG(Reseau *R, char *nom_instance);

Reseau *cree_reseau(int gamma);
void liberer_reseau(Reseau *reseau);

Noeud *recherche_cree_noeud_hachage(Reseau *R, TableHachage *table, double x, double y);
Reseau *reconstitue_reseau_hachage(Chaines *C, TableHachage *table_hachage);

Noeud *recherche_cree_noeud_arbre(Reseau *R, ArbreQuat *arbre, ArbreQuat *parent, double x, double y);
Reseau *reconstitue_reseau_arbre(Chaines *C, ArbreQuat *arbre);

#endif