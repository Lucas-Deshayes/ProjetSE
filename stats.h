#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


#ifndef STATS_H
#define STATS_H

void retarde(int secondes);
void stat_sync();
void stats_module_log();
int compte_lignes_fichier(char * nomFichier);
int compte_caracteres_fichier(char * cheminFichier);

#endif