#include "log.h"

void WriteLog(char* log){

	FILE* log_file= NULL;
	log_file = fopen("log.txt", "a");
	if (log_file != NULL)
		{
			time_t t;   // variable pour le temps
			time(&t);
			char* heure = ctime(&t);   // mettre le temps dans une variable de type string
			strcat(heure, log);
			fputs(heure, log_file);
			fclose(log_file);
	}
	else{
		printf("Erreur d'enregistrement de log");
	}
}
