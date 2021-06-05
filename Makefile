CC=gcc
fichiers=Serveur.c synchro_liste.c cpyListe.c test_serveur.c log.c
exec=exec

all:
	$(CC) -Wall $(fichiers) -o $(exec) -pthread
	./exec

clean:
	rm -f $(exec)
