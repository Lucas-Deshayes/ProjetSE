CC=gcc
fichiers=Serveur.c 
exec=exec

all:
	$(CC) -Wall $(fichiers) -o $(exec) -pthread
	./exec

clean:
	rm -f $(exec)
