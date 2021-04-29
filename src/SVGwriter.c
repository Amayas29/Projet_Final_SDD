#include "SVGwriter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commun.h"

void SVG_init(SVGwriter *svg, char *nom, int sizeX, int sizeY) {
    if (!svg) {
        print_probleme("Pointeur invalide");
        return;
    }

    char filename[100];

    strcpy(filename, nom);
    strcat(filename, ".html");

    svg->file = fopen(filename, "w");

    if (svg->file == NULL) {
        printf("Unable to create %s\n", nom);
        exit(1);
    }

    strcpy(svg->line_color, Red);
    strcpy(svg->point_color, Black);

    svg->sizeX = sizeX;
    svg->sizeY = sizeY;

    svg->gencol[0] = '0';
    svg->gencol[1] = '1';
    svg->gencol[2] = '2';
    svg->gencol[3] = '3';
    svg->gencol[4] = '4';
    svg->gencol[5] = '5';
    svg->gencol[6] = '6';
    svg->gencol[7] = '7';
    svg->gencol[8] = '8';
    svg->gencol[9] = '9';
    svg->gencol[10] = 'A';
    svg->gencol[11] = 'B';
    svg->gencol[12] = 'C';
    svg->gencol[13] = 'D';
    svg->gencol[14] = 'E';
    svg->gencol[15] = 'F';

    fprintf(svg->file, "<html><body><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\"");
    fprintf(svg->file, " width=\"100%%\" height=\"100%%\"");
    fprintf(svg->file, " viewBox=\"%lf %lf %lf %lf\"", -2.0, -2.0, sizeX + 7.0, sizeY + 7.0);
    fprintf(svg->file, " preserveAspectRatio=\"yes\">\n");
    fprintf(svg->file, "<g >\n\n");

    svg->line_color[0] = '#';
}

void SVG_line_color(SVGwriter *svg, char *col) {
    if (!svg || !col) {
        print_probleme("Pointeur invalide");
        return;
    }

    strcpy(svg->line_color, col);
}

void SVG_line_rand_color(SVGwriter *svg) {
    if (!svg) {
        print_probleme("Pointeur invalide");
        return;
    }

    int i;
    for (i = 1; i <= 6; i++) svg->line_color[i] = svg->gencol[rand() % 16];
}

void SVG_point_color(SVGwriter *svg, char *col) {
    if (!svg || !col) {
        print_probleme("Pointeur invalide");
        return;
    }

    strcpy(svg->point_color, col);
}

void SVG_point(SVGwriter *svg, double x, double y) {
    if (!svg) {
        print_probleme("Pointeur invalide");
        return;
    }

    fprintf(svg->file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"2\" stroke=\"%s\" stroke-width=\"1\" fill=\"%s\" />\n", x, y, svg->point_color, svg->point_color);
}

void SVG_line(SVGwriter *svg, double xa, double ya, double xb, double yb) {
    if (!svg) {
        print_probleme("Pointeur invalide");
        return;
    }

    fprintf(svg->file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" ", xa, ya, xb, yb);
    fprintf(svg->file, " style=\"stroke:%s;stroke-width:2\"/>\n", svg->line_color);
}

void SVG_finalize(SVGwriter *svg) {
    if (!svg) {
        print_probleme("Pointeur invalide");
        return;
    }

    fprintf(svg->file, "\n\n</g></svg></body></html>\n");
    fclose(svg->file);
    svg->file = NULL;
}