#ifndef _SVGWRITER_H_
#define _SVGWRITER_H_

/* Dans cette librairie, les couleurs sont utilisees sous leur forme
   hexadecimale comme le montre les exemples suivants) */

#define Black "#000000"
#define Cyan "#00FFFF"
#define Fuchsia "#FF00FF"
#define Lime "#00FF00"
#define Orange "#FFA800"
#define Red "#FF0000"
#define RoyalBlue "#486CE8"

#include <stdio.h>

typedef struct SVGwriter_struct {
    char line_color[10];  /* Default color value */
    char point_color[10]; /* Default color value */
    int sizeX, sizeY;     /* Picture size */

    FILE *file; /* File stream */

    /* Technical value for random color generation */
    char gencol[16];

} SVGwriter;

/* Initialise le struct SVGwriter passe en parametre
      nomfic: nom du fichier html \'a creer
      sizeX, sizeY: taille en pixel de l'image \'a creer */
void SVG_init(SVGwriter *svg, char *nomfic, int sizeX, int sizeY);

/* Fixe la couleur des lignes pour la suite */
void SVG_line_color(SVGwriter *svg, char *col);

/* Fixe une couleur aleatoire pour les lignes pour la suite */
void SVG_line_rand_color(SVGwriter *svg);

/* Fixe la couleur des points pour la suite */
void SVG_point_color(SVGwriter *svg, char *col);

/* Ecrit un point dans le fichier */
void SVG_point(SVGwriter *svg, double x, double y);

/*Ecrit un segment dans le fichier */
void SVG_line(SVGwriter *svg, double xa, double ya, double xb, double yb);

/* Ferme le fichier */
void SVG_finalize(SVGwriter *svg);

#endif