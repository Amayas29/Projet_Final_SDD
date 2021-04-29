#include "Reseau.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SVGwriter.h"
#include "commun.h"

// Permet de creer un reseau
Reseau *cree_reseau(int gamma) {
    // On alloue la memoire pour le reseau
    Reseau *reseau = malloc(sizeof(Reseau));

    // On test si l'allocation s'est bien passé
    if (!reseau) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    // On initialise les champs
    reseau->gamma = gamma;
    reseau->commodites = NULL;
    reseau->nb_noeuds = 0;
    reseau->noeuds = NULL;

    return reseau;
}

// Permet d'ecrire un reseau dans un fichier
void ecrire_reseau(Reseau *R, FILE *file) {
    // On test les pointeurs
    if (!R) {
        print_probleme("Pointeur invalide");
        return;
    }

    if (!file) {
        print_probleme("Fichier introuvable");
        return;
    }

    // On écrit les meta-donnes : Les nombres de liaisons, noeuds, commodites et le gamma
    int nombre_liais = nb_liaisons(R);
    fprintf(file, "NbNoeuds: %d\n", R->nb_noeuds);
    fprintf(file, "NbLiaisons: %d\n", nombre_liais);
    fprintf(file, "NbCommodites: %d\n", nb_commodites(R));
    fprintf(file, "Gamma: %d\n\n", R->gamma);

    // On écrit la liste des noeuds
    for (CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv)
        fprintf(file, "v %d %.6f %.6f\n", noeud->nd->num, noeud->nd->x, noeud->nd->y);

    fprintf(file, "\n");

    /* 
        On alloue un tableau de chaines de caracteres pour garder l'historique des chaines écrite
        >> Pour eviter d'ecrire 2 fois une liaison : 1->2 ensuite 2->1 par exemple
    */
    char **cache = malloc(sizeof(char *) * nombre_liais);

    // On test l'allocation
    if (!cache) {
        print_probleme("Erreur d'allocation");
        return;
    }

    // On initilise toutes les chaines à NULL
    for (int i = 0; i < nombre_liais; i++)
        cache[i] = NULL;

    char buffer[BUFSIZ];
    int curr = 0;

    // On parcours les noeuds
    for (CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv) {
        // Pour chaque noeud on parcours ces voisins
        for (CellNoeud *voisin = noeud->nd->voisins; voisin; voisin = voisin->suiv) {
            int i = 0;

            // On écrit dans un buffer sous format suivant : voisin noeud
            sprintf(buffer, "%d %d", voisin->nd->num, noeud->nd->num);

            // Et on cherche si on a déja ajouter la liaison voisin->noeud
            for (; i < nombre_liais; i++) {
                if (cache[i] && strcmp(cache[i], buffer) == 0)
                    break;
            }

            // Si on a pas déja ajouter
            if (i == nombre_liais) {
                sprintf(buffer, "%d %d", noeud->nd->num, voisin->nd->num);
                // On ajoute la liaison noeud -> voisin dans le cache
                cache[curr++] = strdup(buffer);

                // Et on l'ecrit dans le fichier
                fprintf(file, "l %s\n", buffer);
            }
        }
    }

    // On libere le tablleau temporaire cache
    for (int i = 0; i < nombre_liais; i++)
        free(cache[i]);
    free(cache);

    fprintf(file, "\n");

    // On écrit les commodites dans le fichier
    for (CellCommodite *cmd = R->commodites; cmd; cmd = cmd->suiv)
        fprintf(file, "k %d %d\n", cmd->extr_A->num, cmd->extr_B->num);
}

// Pour calculer le nombre de voisins d'un noeud
static int number_voisins(CellNoeud *liste) {
    int number = 0;
    // On parcours la liste et on increment à chaque fois (la longeur de la liste)
    for (; liste; liste = liste->suiv, number++) continue;
    return number;
}

// Permet de calculer le nombre de liaison
int nb_liaisons(Reseau *R) {
    // Si le reseau n'existe pas c'est 0
    if (!R) return 0;

    // Sinon
    int number = 0;

    /* 
        On calcule le nombre total des voisins de chaque noeud 
        (On ajoute à l'accumulateur le nombre de voisins de chaque noeud)
    */
    for (CellNoeud *noeud = R->noeuds; noeud;
         number += number_voisins(noeud->nd->voisins), noeud = noeud->suiv) continue;

    // On devise sur 2 pour eliminer les double liaison (1->2 == 2->1)
    return number / 2;
}

// Permet de calculer le nombre de commodites
int nb_commodites(Reseau *R) {
    // Si le reseau n'existe pas c'est 0
    if (!R) return 0;

    int number = 0;

    // C'est la longeur de la liste
    for (CellCommodite *cmd = R->commodites; cmd; cmd = cmd->suiv, number++) continue;

    return number;
}

// Permet d'afficher le reseau
void affiche_reseau_SVG(Reseau *R, char *nom_instance) {
    if (!R || !nom_instance) {
        print_probleme("Pointeur invalide");
        return;
    }

    CellNoeud *courN, *courv;
    SVGwriter svg;
    double maxx = 0, maxy = 0, minx = 1e6, miny = 1e6;

    courN = R->noeuds;
    while (courN != NULL) {
        if (maxx < courN->nd->x) maxx = courN->nd->x;
        if (maxy < courN->nd->y) maxy = courN->nd->y;
        if (minx > courN->nd->x) minx = courN->nd->x;
        if (miny > courN->nd->y) miny = courN->nd->y;
        courN = courN->suiv;
    }

    SVG_init(&svg, nom_instance, 500, 500);

    courN = R->noeuds;

    while (courN != NULL) {
        SVG_line_rand_color(&svg);
        SVG_point(&svg, 500 * (courN->nd->x - minx) / (maxx - minx), 500 * (courN->nd->y - miny) / (maxy - miny));
        courv = courN->nd->voisins;

        while (courv != NULL) {
            if (courv->nd->num < courN->nd->num)
                SVG_line(&svg, 500 * (courv->nd->x - minx) / (maxx - minx), 500 * (courv->nd->y - miny) / (maxy - miny),
                         500 * (courN->nd->x - minx) / (maxx - minx), 500 * (courN->nd->y - miny) / (maxy - miny));
            courv = courv->suiv;
        }

        courN = courN->suiv;
    }
    SVG_finalize(&svg);
}

// Permet de liberer un reseau
void liberer_reseau(Reseau *reseau) {
    if (!reseau) return;

    liberer_commodites(reseau->commodites);
    liberer_cell_noeuds(reseau->noeuds, 1);
    free(reseau);
}

/* Permet de rechercher un noeud dans la liste */
Noeud *recherche_cree_noeud_liste(Reseau *R, double x, double y) {
    // On teste si le reseau n'est pas null sinon on sort de la fonction
    if (!R) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    CellNoeud *pred = NULL;

    // On boucle sur tout les noeuds du reseau
    for (CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv) {
        // Si les coordonnes du noeud sont egaux a x et a y, on retourne le noeud trouver
        if (noeud->nd && noeud->nd->x == x && noeud->nd->y == y)
            return noeud->nd;

        pred = noeud;
    }

    // Sinon on cree un noeud
    Noeud *nd = cree_noeud(++R->nb_noeuds, x, y);

    // On teste si la creation s'est bien passé
    if (!nd) {
        print_probleme("Erreur de création");
        R->nb_noeuds--;
        return NULL;
    }

    // On cree une cellule pour le noeud
    CellNoeud *cell = cree_cell_noeud(nd);

    // On teste si la creation s'est bien passé
    if (!cell) {
        print_probleme("Erreur de création");
        R->nb_noeuds--;
        liberer_noeud(nd);
        return NULL;
    }

    // Si le precedent est null donc la liste est vide on insere le noeud en tete de liste
    if (!pred) {
        R->noeuds = cell;
        return nd;
    }

    // Sinon on l'insere a la fin de la liste
    pred->suiv = cell;
    return nd;
}

Reseau *reconstitue_reseau_liste(Chaines *C) {
    // Tester le pointeur
    if (!C) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    // Création du reseau
    Reseau *reseau = cree_reseau(C->gamma);

    // Verification de la creation
    if (!reseau) {
        print_probleme("Erreur de création");
        return NULL;
    }

    // On parcours les chaines
    for (CellChaine *chaine = C->chaines; chaine; chaine = chaine->suiv) {
        // Le first c'est pour garder le debut de la commodites
        // Le last pour la fin et aussi pour la liaison (le precedent)
        Noeud *first = NULL, *last = NULL;

        // On parcours les points de chaque chaine
        for (CellPoint *point = chaine->points; point; point = point->suiv) {
            // On recupere le noeud correspondant au point (x, y) (soit par creation ou bien par recherche)
            Noeud *noeud = recherche_cree_noeud_liste(reseau, point->x, point->y);

            // Si on a rien reccupere donc on a eu un probleme d'allocation
            if (!noeud) {
                print_probleme("Erreur de création");
                liberer_reseau(reseau);
                return NULL;
            }

            // Si on a jamais sauvegarder le debut on le sauvegarde
            if (!first)
                first = noeud;

            // Si last existe déja donc on a un precedent
            if (last) {
                int existe = 0;

                // On cherche si on déja la liaison on l'ajoute pas
                for (CellNoeud *voisins = noeud->voisins; voisins; voisins = voisins->suiv) {
                    if (voisins->nd->num == last->num) {
                        existe = 1;
                        break;
                    }
                }

                // si la liaison existe on passe au prochain point
                if (existe) {
                    last = noeud;
                    continue;
                }

                // On créer le cell noeud (correspondant au voisin) pour l'ajouter dans les voisins du noeud
                CellNoeud *vn = cree_cell_noeud(last);
                if (!vn) {
                    print_probleme("Erreur de création");
                    liberer_reseau(reseau);
                    return NULL;
                }

                // On créer le cell noeud (du noeud courrant) pour l'ajouter dans la liste des voisins du voisin courrant
                CellNoeud *vl = cree_cell_noeud(noeud);
                if (!vl) {
                    print_probleme("Erreur de création");
                    liberer_reseau(reseau);
                    free(vn);
                    return NULL;
                }

                // On ajoute le voisin courrant dans la liste des voisins du noeud courrant
                vn->suiv = noeud->voisins;
                noeud->voisins = vn;

                // On ajoute le noeud courrant dans la liste des voisins du voisin courrant
                vl->suiv = last->voisins;
                last->voisins = vl;
            }

            // On garde le dernier noeud
            last = noeud;
        }

        // On tests si first != NULL, donc la liste des points est non NULL
        if (first) {
            // On crée la commodites entre first et last
            CellCommodite *cmd = cree_cell_commodite(first, last);

            // On verifie la creation
            if (!cmd) {
                print_probleme("Erreur de création");
                liberer_reseau(reseau);
                return NULL;
            }

            // On ajoute la commodites en tete de la liste des commodites
            cmd->suiv = reseau->commodites;
            reseau->commodites = cmd;
        }
    }

    return reseau;
}

/* Permet de rechercher un noeud dans la table de hachage */
Noeud *recherche_cree_noeud_hachage(Reseau *R, TableHachage *hash_table, double x, double y) {
    // On teste si le reseau ou la table de hachage n'est pas null sinon on sort de la fonction
    if (!R || !hash_table || !hash_table->table) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    // On calcule l'indice du noeud dans la table
    int hash = hachage(key(x, y), hash_table->lenght);

    // On cherche le noeud dans la liste des noeuds qui se trouve dans l'indice calculer
    for (CellNoeud *liste = hash_table->table[hash]; liste; liste = liste->suiv)
        // Si les coordonnes du noeud sont egaux a x et a y, on retourne le noeud trouver
        if (liste->nd->x == x && liste->nd->y == y) return liste->nd;

    // Sinon on cree un noeud
    Noeud *nd = cree_noeud(R->nb_noeuds + 1, x, y);

    // On teste si la creation s'est bien passé
    if (!nd) {
        print_probleme("Erreur de création");
        return NULL;
    }

    // On cree une cellule pour le noeud pour le reseau
    CellNoeud *noeud_r = cree_cell_noeud(nd);

    // On teste si la creation s'est bien passé
    if (!noeud_r) {
        print_probleme("Erreur de création");
        liberer_noeud(nd);
        return NULL;
    }

    // On cree une cellule pour le noeud pour la table de hachage
    CellNoeud *noeud_h = cree_cell_noeud(nd);

    // On teste si la creation s'est bien passé
    if (!noeud_r) {
        print_probleme("Erreur de création");
        liberer_noeud(nd);
        free(noeud_r);
        return NULL;
    }

    // On insere le noeud en tete de liste dans le reseau
    noeud_r->suiv = R->noeuds;
    R->noeuds = noeud_r;

    // On insere le noeud en tete de liste dans la case qui lui correspond dans la table de hachage
    noeud_h->suiv = hash_table->table[hash];
    hash_table->table[hash] = noeud_h;

    R->nb_noeuds++;
    return nd;
}

Reseau *reconstitue_reseau_hachage(Chaines *C, TableHachage *table_hachage) {
    // Tester le pointeurs
    if (!C || !table_hachage) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    // Création du reseau
    Reseau *reseau = cree_reseau(C->gamma);

    // Verification de la creation
    if (!reseau) {
        print_probleme("Erreur de création");
        return NULL;
    }

    // On parcours les chaines
    for (CellChaine *chaine = C->chaines; chaine; chaine = chaine->suiv) {
        // Le first c'est pour garder le debut de la commodites
        // Le last pour la fin et aussi pour la liaison (le precedent)
        Noeud *first = NULL, *last = NULL;

        // On parcours les points de chaque chaine
        for (CellPoint *point = chaine->points; point; point = point->suiv) {
            // On recupere le noeud correspondant au point (x, y) (soit par creation ou bien par recherche)
            Noeud *noeud = recherche_cree_noeud_hachage(reseau, table_hachage, point->x, point->y);

            // Si on a rien reccupere donc on a eu un probleme d'allocation
            if (!noeud) {
                print_probleme("Erreur de création");
                liberer_reseau(reseau);
                return NULL;
            }

            // Si on a jamais sauvegarder le debut on le sauvegarde
            if (!first)
                first = noeud;

            // Si last existe déja donc on a un precedent
            if (last) {
                int existe = 0;

                // On cherche si on déja la liaison on l'ajoute pas
                for (CellNoeud *voisins = noeud->voisins; voisins; voisins = voisins->suiv) {
                    if (voisins->nd->num == last->num) {
                        existe = 1;
                        break;
                    }
                }

                // si la liaison existe on passe au prochain point
                if (existe) {
                    last = noeud;
                    continue;
                }

                // On créer le cell noeud (correspondant au voisin) pour l'ajouter dans les voisins du noeud
                CellNoeud *vn = cree_cell_noeud(last);
                if (!vn) {
                    print_probleme("Erreur de création");
                    liberer_reseau(reseau);
                    return NULL;
                }

                // On créer le cell noeud (du noeud courrant) pour l'ajouter dans la liste des voisins du voisin courrant
                CellNoeud *vl = cree_cell_noeud(noeud);
                if (!vl) {
                    print_probleme("Erreur de création");
                    liberer_reseau(reseau);
                    free(vn);
                    return NULL;
                }

                // On ajoute le voisin courrant dans la liste des voisins du noeud courrant
                vn->suiv = noeud->voisins;
                noeud->voisins = vn;

                // On ajoute le noeud courrant dans la liste des voisins du voisin courrant
                vl->suiv = last->voisins;
                last->voisins = vl;
            }

            // On garde le dernier noeud
            last = noeud;
        }

        // On tests si first != NULL, donc la liste des points est non NULL
        if (first) {
            // On crée la commodites entre first et last
            CellCommodite *cmd = cree_cell_commodite(first, last);

            // On verifie la creation
            if (!cmd) {
                print_probleme("Erreur de création");
                liberer_reseau(reseau);
                return NULL;
            }

            // On ajoute la commodites en tete de la liste des commodites
            cmd->suiv = reseau->commodites;
            reseau->commodites = cmd;
        }
    }

    return reseau;
}

/* Permet de rechercher un noeud dans l'arbre */
Noeud *recherche_cree_noeud_arbre(Reseau *R, ArbreQuat *arbre, ArbreQuat *parent, double x, double y) {
    // On teste si le reseau ou l'arbre n'est pas null sinon on sort de la fonction
    if (!R || !parent) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    // On chache le noeud dans l'arbre si il existe on le retourne
    Noeud *noeud = recherche_noeud_arbre(arbre, x, y);
    if (noeud) return noeud;

    // sinon on le creer un noeud
    noeud = cree_noeud(R->nb_noeuds + 1, x, y);

    // On teste si la creation s'est bien passé
    if (!noeud) {
        print_probleme("Erreur de création");
        return NULL;
    }

    // on insere le noeud dans l'arbre
    inserer_noeud_arbre(noeud, &arbre, parent);

    // On cree une cellule pour le noeud
    CellNoeud *noeud_r = cree_cell_noeud(noeud);

    // On teste si la creation s'est bien passé
    if (!noeud_r) {
        print_probleme("Erreur de création");
        liberer_noeud(noeud);
        return NULL;
    }

    // On insere le noeud en tete de liste dans reseau
    noeud_r->suiv = R->noeuds;
    R->noeuds = noeud_r;

    R->nb_noeuds++;
    return noeud;
}

Reseau *reconstitue_reseau_arbre(Chaines *C, ArbreQuat *arbre) {
    // Tester le pointeurs
    if (!C || !arbre) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    // Création du reseau
    Reseau *reseau = cree_reseau(C->gamma);

    // Verification de la creation
    if (!reseau) {
        print_probleme("Erreur de création");
        return NULL;
    }

    // On parcours les chaines
    for (CellChaine *chaine = C->chaines; chaine; chaine = chaine->suiv) {
        // Le first c'est pour garder le debut de la commodites
        // Le last pour la fin et aussi pour la liaison (le precedent)
        Noeud *first = NULL, *last = NULL;

        // On parcours les points de chaque chaine
        for (CellPoint *point = chaine->points; point; point = point->suiv) {
            // On determine le sous arbre ou ajouter selon les coordonnes du point et du centre de l'arbre
            ArbreQuat *sous_arbre = NULL;

            if (arbre->xc > point->x) {
                if (arbre->yc > point->y)
                    sous_arbre = arbre->so;
                else
                    sous_arbre = arbre->no;

            } else {
                if (arbre->yc > point->y)
                    sous_arbre = arbre->se;
                else
                    sous_arbre = arbre->ne;
            }

            // On recupere le noeud correspondant au point (x, y) (soit par creation ou bien par recherche)
            Noeud *noeud = recherche_cree_noeud_arbre(reseau, sous_arbre, arbre, point->x, point->y);

            // Si on a rien reccupere donc on a eu un probleme d'allocation
            if (!noeud) {
                print_probleme("Erreur de création");
                liberer_reseau(reseau);
                return NULL;
            }

            // Si on a jamais sauvegarder le debut on le sauvegarde
            if (!first)
                first = noeud;

            // Si last existe déja donc on a un precedent
            if (last) {
                int existe = 0;

                // On cherche si on déja la liaison on l'ajoute pas
                for (CellNoeud *voisins = noeud->voisins; voisins; voisins = voisins->suiv) {
                    if (voisins->nd->num == last->num) {
                        existe = 1;
                        break;
                    }
                }

                // si la liaison existe on passe au prochain point
                if (existe) {
                    last = noeud;
                    continue;
                }

                // On créer le cell noeud (correspondant au voisin) pour l'ajouter dans les voisins du noeud
                CellNoeud *vn = cree_cell_noeud(last);
                if (!vn) {
                    print_probleme("Erreur de création");
                    liberer_reseau(reseau);
                    return NULL;
                }

                // On créer le cell noeud (du noeud courrant) pour l'ajouter dans la liste des voisins du voisin courrant
                CellNoeud *vl = cree_cell_noeud(noeud);
                if (!vl) {
                    print_probleme("Erreur de création");
                    liberer_reseau(reseau);
                    free(vn);
                    return NULL;
                }

                // On ajoute le voisin courrant dans la liste des voisins du noeud courrant
                vn->suiv = noeud->voisins;
                noeud->voisins = vn;

                // On ajoute le noeud courrant dans la liste des voisins du voisin courrant
                vl->suiv = last->voisins;
                last->voisins = vl;
            }

            // On garde le dernier noeud
            last = noeud;
        }

        // On tests si first != NULL, donc la liste des points est non NULL
        if (first) {
            // On crée la commodites entre first et last
            CellCommodite *cmd = cree_cell_commodite(first, last);

            // On verifie la creation
            if (!cmd) {
                print_probleme("Erreur de création");
                liberer_reseau(reseau);
                return NULL;
            }

            // On ajoute la commodites en tete de la liste des commodites
            cmd->suiv = reseau->commodites;
            reseau->commodites = cmd;
        }
    }

    return reseau;
}