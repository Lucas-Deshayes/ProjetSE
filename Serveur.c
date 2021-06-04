#include "Serveur.h"

#define ITERATIONS 10

int var_globale = 10;
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
			int rdm = rand()%4;
			switch (rdm)
			{
			case 0:
				// 0 - ajout
				printf("serveurIntegration - Ajout\n");
				break;
			case 1:
				// 1 - ecrire
				printf("serveurIntegration - Ecrire\n");
				break;
			case 2:
				// 2 - supprimer
				printf("serveurIntegration - Supprimer\n");
				break;
			case 3:
				// 3 ne rien faire
				printf("serveurIntegration - Ne rien faire\n");
				break;	
			default:
				break;
			}
			// switch random 
			rdm = rand()%5;
			sleep(rdm);			
		}
	//}
	return 0;
}

void * serveurBackUp(){
   /*while(1){
	   printf("serveurBackUp\n");
	   sleep(2);
	}*/
	return 0;
}



int main(int nbarg, char* argv[]){

	time_t seed;
	seed = time(NULL);
	srand(seed);
	
	pthread_t tid1;
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

}