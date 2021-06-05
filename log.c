#include "log.h"

void WriteLog(const char* log){
	FILE* log_file= NULL;
	log_file = fopen("fichier_log.txt", "a");
	if (log_file != NULL)
		{
			fputs("\n", log_file);
			fputs(log, log_file);
			fclose(log_file); 
	}
	else{
		printf("Erreur d'enregistrement de log");
	}	
}
