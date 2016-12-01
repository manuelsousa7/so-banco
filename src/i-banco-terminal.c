/******************************************************************************************
* File Name:    i-banco.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <stdio.h> - I/O regular
                #include <signal.h> - signal(), kill()
                #include <unistd.h> - fork()
                #include <sys/wait.h> - waitpid()
                #include "contas.h" - Prototipos de todas as operações relacionadas com contas
                #include "commandlinereader.h" - Prototipos das funcoes de leitura dos comandos
                #include "parte1.h" - Prototipos das funcoes da parte1 - Defines (macros) dos comandos
                #include "parte2e3.h" - Prototipos e Estruturas usadas na entrega 2 e 3
* DESCRIPTION:  funcao main (i-banco)
* DIAGNOSTICS:  tested
* USAGE:        make clean
                make all
                make run
*****************************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "contas.h"
#include "commandlinereader.h"
#include "parte1.h"
#include "parte234.h"
#include "parte4.h"

/* Constantes */
#define MAXARGS 4
#define BUFFER_SIZE 100


void sendComandToServer(int client_to_server, int server_to_client,  int idConta, int idConta2, int valor, int OP) {
    char response[BUFFER_SIZE];
    comando_t comando;
    comando.operacao = OP;
    comando.valor = valor;
    comando.idConta = idConta;
    comando.idConta2 = idConta2;
    comando_t *pointer;
    write(client_to_server, &comando, sizeof(pointer));
    //read(server_to_client,*response,sizeof(response));
    //return response;
    puts(response);
}


/******************************************************************************************
* main()
*
* Arguments:    Nenhum
* Returns: int  0
* Description:  Leitura dos comandos do banco e criação de processos nas simulações
*****************************************************************************************/
int main (int argc, char** argv) {
    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    int sairAgora = 0;
    int numargs = 0;
    int client_to_server;
    char *myfifo = "/tmp/client_to_server_fifo";

    int server_to_client;
    char *myfifo2 = "/tmp/server_to_client_fifo";

    client_to_server = open(myfifo, O_WRONLY);
    server_to_client = open(myfifo2, O_RDONLY);

    printf("Bem-vinda/o ao i-banco\n\n");

    while (1) {
        numargs = readLineArguments(args, MAXARGS + 1, buffer, BUFFER_SIZE);

        if (numargs < 0 || (numargs > 0  && (strcmp(args[0], COMANDO_SAIR) == 0))) {
            if (numargs < 2) {

                /* Sair Agora */
            } else if (numargs == 2 && strcmp(args[1], COMANDO_AGORA) == 0) {
                sairAgora = 1;
            } else {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SAIR);
                continue;
            }



            //TODO
            if (sairAgora == 1) {
                puts("sendComandToServer(sairagoa)");
            } else {
                puts("endComandToServer(sair)");
            }



            close(client_to_server);
            close(server_to_client);
            exit(EXIT_SUCCESS);

        }

        else if (numargs == 0)
            /* Nenhum argumento; ignora e volta a pedir */
            continue;

        /* Debitar */
        else if (strcmp(args[0], COMANDO_DEBITAR) == 0) {
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_DEBITAR);
                continue;
            }
            sendComandToServer(client_to_server, server_to_client, atoi(args[1]), -1, atoi(args[2]), OP_DEBITAR);
        }

        /* Creditar */
        else if (strcmp(args[0], COMANDO_CREDITAR) == 0) {
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_CREDITAR);
                continue;
            }

            sendComandToServer(client_to_server, server_to_client, atoi(args[1]), -1, atoi(args[2]), OP_CREDITAR);
        }

        /* Ler Saldo */
        else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
            if (numargs < 2) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
                continue;
            }
            sendComandToServer(client_to_server, server_to_client, atoi(args[1]), -1, -1, OP_LERSALDO);
        }

        /* Tranferir */
        else if (strcmp(args[0], COMANDO_TRANSFERIR) == 0) {
            if (numargs != 4) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_TRANSFERIR);
                continue;
            }
            sendComandToServer(client_to_server, server_to_client, atoi(args[1]), atoi(args[2]), atoi(args[3]), OP_TRANSFERIR);
        }
        /* Saco Azul */
        else if (strcmp(args[0], COMANDO_SACOAZUL) == 0) {
            if (numargs != 2) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SACOAZUL);
                continue;
            }
            sendComandToServer(client_to_server, server_to_client, atoi(args[1]), -1, -1, OP_SACOAZUL);
        }

        /* Simular */
        else if (strcmp(args[0], COMANDO_SIMULAR) == 0 && numargs == 2) {
            printf("adsasdadsadsads\n");
            continue;
        } else {
            printf("Comando desconhecido. Tente de novo.\n");
        }

    }
}