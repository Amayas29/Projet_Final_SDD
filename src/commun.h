#ifndef __COMMUN_H__
#define __COMMUN_H__

#include <stdio.h>

#define print_probleme(message) fprintf(stderr, "%s dans la fonction %s à la ligne %d dans le fichier %s\n", message, __PRETTY_FUNCTION__, __LINE__, __FILE__)

#endif