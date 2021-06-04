#include <sys/types.h>	//pour le type pid_t (entier sur 32bits sur la plupart des UNIX
#include <unistd.h>		//pour le fork()
#include <stdio.h>		//perror() et printf()
#include <stdlib.h>		//pour utiliser le rand() => man 3 rand
#include <time.h>		//pour utiliser le time()
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>

enum dossiers { 
    dossierBackUp, 
    dossierProduction 
};

#ifndef SERVEUR_H
#define SERVEUR_H

void * serveurProduction();
void * serveurIntegration();
void * serveurBackUp();


#endif