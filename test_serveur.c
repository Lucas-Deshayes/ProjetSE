#include "test_serveur.h"

void testServer(bool serverProd, bool serverBackup){

	if (serverProd == true && serverBackup == true){
		printf("Test Serveur - ServeurProd : ON - Serveur BackUp : ON\n");
	}
	else if (serverProd == true){
		printf("Test Serveur - ServeurProd : ON - Serveur BackUp : OFF\n");
	}
	else if (serverBackup == true){
		printf("Test Serveur - ServeurProd : OFF - Serveur BackUp : ON\n");
	}
}
