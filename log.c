#include "log.h"

void WriteLog(char* log){

	time_t timestamp = time( NULL );
    struct tm * pTime = localtime( & timestamp );

    char buffer[80];
    strftime( buffer, 80, "%d/%m/%Y %H:%M:%S", pTime );
    printf( "Date and french time : %s\n", buffer );

	//strcat(log,buffer);

	FILE* log_file= NULL;
	log_file = fopen("log.txt", "a");
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
