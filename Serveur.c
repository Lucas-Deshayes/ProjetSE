#include "Serveur.h"
#include "synchro_liste.h"
#include "cpyListe.h"

static pthread_mutex_t  mutexDossierProduction = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t  mutexDossierBackUp = PTHREAD_MUTEX_INITIALIZER;

bool serveurProductionStatut = false;
bool serveurBackupStatut = false;

void * serveurIntegration(){
	
	int rdm;
	
	while (1){
		while (serveurProductionStatut){
			rdm = rand()%5;
			switch (rdm){
			case 0:
				printf("Serveur Integration - Test serveur\n");
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
			pthread_mutex_lock(& mutexDossierProduction);
			pthread_mutex_lock(& mutexDossierBackUp);
			synchroBackUpToProduction();
			pthread_mutex_unlock(& mutexDossierProduction);
			pthread_mutex_unlock(& mutexDossierBackUp);
			rdm = rand()%5;
			sleep(rdm);	
		}

		if (serveurProductionStatut == false && serveurBackupStatut == false){
			printf("Aucun serveur ne tourne\n");
		}

		printf("serveurProductionStatut : %d\n",serveurProductionStatut);
		sleep(4);
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
				printf("serveurIntegration - Ajout fichier\n");
				ajout_fichier(d);
				pthread_mutex_unlock(& mutexDossierProduction);
				break;
			case 1:
				// 1 - Ecrire
				pthread_mutex_lock(& mutexDossierProduction);
				printf("serveurIntegration - Ecrire fichier\n");
				pthread_mutex_unlock(& mutexDossierProduction);
				break;
			case 2:
				// 2 - Lire
				pthread_mutex_lock(& mutexDossierProduction);
				printf("serveurIntegration - Lire fichier\n");
				pthread_mutex_unlock(& mutexDossierProduction);
				break;
			case 3:
				printf("serveurIntegration - Arret en cours\n");
				serveurProductionStatut = false;
				break;
			default:
				printf("serveurIntegration - Ne rien faire\n");
				break;
			}
			rdm = rand()%5;
			sleep(rdm);			
		}
		rdm = rand()%5;
		if (rdm > 3){
			serveurProductionStatut = true;
			printf("serveurIntegration - Redemarage\n");
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
						printf("serveurBackUp - Ajout fichier\n");
						ajout_fichier(d);
						pthread_mutex_unlock(& mutexDossierBackUp);
						break;
					case 1:
						// 1 - Ecrire
						pthread_mutex_lock(& mutexDossierBackUp);
						printf("serveurBackUp - Ecrire fichier\n");
						pthread_mutex_unlock(& mutexDossierBackUp);
						break;
					case 2:
						// 2 - Lire
						pthread_mutex_lock(& mutexDossierBackUp);
						printf("serveurBackUp - Lire fichier\n");
						pthread_mutex_unlock(& mutexDossierBackUp);
						break;	
					default:
						printf("serveurBackUp - Ne rien faire\n");
						break;
				}
				rdm = rand()%5;
				sleep(rdm);			
			}
		}
	}
	return 0;
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
	//printf("%s\n\n",nomFichier);

	// creation du texte aleatoire
	int tailleTexteFichier = random_intervalle(50,200);
	char texteFichier[tailleTexteFichier];
	random_string(texteFichier,tailleTexteFichier,false);
	//printf("%s\n",texteFichier);

	//creation du fichier dans le dossier choisit
	FILE* fichier = NULL;
	char dossier[50];
	dossier[0] = '\0';
	if(d == DossierBackUp)
		strcat(dossier,"DossierBackUp");
	else if(d == DossierProduction) 
		strcat(dossier,"DossierProduction");

	strcat(dossier,nomFichier);
	printf("%s\n",dossier);
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

void synchroProductionToBackUp (){

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

int main(int nbarg, char* argv[]){
	
	time_t seed;
	seed = time(NULL);
	srand(seed);
	
	pthread_t tid1;
	pthread_create(&tid1,NULL,serveurIntegration,NULL);

	pthread_t tid2;
	serveurProductionStatut = true;
	pthread_create(&tid2,NULL,serveurProduction,NULL);

	pthread_t tid3;
	serveurBackupStatut = true;
	pthread_create(&tid3,NULL,serveurBackUp,NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
}