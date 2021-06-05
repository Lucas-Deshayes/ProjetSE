#include "Serveur.h"
#include "synchro_liste.h"
#include "cpyListe.h"
#include "test_serveur.h"
#include "log.h"

static pthread_mutex_t  mutexDossierProduction = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t  mutexDossierBackUp = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t  mutexFichierLogs = PTHREAD_MUTEX_INITIALIZER;


bool serveurProductionStatut = false;
bool serveurBackupStatut = false;
int fichierRandom = 0;


void * serveurIntegration(){
	
	int rdm;
	
	while (1){
		while (serveurProductionStatut){
			rdm = rand()%5;
			switch (rdm){
			case 0:
				printf("Serveur Integration - Test serveur\n");
				testServer(serveurProductionStatut,serveurBackupStatut);
				break;
			case 1:
				printf("Serveur Integration - Stat serveur\n");
				break;
			case 2:
				//pthread_mutex_lock(& mutexFichierLogs);
				//WriteLog("Serveur Integration -");
				//pthread_mutex_unlock(& mutexFichierLogs);
				break;
			default:
				pthread_mutex_lock(& mutexDossierProduction);
				pthread_mutex_lock(& mutexDossierBackUp);
				printf("Serveur Integration - Synchro ProductionToBackUp\n");
				synchroProductionToBackUp();
				pthread_mutex_unlock(& mutexDossierProduction);
				pthread_mutex_unlock(& mutexDossierBackUp);
				break;
			}
			rdm = rand()%5;
			sleep(rdm);	
		}

		while (serveurBackupStatut && !serveurProductionStatut){
			rdm = rand()%5;
			switch (rdm){
			case 0:
				printf("Serveur Integration - Test serveur\n");
				testServer(serveurProductionStatut,serveurBackupStatut);
				break;
			case 1:
				printf("Serveur Integration - Stat serveur\n");
				break;
			case 2:
				printf("Serveur Integration - Logs\n");
				break;
			default:
				pthread_mutex_lock(& mutexDossierProduction);
				pthread_mutex_lock(& mutexDossierBackUp);
				printf("Serveur Integration - Synchro BackUpToProduction\n");
				synchroBackUpToProduction();
				pthread_mutex_unlock(& mutexDossierProduction);
				pthread_mutex_unlock(& mutexDossierBackUp);
				break;
			}
			rdm = rand()%5;
			sleep(rdm);
		}

		if (serveurProductionStatut == false && serveurBackupStatut == false){
			printf("Aucun serveur ne tourne\n");
		}
	}
	return 0;
}

void * serveurProduction(){
	enum dossiers d = DossierProduction;
   	while(1) {
		int rdm;
		while(serveurProductionStatut){
			rdm = rand()%5;
			switch (rdm){
			case 0:
				// 0 - Ajout
				pthread_mutex_lock(& mutexDossierProduction);
				printf("Serveur Production - Ajout fichier\n");
				ajout_fichier(d);
				pthread_mutex_unlock(& mutexDossierProduction);
				break;
			case 1:
				// 1 - Ecrire
				pthread_mutex_lock(& mutexDossierProduction);
				printf("Serveur Production - Ecrire fichier\n");
				pthread_mutex_unlock(& mutexDossierProduction);
				break;
			case 2:
				// 2 - Lire
				pthread_mutex_lock(& mutexDossierProduction);
				printf("Serveur Production - Lire fichier\n");
				pthread_mutex_unlock(& mutexDossierProduction);
				break;
			case 3:
				printf("Serveur Production - Arret en cours\n");
				serveurProductionStatut = false;
				break;
			default:
				printf("Serveur Production - Ne rien faire\n");
				break;
			}
			rdm = rand()%5;
			sleep(rdm);
		}
		rdm = rand()%5;
		if (rdm > 3){
			serveurProductionStatut = true;
			printf("Serveur Production - Redemarage\n");
		}
	}
	return 0;
}

void * serveurBackUp(){
	enum dossiers d = DossierBackUp;
   	while(1) {
		while(serveurBackupStatut){
			if(!serveurProductionStatut){
				int rdm = rand()%5;
				switch (rdm){
					case 0:
						// 0 - Ajout
						pthread_mutex_lock(& mutexDossierBackUp);
						printf("Serveur BackUp - Ajout fichier\n");
						ajout_fichier(d);
						pthread_mutex_unlock(& mutexDossierBackUp);
						break;
					case 1:
						// 1 - Ecrire
						pthread_mutex_lock(& mutexDossierBackUp);
						printf("Serveur BackUp - Ecrire fichier\n");
						pthread_mutex_unlock(& mutexDossierBackUp);
						break;
					case 2:
						// 2 - Lire
						pthread_mutex_lock(& mutexDossierBackUp);
						printf("Serveur BackUp - Lire fichier\n");
						pthread_mutex_unlock(& mutexDossierBackUp);
						break;	
					default:
						printf("Serveur BackUp - Ne rien faire\n");
						break;
				}
				rdm = rand()%5;
				sleep(rdm);			
			}
		}
	}
	return 0;
}




void synchroProductionToBackUp () {

	char* cheminP = "./DossierProduction";
    char* cheminB = "./DossierBackUp";

    enregistre_contenu_rep(cheminB,"ancienRep.txt");
    enregistre_contenu_rep(cheminP,"nouveauRep.txt");

    compare_deux_repertoires("nouveauRep.txt","ancienRep.txt"); // Copie de Production vers BackUp
    copie_liste_fichiers(cheminP,cheminB);

}

void synchroBackUpToProduction (){

	char* cheminP = "./DossierBackUp";
    char* cheminB = "./DossierProduction";

    enregistre_contenu_rep(cheminB,"ancienRep.txt");
    enregistre_contenu_rep(cheminP,"nouveauRep.txt");

    compare_deux_repertoires("nouveauRep.txt","ancienRep.txt"); // Copie de BackUp vers Production
    copie_liste_fichiers(cheminP,cheminB);

}



void random_string(char *s, int len, bool fichier) {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int i,debut;
	if (fichier){
		debut=1;
		s[0]='/';
	}
	else 
		debut=0;

	for (i=debut; i < len; ++i) {
        s[i] = alphabet[rand() % (sizeof(alphabet) - 1)];
    }
    s[len] = 0;
}

int random_intervalle(int a, int b) {
	return (rand()%(b-a)) + a;
}

void ajout_fichier(enum dossiers d) {
	// creation d'un nom de fichier aleatoire
	int tailleNomFichier = random_intervalle(5,15);
	char nomFichier[tailleNomFichier];
	random_string(nomFichier,tailleNomFichier,true);
	strcat(nomFichier,".txt");

	// creation du texte aleatoire
	int tailleTexteFichier = random_intervalle(10,25);
	char texteFichier[tailleTexteFichier];
	random_string(texteFichier,tailleTexteFichier,false);
	strcat(texteFichier,"\n");

	//creation du fichier dans le dossier choisit
	FILE* fichier = NULL;
	char dossier[50];
	dossier[0] = '\0';
	if(d == DossierBackUp)
		strcat(dossier,"DossierBackUp");
	else if(d == DossierProduction) 
		strcat(dossier,"DossierProduction");

	strcat(dossier,nomFichier);
	fichier = fopen(dossier, "w");

	// ecriture dans le fichier
	if (fichier != NULL) {
		fputs(texteFichier, fichier);
		fclose(fichier);
	}
	else {
		printf("Impossible d'ouvrir le fichier");
	}
}

int compte_nombre_fichier(char * path) {
	DIR *folder;
    struct dirent *entry;
    int files = 0;
    folder = opendir(path);
    if(folder == NULL)
    {
        perror("Unable to read directory");
        return(1);
    }
    while( (entry=readdir(folder)) ) {
        files++;
    }
	closedir(folder);
    return files-2;
}

void modifier_fichier(enum dossiers d) {
	DIR *folder;
    struct dirent *entry;
    int files = 0;
	
	// path du dossier 
	char pathDossier[50];
	strcat(pathDossier,"./");
	if(d == DossierBackUp)
		strcat(pathDossier,"DossierBackUp");
	else if(d == DossierProduction) 
		strcat(pathDossier,"DossierProduction");

	// fichier aleatoire
	int nbfichiers = compte_nombre_fichier(pathDossier);
	if(nbfichiers==1)
		fichierRandom = 0;
	else
		fichierRandom = random_intervalle(0,nbfichiers);


    folder = opendir(pathDossier);
    if(folder == NULL)
    {
        perror("Unable to read directory");
    }
	char pathFichier[50];
	pathFichier[0] = '\0';   

	// parcourt des fichiers
    while( (entry=readdir(folder)) )
    {
		if (strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
		
			if(fichierRandom==files){
				strcpy(pathFichier,pathDossier);
				strcat(pathFichier,"/");
				strcat(pathFichier,entry->d_name);
			}
			files++;
		}
    }
    closedir(folder);
	ajout_fin_fichier(pathFichier);
}


void ajout_fin_fichier(char * pathFichier) {

	FILE* fichier = NULL;
	fichier = fopen(pathFichier, "a");

	// creation du texte aleatoire
	int tailleTexte = random_intervalle(10,25);
	char texteAjout[tailleTexte];
	random_string(texteAjout,tailleTexte,false);
	strcat(texteAjout,"\n");
	
	// ajouter a la suite du fichier
	if (fichier != NULL) {
		
		fputs(texteAjout, fichier);
		fclose(fichier);
	}
	else {
		printf("Impossible d'ouvrir le fichier");
	}
}

void compte_nombre_caractere(char * pathFichier) {
	int compteur = 0;
	FILE * file;
	
	// ouverture du fichier
	if ((file = fopen(pathFichier, "r")) == NULL)
		perror("Erreur à l'ouverture du fichier");
	else {
	// parcours du fichier
	while(fgetc(file) != EOF)
		compteur ++;
	}
	
	printf("%d caracteres\n", compteur);
	fclose(file);
}

void lecture_fichier(enum dossiers d) {
	DIR *folder;
    struct dirent *entry;
    int files = 0;
	
	// path du dossier 
	char pathDossier[50];
	strcat(pathDossier,"./");
	if(d == DossierBackUp)
		strcat(pathDossier,"DossierBackUp");
	else if(d == DossierProduction) 
		strcat(pathDossier,"DossierProduction");

	// fichier aleatoire
	int nbfichiers = compte_nombre_fichier(pathDossier);
	if(nbfichiers==1)
		fichierRandom = 0;
	else
		fichierRandom = random_intervalle(0,nbfichiers);

    folder = opendir(pathDossier);
    if(folder == NULL)
    {
        perror("Unable to read directory");
    }
	char pathFichier[50];
	pathFichier[0] = '\0';   

	// parcourt des fichiers
    while( (entry=readdir(folder)) )
    {
		if (strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
		
			if(fichierRandom==files){
				printf("--> fichier : %s - ",entry->d_name);
				strcpy(pathFichier,pathDossier);
				strcat(pathFichier,"/");
				strcat(pathFichier,entry->d_name);
			}
			files++;
		}
    }
    closedir(folder);
	compte_nombre_caractere(pathFichier);
}


int main(int nbarg, char* argv[]){

	time_t seed;
	seed = time(NULL);
	srand(seed);

	WriteLog("Serveur Production - ");
	/*enum dossiers d = DossierProduction; 
	lecture_fichier(d);*/

	sleep(5);
	WriteLog("Serveur Integration - ");

	sleep(3);
	WriteLog("Serveur BackUp - ");

	/*enum dossiers d = DossierProduction; 
	modifier_fichier(d);*/

	/*pthread_t tid1;
	pthread_create(&tid1,NULL,serveurIntegration,NULL);

	pthread_t tid2;
	serveurProductionStatut = true;
	pthread_create(&tid2,NULL,serveurProduction,NULL);

	pthread_t tid3;
	serveurBackupStatut = true;
	pthread_create(&tid3,NULL,serveurBackUp,NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);*/
}