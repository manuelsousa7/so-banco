all: i-banco i-banco-terminal
i-banco: contas.o i-banco.o parte234.o parte4.o hashtable.o
	gcc -pthread -o i-banco contas.o i-banco.o parte234.o parte4.o hashtable.o

i-banco-terminal: commandlinereader.o i-banco-terminal.o contas.o parte4.o
	gcc -pthread -o i-banco-terminal commandlinereader.o i-banco-terminal.o contas.o parte4.o

commandlinereader.o : commandlinereader.c commandlinereader.h
	gcc $(CFLAGS) -c commandlinereader.c

contas.o : contas.c contas.h
	gcc $(CFLAGS) -c contas.c

parte4.o : parte4.c parte4.h
	gcc $(CFLAGS) -c parte4.c

parte234.o : parte234.c parte234.h
	gcc $(CFLAGS) -c parte234.c

hashtable.o : hashtable.c hashtable.h
	gcc $(CFLAGS) -c hashtable.c

i-banco-terminal.o: i-banco-terminal.c commandlinereader.h contas.h parte4.h
	gcc $(CFLAGS) -c i-banco-terminal.c

i-banco.o: i-banco.c contas.h parte234.h parte4.h hashtable.h
	gcc $(CFLAGS) -c i-banco.c

run-servidor: i-banco
	./i-banco

run-cliente: i-banco-terminal
	./i-banco-terminal i-banco-pipe

clean:
	rm -f *.o *.txt *server_to_client_fifo_* i-banco i-banco-terminal i-banco-pipe

CFLAGS = -g -Wall -pedantic -pthread -lpthread
