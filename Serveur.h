#include <sys/types.h>	//pour le type pid_t (entier sur 32bits sur la plupart des UNIX
#include <unistd.h>		//pour le fork()
#include <stdio.h>		//perror() et printf()
#include <stdlib.h>		//pour utiliser le rand() => man 3 rand
#include <time.h>		//pour utiliser le time()
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>


enum dossiers { 
    DossierBackUp, 
    DossierProduction 
};

#ifndef SERVEUR_H
#define SERVEUR_H

void * serveurProduction();
void * serveurIntegration();
void * serveurBackUp();

void random_string(char *s, int len, bool fichier);

int random_intervalle(int a, int b);

void ajout_fichier(enum dossiers d);

int compte_nombre_fichier(char * path);
void modifier_fichier(enum dossiers d);
void ajout_fin_fichier(char * pathFichier);

#endif