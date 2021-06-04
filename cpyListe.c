#include "cpyListe.h"

void copie_fichier(char * cheminFichierSource, char * cheminFichierDestination)
{
	char ligne[256];

	FILE * fichierSource = NULL;
	FILE * fichierDestination = NULL;

	fichierSource = fopen(cheminFichierSource, "r");
	fichierDestination = fopen(cheminFichierDestination, "w");

	if(fichierSource == NULL || fichierDestination == NULL)
		printf("erreur\n"); //remplacer par des exit
	else{
		while (fgets(ligne, 256, fichierSource) != NULL)
			fputs(ligne, fichierDestination);
	}

	fclose(fichierSource);
	fclose(fichierDestination);
}



void copie_liste_fichiers(char * cheminRepertoireSource, char * cheminRepertoireDestination)
{
	char ligne[256];
	char cheminFichierSource[100];
	char cheminFichierDestination[100];
	FILE * fichierListe = NULL;
	fichierListe = fopen("difference.txt", "r");

	if(fichierListe == NULL)
		printf("erreur");
	else{
		while (fgets(ligne, 256, fichierListe) != NULL)
		{
			
			char * nomFichier = strtok(ligne, "|");

			if(strcmp(nomFichier,"\0") != 0)
			{
				strcpy(cheminFichierSource,cheminRepertoireSource);
				strcpy(cheminFichierDestination,cheminRepertoireDestination);
				strcat(cheminFichierSource,"/");
				strcat(cheminFichierDestination,"/");
				strcat(cheminFichierSource,nomFichier);
				strcat(cheminFichierDestination,nomFichier);
				printf("src:%s\n", cheminFichierSource);
				printf("dst:%s\n", cheminFichierDestination);
				printf("---------------------\n");

				copie_fichier(cheminFichierSource, cheminFichierDestination);
			}
		} 
	}

	fclose(fichierListe);

}




