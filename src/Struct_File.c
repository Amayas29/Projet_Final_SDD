#include "Struct_File.h"

#include <stdio.h>
#include <stdlib.h>

#include "commun.h"

S_file *cree_file() {
    S_file *file = malloc(sizeof(S_file));
    if (!file) {
        print_probleme("Erreur d'allocation");
        return NULL;
    }

    init_file(file);
    return file;
}

void init_file(S_file *f) {
    if (!f) {
        print_probleme("Pointeur invalide");
        return;
    }

    f->tete = NULL;
    f->dernier = NULL;
}

int est_file_vide(S_file *f) {
    if (!f) {
        print_probleme("Pointeur invalide");
        return -1;
    }

    return f->tete == NULL;
}

void enfile(S_file *f, int donnee) {
    if (!f) {
        print_probleme("Pointeur invalide");
        return;
    }

    Cellule_file *nouv = (Cellule_file *)malloc(sizeof(Cellule_file));
    nouv->val = donnee;
    nouv->suiv = NULL;
    if (f->tete == NULL)
        f->tete = nouv;
    else
        f->dernier->suiv = nouv;
    f->dernier = nouv;
}

int defile(S_file *f) {
    if (!f) {
        print_probleme("Pointeur invalide");
        return -1;
    }

    int v = f->tete->val;
    Cellule_file *temp = f->tete;
    if (f->tete == f->dernier)
        f->dernier = NULL;
    f->tete = f->tete->suiv;
    free(temp);
    return v;
}

void liberer_file(S_file *f) {
    if (!f) return;

    while (!(est_file_vide(f))) defile(f);
    free(f);
}