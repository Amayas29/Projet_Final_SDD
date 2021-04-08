#ifndef __COMMUN_H__
#define __COMMUN_H__

#include <stdio.h>

#define NB_MAX  10000
#define print_probleme(message) fprintf(stderr, "%s dans la fonction %s à la ligne %d\n", message, __PRETTY_FUNCTION__, __LINE__)

#endif