#include "stats.h"


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
	int supp = 0;
	FILE * fichier = NULL;
	fichier = fopen("difference.txt", "r");
	FILE * sortie = fopen("stats.txt","a");
	if(fichier == NULL || sortie == NULL)
	{
		perror("OUVERTURE DE DIFFERENCE.txt");
		exit(-2);
	}
	else{
		time_t t;
		time(&t);
		char* heure = ctime(&t);
		fputs("\n---------STAT_SYNCHRO----------",sortie);
		fputs(heure,sortie);
		while (fgets(ligne, 256, fichier) != NULL)
		{
			char * nomFichier = strtok(ligne, "|"); //strtok permet de segmenter une chaîne de caractères sur un caractère donné
			char typeModif = strtok(ligne, "|")[0]; //NULL signifie qu'il faut continuer de segmenter la chaîne passée en 1er
			if(typeModif == 'M') //si symbole de modification de fichier
			{
				fprintf(sortie,"%s a été modifié\n", nomFichier);
				modifier++;
			}
			else if (typeModif == 'C') // Si symbole de création de fichier
			{
				fprintf(sortie,"%s a été créé\n", nomFichier);
				creer++;
			}
			else // Sinon c'est une suppression
			{
				fprintf(sortie,"%s a été supprimé\n", nomFichier);
				supp++;
			}
		}
		fprintf(sortie,"\nTotal de fichier modifies : %d\n",modifier);
		fprintf(sortie,"Total de fichier crees : %d\n",creer);
		fprintf(sortie,"Total de fichier supprimes : %d\n",supp);
		fputs("-------------------------------\n",sortie);
	}
	fclose(sortie);
	fclose(fichier);
}

void stats_module_log()
{
	int nbLignes = compte_lignes_fichier("log.txt"); // ./logs.txt
	int nbCaracteres = compte_caracteres_fichier("log.txt"); // ./logs.txt
	FILE * sortie = fopen("stats.txt","a");
	time_t t;
	time(&t);
	char* heure = ctime(&t);
	fputs("\n---------STAT_LOG---------------",sortie);
	fputs(heure,sortie);
	if(sortie == NULL)
	{
		perror("OUVERTURE DE STATS.TXT");
		exit(-2);
	}

	if(nbLignes>1)
		fprintf(sortie,"Le fichier logs fait %d lignes, et comporte %d caractères.\n", nbLignes, nbCaracteres);
	else
	{
		if(nbCaracteres == 0)
			fprintf(sortie,"Le fichier log est vide.\n");
		else if(nbCaracteres == 1)
			fprintf(sortie,"Le fichier log fait %d ligne, et comporte %d caractère.\n", nbLignes, nbCaracteres);
		else
			fprintf(sortie,"Le fichier log fait %d ligne, et comporte %d caractères.\n", nbLignes, nbCaracteres);
	}
	fputs("-------------------------------\n",sortie);
	fclose(sortie);
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


