#include "Serveur.h"

static pthread_mutex_t  mutexDossierIntegration = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t  mutexDossierBackUp = PTHREAD_MUTEX_INITIALIZER;

bool serveurIntegrationStatut = false;
bool serveurBackupStatut = false;

void * serveurProduction(){
   /*while(1){
	   printf("Serveur de Production\n");
	   sleep(2);
	}*/
	return 0;
}

void * serveurIntegration(){
	//while(1) {
		while(serveurIntegrationStatut){
			int rdm = rand()%5;
			switch (rdm)
			{
			case 0:
				// 0 - Ajout
				pthread_mutex_lock(& mutexDossierIntegration);
				printf("serveurIntegration - Ajout fichier\n");
				pthread_mutex_unlock(& mutexDossierIntegration);
				break;
			case 1:
				// 1 - Ecrire
				pthread_mutex_lock(& mutexDossierIntegration);
				printf("serveurIntegration - Ecrire fichier\n");
				pthread_mutex_unlock(& mutexDossierIntegration);
				break;
			case 2:
				// 2 - Lire
				pthread_mutex_lock(& mutexDossierIntegration);
				printf("serveurIntegration - Lire fichier\n");
				pthread_mutex_unlock(& mutexDossierIntegration);
				break;
			default:
				printf("serveurIntegration - Ne rien faire\n");
				break;
			}
			rdm = rand()%5;
			sleep(rdm);			
		}
	//}
	return 0;
}

void * serveurBackUp(){
   //while(1) {
		while(serveurBackupStatut){
			int rdm = rand()%5;
			switch (rdm)
			{
			case 0:
				// 0 - Ajout
				pthread_mutex_lock(& mutexDossierBackUp);
				printf("serveurBackUp - Ajout fichier\n");
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
	//}
	return 0;
}


void ajout_fichier(char* dossier) {

}


int main(int nbarg, char* argv[]){

	time_t seed;
	seed = time(NULL);
	srand(seed);
	
	/*pthread_t tid1;
	pthread_create(&tid1,NULL,serveurProduction,NULL);

	pthread_t tid2;
	serveurIntegrationStatut = true;
	pthread_create(&tid2,NULL,serveurIntegration,NULL);


	pthread_t tid3;
	serveurBackupStatut = true;
	pthread_create(&tid3,NULL,serveurBackUp,NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);

	pthread_t tid2;
	serveurIntegrationStatut = true;
	pthread_create(&tid2,NULL,serveurIntegration,NULL);

	pthread_join(tid2,NULL);*/
}