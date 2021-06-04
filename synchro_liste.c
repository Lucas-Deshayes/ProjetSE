#include "synchro_liste.h"
//Recupere la date de de derniere modification d'un fichier.
//En entrée on donne le chemin du fichier et un tableau de caractere poour stocker la valeur du temps en String.
void get_heure_modif_fichier(char* fichier,char * modifTime)
{
	struct tm* lt;
	struct stat attr;
    stat(fichier, &attr);
	lt = localtime(&attr.st_mtime);
	strftime(modifTime, sizeof(modifTime) * 15, "%s", lt);
	//printf("CHEMIN : %s\n",fichier);
	//printf("TEMPS : %s\n",ctime(&attr.st_mtime));
}

//Ecrit dans un fichier le contenu d'un répertoire selon le format NOMFICHIER|HeureDeModificationEnSeconde
void enregistre_contenu_rep(char * cheminRepertoire, char * fichierEnregistrement)
{
	char modifTime[16];
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
			while((d = readdir(dir)) != NULL)//Tant qu'il y a des fichiers dans le dossier. 
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
					strcat(chemin,cheminRepertoire);
					strcat(chemin,"/");
					strcat(chemin,d->d_name);


					realpath(chemin, buf);//Determaine le chemin absolue du fichier. ecrit le resultat dans buf.
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

//Compare deux répertoire et ecrit les difference dans un fichier difference.txt
//Le dossier 1 en entrée est prioritaire pour la comparaison.
//Donc si il y a un fichier manquant dans le dossier 2 alors c'est un ajout dans le dossier 2
//et pas une suppression dans le dossier 1.
void compare_deux_repertoires(char * cheminFichier1,char* cheminFichier2)
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
		while (fgets(ligne1, 256, fichier1) != NULL )
		{
			char * nomFichier1 = strtok(ligne1, "|");
			char * dateFichier1 = strtok(NULL, "|");
			if(fgets(ligne2,sizeof(ligne2),fichier2) != NULL)
			{
				char * nomFichier2 = strtok(ligne2, "|");
				char * dateFichier2 = strtok(NULL, "|");

				if(strcmp(nomFichier1,nomFichier2) == 0)//Le meme nom
				{
					struct tm time;
					strptime(dateFichier1,"%s",&time);
					time_t temps1 = mktime(&time);
					strptime(dateFichier2,"%s",&time);
					time_t temps2 = mktime(&time);
					if(difftime(temps1,temps2) != 0)//Si la derniere modification est plus recente.
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
					fseek(fichier2,0,SEEK_SET);
					for (int i = 0; i < ligne; i++)
					{
						fgets(ligne2,sizeof(ligne2),fichier2);
					}
				}
			}
			else
			{
				char str[100];
				str[0] ='\0';
				strcat(str,nomFichier1);
				strcat(str, "|");
				strcat(str,"C\n");
				fputs(str,sortie);
			}
		}
	}
	fclose(fichier1);
	fclose(fichier2);
	fclose(sortie);
}


