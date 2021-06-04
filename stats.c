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
int compte_lignes_fichier(char * nomFichier);
void get_heure_modif_fichier(char* fichier,char * modifTime);
void enregistre_contenu_rep(char * cheminRepertoire, char * fichierEnregistrement, char * modifTime);
void compare_deux_repertoires(char * cheminFichier1);
void stat_sync();
void stats_module_log();
int compte_nb_caracteres_fichier(char * cheminFichier);



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


void get_heure_modif_fichier(char* fichier,char * modifTime)
{
	//struct stat st;
	//time_t t = st.st_mtime;
	struct tm* lt;
	struct stat attr;
    	stat(fichier, &attr);
    	printf("Last modified time: %s", ctime(&attr.st_mtime));
	lt = localtime(&attr.st_mtime);
	strftime(modifTime, sizeof(modifTime) * 15, "%s", lt);
	printf("szfvrhgzcfegiAHHHHHHHHHHHHHHHHHHH: %s\n",modifTime);
    
}


void enregistre_contenu_rep(char * cheminRepertoire, char * fichierEnregistrement, char * modifTime)
{
	struct dirent 	*d;
	DIR		*dir;
	char str[100];
	int nbLettresFichier;

	dir = opendir(cheminRepertoire);

	if(dir == NULL)
	{
		perror("ERREUR OUVERTURE DE REPERTOIRE\n");
		exit(-1);
	}
	
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
					str[0]='\0';
					nbLettresFichier = 0; 
					while((d->d_name)[nbLettresFichier] != '\0')
					{
						nbLettresFichier++;
					}
					strcpy(str, d->d_name);
					strcat(str, "|");
					char buf[PATH_MAX + 1];
					realpath(d->d_name, buf);
					get_heure_modif_fichier(buf,modifTime);
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
	{
		perror("ERREUR OUVERTURE DE FICHIER\n");
		exit(-1);
	}
	else{
		while (fgets(ligne, 256, fichier) != NULL)
		{
			char * nomFichier = strtok(ligne, "|");
			printf("%s\n", nomFichier);
			char * dateFichier = strtok(NULL, "|");
			printf("%s\n", dateFichier);
			struct tm time;
			strptime(dateFichier,"%s",&time);
			time_t temps = mktime(&time);
			printf("%s\n",ctime(&temps));
		} 
	}
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
			char * nomFichier = strtok(ligne, "|");
			char typeModif = strtok(NULL, "|")[0];
			if(typeModif == 'M')
			{
				printf("%s a été modifié\n", nomFichier);
				modifier++;
			}
			else
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
	int nbCaracteres = compte_nb_caracteres_fichier("./modif.txt"); // ./logs.txt

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



int compte_nb_caracteres_fichier(char * cheminFichier)
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
			for(i=0; ligne[i+1] != '\0' ; i++){}
			nbCaracteres += i;
		}
	}

	fclose(fichier);
	return nbCaracteres;
}





