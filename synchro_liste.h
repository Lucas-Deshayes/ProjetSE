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

void get_heure_modif_fichier(char* fichier,char * modifTime);
void enregistre_contenu_rep(char * cheminRepertoire, char * fichierEnregistrement);
void compare_deux_repertoires(char * cheminFichier1,char * cheminFichier2);
void compare_pour_suppression(char * cheminFichier1,char* cheminFichier2);