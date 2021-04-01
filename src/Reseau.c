#include "Reseau.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SVGwriter.h"
#include "commun.h"

Noeud *cree_noeud(int numero, double x, double y) {
    Noeud *nd = (Noeud *)maloc(sizeof(Noeud));
    if (!nd) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }
    nd->num = numero;
    nd->voisins = NULL;
    nd->x = x;
    nd->y = y;

    return nd;
}

CellNoeud *cree_cell_noeud(Noeud *noeud) {
    CellNoeud *cell = (CellNoeud *)malloc(sizeof(CellNoeud));
    if (!cell) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    cell->nd = noeud;
    cell->suiv = NULL;
    return cell;
}

CellCommodite *cree_cell_commodite(Noeud *extr_A, Noeud *extr_B) {
    CellCommodite *comm = (CellCommodite *)malloc(sizeof(CellCommodite));
    if (!comm) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }
    comm->extr_A = extr_A;
    comm->extr_B = extr_B;
    comm->suiv = NULL;

    return comm;
}

Reseau *cree_reseau(int gamma) {
    Reseau *reseau = malloc(sizeof(Reseau));

    if (!reseau) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    reseau->gamma = gamma;
    reseau->commodites = NULL;
    reseau->nb_noeuds = 0;
    reseau->noeuds = NULL;
    return reseau;
}

Noeud *recherche_cree_noeud_liste(Reseau *R, double x, double y) {
    if (!R) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    CellNoeud *pred = NULL;

    for (CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv) {
        if (noeud->nd && noeud->nd->x == x && noeud->nd->y == y)
            return noeud->nd;

        pred = noeud;
    }

    Noeud *nd = cree_noeud(++R->nb_noeuds, x, y);
    if (!nd) {
        R->nb_noeuds--;
        return NULL;
    }

    CellNoeud *cell = cree_cell_noeud(nd);
    if (!cell) {
        R->nb_noeuds--;
        liberer_noeud(nd);
        return NULL;
    }

    if (!pred) {
        R->noeuds = cell;
        return nd;
    }

    pred->suiv = cell;
    return nd;
}

Reseau *reconstitue_reseau_liste(Chaines *C) {
    if (!C) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    Reseau *reseau = cree_reseau(C->gamma);
    if (!reseau)
        return NULL;

    for (CellChaine *chaine = C->chaines; chaine; chaine = chaine->suiv) {
        Noeud *first = NULL, *last = NULL;

        for (CellPoint *point = chaine->points; point; point = point->suiv) {
            Noeud *noeud = recherche_cree_noeud_liste(reseau, point->x, point->y);

            if (!noeud) {
                liberer_reseau(reseau);
                return NULL;
            }

            if (!first)
                first = noeud;

            if (last) {
                int existe = 0;
                for (CellNoeud *voisins = noeud->voisins; voisins; voisins = voisins->suiv) {
                    if (voisins->nd->num == last->num) {
                        existe = 1;
                        break;
                    }
                }

                if (existe) {
                    last = noeud;
                    continue;
                }

                CellNoeud *vn = cree_cell_noeud(last);
                if (!vn) {
                    liberer_reseau(reseau);
                    return NULL;
                }

                CellNoeud *vl = cree_cell_noeud(noeud);
                if (!vl) {
                    liberer_reseau(reseau);
                    free(vn);
                    return NULL;
                }

                vn->suiv = noeud->voisins;
                noeud->voisins = vn;

                vl->suiv = last->voisins;
                last->voisins = vl;
            }

            last = noeud;
        }

        if (first) {
            CellCommodite *cmd = cree_cell_commodite(first, last);

            if (!cmd) {
                liberer_reseau(reseau);
                return NULL;
            }

            cmd->suiv = reseau->commodites;
            reseau->commodites = cmd;
        }
    }

    return reseau;
}

void ecrire_reseau(Reseau *R, FILE *file) {
    if (!R) {
        print_probleme("Pointeur invalide");
        return;
    }

    if (!file) {
        print_probleme("Fichier introuvable");
        return;
    }

    int nombre_liais = nb_liaisons(R);
    fprintf(file, "NbNoeuds: %d\n", R->nb_noeuds);
    fprintf(file, "NbLiaisons: %d\n", nombre_liais);
    fprintf(file, "NbCommodites: %d\n", nb_commodites(R));
    fprintf(file, "Gamma: %d\n\n", R->gamma);

    for (CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv)
        fprintf(file, "v %d %.6f %.6f\n", noeud->nd->num, noeud->nd->x, noeud->nd->y);

    fprintf(file, "\n");

    char **cache = malloc(sizeof(char *) * nombre_liais);

    if (!cache) {
        print_probleme("Erreur d'allocation");
        return;
    }

    for (int i = 0; i < nombre_liais; i++)
        cache[i] = NULL;

    char buffer[BUFSIZ];
    int curr = 0;

    for (CellNoeud *noeud = R->noeuds; noeud; noeud = noeud->suiv) {
        for (CellNoeud *voisin = noeud->nd->voisins; voisin; voisin = voisin->suiv) {
            int i = 0;
            sprintf(buffer, "%d %d", voisin->nd->num, noeud->nd->num);

            for (; i < nombre_liais; i++) {
                if (cache[i] && strcmp(cache[i], buffer) == 0)
                    break;
            }

            if (i == nombre_liais) {
                sprintf(buffer, "%d %d", noeud->nd->num, voisin->nd->num);
                cache[curr++] = strdup(buffer);
                fprintf(file, "l %s\n", buffer);
            }
        }
    }

    for (int i = 0; i < nombre_liais; i++)
        free(cache[i]);
    free(cache);

    fprintf(file, "\n");

    for (CellCommodite *cmd = R->commodites; cmd; cmd = cmd->suiv)
        fprintf(file, "k %d %d\n", cmd->extr_A->num, cmd->extr_B->num);
}

static int number_voisins(CellNoeud *liste) {
    int number = 0;
    for (; liste; liste = liste->suiv, number++) continue;
    return number;
}

int nb_liaisons(Reseau *R) {
    if (!R) return 0;

    int number = 0;
    for (CellNoeud *noeud = R->noeuds; noeud;
         number += number_voisins(noeud->nd->voisins), noeud = noeud->suiv) continue;

    return number / 2;
}

int nb_commodites(Reseau *R) {
    if (!R) return 0;

    int number = 0;
    for (CellCommodite *cmd = R->commodites; cmd; cmd = cmd->suiv, number++) continue;
    return number;
}

void affiche_reseau_SVG(Reseau *R, char *nomInstance) {
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

    SVG_init(&svg, nomInstance, 500, 500);

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

void liberer_cell_noeuds(CellNoeud *cells, int rm) {
    CellNoeud *tmp = NULL;

    while (cells) {
        if (rm)
            liberer_noeud(cells->nd);

        tmp = cells->suiv;
        free(cells);
        cells = tmp;
    }
}

void liberer_noeud(Noeud *noeud) {
    if (!noeud) return;

    liberer_cell_noeuds(noeud->voisins, 0);
    free(noeud);
}

void liberer_commodites(CellCommodite *commodites) {
    CellCommodite *tmp = NULL;
    while (commodites) {
        tmp = commodites->suiv;
        free(commodites);
        commodites = tmp;
    }
}

void liberer_reseau(Reseau *reseau) {
    if (!reseau) return;

    liberer_commodites(reseau->commodites);
    liberer_cell_noeuds(reseau->noeuds, 1);
    free(reseau);
}

Noeud *recherche_cree_noeud_hachage(Reseau *R, TableHachage *hash_table, double x, double y) {
    if (!R || !hash_table || !hash_table->table) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    int hash = hachage(key(x, y), hash_table->lenght);
    CellNoeud *liste = hash_table->table[hash];

    for (; liste && (liste->nd->x != x || liste->nd->y != y); liste = liste->suiv) continue;

    if (liste)
        return liste->nd;

    int nb = R->nb_noeuds + 1;
    Noeud *nd = cree_noeud(nb, x, y);
    if (!nd)
        return NULL;

    CellNoeud *noeud_r = cree_cell_noeud(nd);
    if (!noeud_r) {
        liberer_noeud(nd);
        return NULL;
    }

    CellNoeud *noeud_h = cree_cell_noeud(nd);
    if (!noeud_r) {
        liberer_noeud(nd);
        free(noeud_r);
        return NULL;
    }

    noeud_r->suiv = R->noeuds;
    R->noeuds = noeud_r;

    noeud_h->suiv = hash_table->table[hash];
    hash_table->table[hash] = noeud_r;

    R->nb_noeuds = nb;
    return nd;
}

Reseau *reconstitue_reseau_hachage(Chaines *C, int lenght) {
    if (!C) {
        print_probleme("Pointeur invalide");
        return NULL;
    }

    if (lenght <= 0) {
        print_probleme("Taille invalide");
        return NULL;
    }

    TableHachage *table_hachage = cree_table_hachage(lenght);
    if (!table_hachage)
        return NULL;

    Reseau *reseau = cree_reseau(C->gamma);
    if (!reseau) {
        liberer_table_hachage(table_hachage);
        return NULL;
    }

    for (CellChaine *chaine = C->chaines; chaine; chaine = chaine->suiv) {
        Noeud *first = NULL, *last = NULL;

        for (CellPoint *point = chaine->points; point; point = point->suiv) {
            Noeud *noeud = recherche_cree_noeud_hachage(reseau, table_hachage, point->x, point->y);

            if (!noeud) {
                liberer_reseau(reseau);
                liberer_table_hachage(table_hachage);
                return NULL;
            }

            if (!first)
                first = noeud;

            if (last) {
                int existe = 0;
                for (CellNoeud *voisins = noeud->voisins; voisins; voisins = voisins->suiv) {
                    if (voisins->nd->num == last->num) {
                        existe = 1;
                        break;
                    }
                }

                if (existe) {
                    last = noeud;
                    continue;
                }

                CellNoeud *vn = cree_cell_noeud(last);
                if (!vn) {
                    liberer_reseau(reseau);
                    liberer_table_hachage(table_hachage);
                    return NULL;
                }

                CellNoeud *vl = cree_cell_noeud(noeud);
                if (!vl) {
                    liberer_reseau(reseau);
                    liberer_table_hachage(table_hachage);
                    free(vn);
                    return NULL;
                }

                vn->suiv = noeud->voisins;
                noeud->voisins = vn;

                vl->suiv = last->voisins;
                last->voisins = vl;
            }

            last = noeud;
        }

        if (first) {
            CellCommodite *cmd = cree_cell_commodite(first, last);

            if (!cmd) {
                liberer_reseau(reseau);
                liberer_table_hachage(table_hachage);
                return NULL;
            }

            cmd->suiv = reseau->commodites;
            reseau->commodites = cmd;
        }
    }

    liberer_table_hachage(table_hachage);
    return reseau;
}

Noeud *recherche_cree_noeud_arbre(Reseau *R, ArbreQuat *arbre, ArbreQuat *parent, double x, double y) {
    if (!R) {
        print_probleme("Pointeur invalide");
        return NULL;
    }
}