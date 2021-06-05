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

void retarde(int secondes);
void stat_sync();
void stats_module_log();
int compte_lignes_fichier(char * nomFichier);
int compte_caracteres_fichier(char * cheminFichier);



int main()
{   
  	clock_t debut, fin; 
    	double tempsPasse; 
		
	char modifTime[16];

   	debut = clock();
	retarde(1); //transformer en rand dans un intervalle - éventuellement par une portion de code

	printf("--- Global: ---\n"); 
	//Temps passé
    	fin = clock(); 
    	tempsPasse = ((double)fin - debut) / CLOCKS_PER_SEC; 
    	printf("%.2f secondes se sont passées.\n", tempsPasse);
	stats_module_log();
	return 0;
}



void retarde(int secondes)
{
    clock_t debut, pause;

    pause = secondes * CLOCKS_PER_SEC;
    debut = clock();
    while( (clock() - debut) < pause );
}


void stat_sync()
{
	char ligne[256];
	int modifier = 0;
	int creer = 0;
	FILE * fichier = NULL;
	fichier = fopen("difference.txt", "r");

	if(fichier == NULL)
	{
		perror("OUVERTURE DE DIFFERENCE.txt");
		exit(-2);
	}
	else{
		while (fgets(ligne, 256, fichier) != NULL)
		{
			char * nomFichier = strtok(ligne, "|"); //strtok permet de segmenter une chaîne de caractères sur un caractère donné
			char typeModif = strtok(NULL, "|")[0]; //NULL signifie qu'il faut continuer de segmenter la chaîne passée en 1er
			if(typeModif == 'M') //si symbole de modification de fichier
			{
				printf("%s a été modifié\n", nomFichier);
				modifier++;
			}
			else  //sinon c'est une création de fichier
			{
				printf("%s a été créé\n", nomFichier);
				creer++;
			}
		}
		printf("TOTAL de fichier modifies : %d\n",modifier);
		printf("TOTAL de fichier crees : %d\n",creer);
	}

	fclose(fichier);
}


void stats_module_log()
{
	int nbLignes = compte_lignes_fichier("./modif.txt"); // ./logs.txt
	int nbCaracteres = compte_caracteres_fichier("./modif.txt"); // ./logs.txt

	if(nbLignes>1)
		printf("Le fichier logs fait %d lignes, et comporte %d caractères.\n", nbLignes, nbCaracteres);
	else
	{
		if(nbCaracteres == 0)
			printf("Le fichier logs est vide.\n");
		else if(nbCaracteres == 1)
			printf("Le fichier logs fait %d ligne, et comporte %d caractère.\n", nbLignes, nbCaracteres);
		else
			printf("Le fichier logs fait %d ligne, et comporte %d caractères.\n", nbLignes, nbCaracteres);
	}
}


int compte_lignes_fichier(char * nomFichier)
{
	FILE * fichier = NULL;
	int c;
	int nbLignes = 0;

	fichier = fopen(nomFichier, "r");

	if(fichier == NULL)
	{
		perror("ERREUR OUVERTURE DE FICHIER\n");
		exit(-1);
	}
	else
	{
		while((c=fgetc(fichier)) != EOF)
		{
			if(c=='\n')
				nbLignes++;
		}
	}
	return nbLignes;
}


int compte_caracteres_fichier(char * cheminFichier)
{
	int i;
	int nbCaracteres = 0;
	char ligne[256];
	FILE * fichier = NULL;

	fichier = fopen(cheminFichier, "r");

	if(fichier == NULL)
	{
		perror("ERREUR OUVERTURE DE FICHIER\n");
		exit(-1);
	}
	else{
		while (fgets(ligne, 256, fichier) != NULL)
		{
			for(i=0; ligne[i+1] != '\0' ; i++){} //incrémente i afin que i prenne le nombre de caractères de la ligne
			nbCaracteres += i; //ajoute le nombre de caractères de la ligne à nbCaractères
		}
	}

	fclose(fichier);
	return nbCaracteres;
}





