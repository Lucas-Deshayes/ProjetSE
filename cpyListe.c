#include "cpyListe.h"

void copie_fichier(char * cheminFichierSource, char * cheminFichierDestination)
{
	char ligne[256];

	FILE * fichierSource = NULL;
	FILE * fichierDestination = NULL;

	fichierSource = fopen(cheminFichierSource, "r");
	fichierDestination = fopen(cheminFichierDestination, "w");

	if(fichierSource == NULL || fichierDestination == NULL)
	{
		if(fichierSource == NULL)
			perror("ERREUR OUVERTURE DU FICHIER SOURCE\n");
		if(fichierDestination == NULL)
			perror("ERREUR OUVERTURE DE FICHIER DE DESTINATION\n");
		exit(-1);
	}
	else{
		while (fgets(ligne, 256, fichierSource) != NULL) //parcourt le fichier source afin d'en récupérer la ligne parcourue
			fputs(ligne, fichierDestination); //écrit la ligne récupérée dans le fichier de destination
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
			
			char * nomFichier = strtok(ligne, "|");//segmente la ligne récupérée afin de récupérer la partie avant le séparateur "|"
			char typeModif = strtok(NULL, "|")[0];
			if(typeModif == 'M' || typeModif == 'C')//si il faut mettre à jour le fichier ou l'ajouter.
			{
				if(strcmp(nomFichier,"\0") != 0)//si ce n'est pas la fin du fichier
				{
					//concataine le chemin du répertoire src + "/" + nom du fichier src, afin de récuperer le chemin du fichier src  
					strcpy(cheminFichierSource,cheminRepertoireSource);
					strcat(cheminFichierSource,"/");
					strcat(cheminFichierSource,nomFichier);

					//concataine le chemin du répertoire dst + "/" + nom du fichier dst, afin de récuperer le chemin du fichier dst  
					strcpy(cheminFichierDestination,cheminRepertoireDestination);
					strcat(cheminFichierDestination,"/");
					strcat(cheminFichierDestination,nomFichier);

					/*printf("src:%s\n", cheminFichierSource);
					printf("dst:%s\n", cheminFichierDestination);
					printf("---------------------\n");*/

					copie_fichier(cheminFichierSource, cheminFichierDestination);
				}
			}
			else if (typeModif == 'S')//Supprimer les fichiers en trop
			{
				strcpy(cheminFichierDestination,cheminRepertoireDestination);
				strcat(cheminFichierDestination,"/");
				strcat(cheminFichierDestination,nomFichier);
				remove(cheminFichierDestination);
			}
		}
	}

	fclose(fichierListe);

}




