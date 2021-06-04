#include <sys/types.h>	//pour le type pid_t (entier sur 32bits sur la plupart des UNIX
#include <unistd.h>		//pour le fork()
#include <stdio.h>		//perror() et printf()
#include <stdlib.h>		//pour utiliser le rand() => man 3 rand
#include <time.h>		//pour utiliser le time()
#include <sys/wait.h>
#include <pthread.h>
#include <stdbool.h>

#define ITERATIONS 10

int var_globale = 10;
bool serveurIntegrationStatut = false;
bool serveurBackupStatut = false;


void * serveurProduction(){
   /*while(1){
	   printf("Serveur de Production\n");
	   sleep(2);
	}*/
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
			printf("serveurIntegration\n");
			
		}
	//}
}

void * serveurBackUp(){
   /*while(1){
	   printf("serveurBackUp\n");
	   sleep(2);
	}*/
}

int main(int nbarg, char* argv[]){

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