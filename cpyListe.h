#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


void copie_fichier(char * cheminFichierSource, char * cheminFichierDestination);
void copie_liste_fichiers(char * cheminRepertoireSource, char * cheminRepertoireDestination);