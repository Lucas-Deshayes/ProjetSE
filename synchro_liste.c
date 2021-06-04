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
void enregistre_contenu_rep(char * cheminRepertoire, char * fichierEnregistrement, char * modifTime);
void compare_deux_repertoires(char * cheminFichier1,char * cheminFichier2);

int main()
{

	char modifTime[16];

	printf("\n--- Serveur d'intégration: ---\n");

	/*int nbLignes = compte_lignes_fichier("ancienRep.txt");
	printf("yoo: %d", nbLignes);*/
	enregistre_contenu_rep(".", "ancienRep.txt", modifTime);
	enregistre_contenu_rep("../test", "nouveauRep.txt", modifTime);

	compare_deux_repertoires("ancienRep.txt","nouveauRep.txt");

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


void get_heure_modif_fichier(char* fichier,char * modifTime)
{
	struct tm* lt;
	struct stat attr;
    stat(fichier, &attr);
	lt = localtime(&attr.st_mtime);
	strftime(modifTime, sizeof(modifTime) * 15, "%s", lt);
	printf("CHEMIN : %s\n",fichier);
	printf("TEMPS : %s\n",ctime(&attr.st_mtime));
}


void enregistre_contenu_rep(char * cheminRepertoire, char * fichierEnregistrement, char * modifTime)
{
	struct dirent 	*d;
	DIR		*dir;
	char str[100];
	int nbLettresFichier;

	dir = opendir(cheminRepertoire);

	if(dir == NULL)
		perror("erreur\n");
	else
	{
		FILE * fichier = NULL;
		fichier = fopen(fichierEnregistrement, "w");

		if(fichier == NULL)
			perror("erreur");
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
					strcat(str, d->d_name);
					
					strcat(str, "|");
					char buf[PATH_MAX + 1];
					char* chemin = malloc((sizeof(d->d_name)+sizeof(cheminRepertoire) + 1)*sizeof(char));
					chemin[0] = '\0';
					//printf("%s\n",cheminRepertoire);
					strcat(chemin,cheminRepertoire);
					//printf("%s\n",chemin);
					strcat(chemin,"/");
					strcat(chemin,d->d_name);
					//printf("%s\n",chemin);

					//printf("PATH : %s\n",realpath(chemin, buf));
					realpath(chemin, buf);
					get_heure_modif_fichier(buf,modifTime);
					strcat(str, modifTime);
					//printf("%s\n",str);
					fputs(strcat(str, "\n"), fichier);
				}
			}
		}
		fclose(fichier);
	}


	if (closedir(dir) == -1)
		printf("erreur\n");

}


void compare_deux_repertoires(char * cheminFichier1,char* cheminFichier2) //après -> rajouter cheminFichier2
{
	char ligne1[256];
	char ligne2[256];
	FILE * fichier1 = NULL;
	FILE * fichier2 = NULL;
	fichier1 = fopen(cheminFichier1, "r");
	fichier2 = fopen(cheminFichier2,"r");
	FILE * sortie = fopen("difference.txt","w");
	if(fichier1 == NULL || fichier2 == NULL || sortie == NULL)
	{
		perror("erreur //THIS");
		exit(-1);
	}
	else{
		int ligne = 0;
		while (fgets(ligne1, 256, fichier1) != NULL && fgets(ligne2,sizeof(ligne2),fichier2) != NULL)
		{
			char * nomFichier1 = strtok(ligne1, "|");
			char * dateFichier1 = strtok(NULL, "|");
			char * nomFichier2 = strtok(ligne2, "|");
			char * dateFichier2 = strtok(NULL, "|");
			printf("COMPARAISON : %s %s\n",nomFichier1,nomFichier2);
			//printf("POINTER : %ld\n",ftell(fichier2));

			if(strcmp(nomFichier1,nomFichier2) == 0)
			{
				struct tm time;
				strptime(dateFichier1,"%s",&time);
				time_t temps1 = mktime(&time);
				strptime(dateFichier2,"%s",&time);
				time_t temps2 = mktime(&time);
				if(difftime(temps1,temps2) != 0)
				{
					char str[100];
					str[0] ='\0';
					strcat(str,nomFichier1);
					strcat(str, "|");
					strcat(str,"M\n");
					fputs(str,sortie);
				}
				ligne++;
			}
			else //FICHIER DIFFERENT
			{
				char str[100];
				str[0] ='\0';
				strcat(str,nomFichier1);
				strcat(str, "|");
				strcat(str,"C\n");
				fputs(str,sortie);
				//printf("%ld\n",ftell(fichier2));
				fseek(fichier2,0,SEEK_SET);
				for (int i = 0; i < ligne; i++)
				{
					fgets(ligne2,sizeof(ligne2),fichier2);
				}
				//printf("%ld\n",ftell(fichier2));
			}

		}
	}
	fclose(fichier1);
	fclose(fichier2);
	fclose(sortie);
}


