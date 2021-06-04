#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


void retarde(int secondes);
int compte_lignes_fichier(char * nomFichier); // finalement inutile :)  (je garde, on sait jamais)
void get_heure_modif_fichier(char * modifTime);
void enregistre_contenu_rep(char * cheminRepertoire, char * fichierEnregistrement, char * modifTime);
void compare_deux_repertoires(char * cheminFichier1);


int main()
{   
  	clock_t debut, fin; 
    	double tempsPasse; 

	char modifTime[15];

   	debut = clock();
	retarde(1); //transformer en rand dans un intervalle - éventuellement par une portion de code

	printf("--- Global: ---\n"); 

	//Temps passé
    	fin = clock(); 
    	tempsPasse = ((double)fin - debut) / CLOCKS_PER_SEC; 
    	printf("%.2f secondes se sont passées.\n", tempsPasse);

	//Taille diff
	//Nb erreurs


	printf("\n--- Serveur d'intégration: ---\n");

	/*int nbLignes = compte_lignes_fichier("ancienRep.txt");
	printf("yoo: %d", nbLignes);*/
	enregistre_contenu_rep(".", "ancienRep.txt", modifTime);
	//compare_deux_repertoires("ancienRep.txt");

	//taille diff
	//nb fichiers reçus

	printf("\n--- Serveur de back up: ---\n");
	//taille diff
	//nb fichiers reçus

	printf("\n--- Serveur de production: ---\n");
	//taille diff
	//nb fichiers reçus

	return 0;	
}



void retarde(int secondes)
{
    clock_t debut, pause;

    pause = secondes * CLOCKS_PER_SEC;
    debut = clock();
    while( (clock() - debut) < pause );
}


int compte_lignes_fichier(char * nomFichier) // finalement inutile :)  (je garde, on sait jamais)
{
	FILE * fichier = NULL;
	int c;
	int nbLignes = 0;

	fichier = fopen(nomFichier, "r");

	if(fichier == NULL)
		printf("erreur");
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


void get_heure_modif_fichier(char * modifTime)
{
	struct stat st;
	time_t t = st.st_mtime;
	struct tm lt;

	localtime_r(&t, &lt);
	strftime(modifTime, sizeof(modifTime) * 15, "%s", &lt);
	printf("szfvrhgzcfegiAHHHHHHHHHHHHHHHHHHH: %s\n",modifTime);
}


void enregistre_contenu_rep(char * cheminRepertoire, char * fichierEnregistrement, char * modifTime)
{
	struct dirent 	*d;
	DIR		*dir;
	
	int nbLettresFichier;
	char * str = malloc(0 * sizeof(char *));

	dir = opendir(cheminRepertoire);

	if(dir == NULL)
		printf("erreur\n");	
	
	else
	{
		FILE * fichier = NULL;
		fichier = fopen(fichierEnregistrement, "w");

		if(fichier == NULL)
			printf("erreur");
		else
		{
			while((d = readdir(dir)) != NULL)
			{
				if(strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0)
				{
					fseek(fichier, 0, SEEK_END);
									
					str = realloc(str, nbLettresFichier * sizeof(char*) );

					nbLettresFichier = 0; 
					while((d->d_name)[nbLettresFichier] != '\0')
					{
						nbLettresFichier++;
					}

					strcpy(str, d->d_name);
					strcat(str, "|");
					get_heure_modif_fichier(modifTime);
					strcat(str, modifTime);
					fputs(strcat(str, "\n"), fichier);
				}
			}
		}
		fclose(fichier);
	}


	if (closedir(dir) == -1) 
		printf("erreur\n");

}


void compare_deux_repertoires(char * cheminFichier1) //après -> rajouter cheminFichier2
{
	char ligne[256];

	FILE * fichier = NULL;
	fichier = fopen(cheminFichier1, "r");

	if(fichier == NULL)
		printf("erreur");
	else{
		while (fgets(ligne, 256, fichier) != NULL)
		{
			char * nomFichier = strtok(ligne, "|");
			printf("%s\n", nomFichier);
			char * dateFichier = strtok(NULL, "|");
			printf("%s\n", dateFichier);
		} 
	}
}







