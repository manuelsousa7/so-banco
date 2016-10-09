/******************************************************************************************
* File Name:    main.c
* Author:       Goncalo Marques (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Hashtags - IST/IAED - 2015/2016 2º Semestre
* SYNOPSIS:     #include <stdio.h>
                #include <string.h>  - strdup
                #include <stdlib.h>  - qsort
                #include <stdbool.h> - bool's
                #include "avl.h" - toda a estrura da Arvore AVL
                #include "auxiliares.h" - funcoes auxiliares a main 
* DESCRIPTION:  funcao main
* DIAGNOSTICS:  tested
* USAGE:        make clean
                make all
                make run
*****************************************************************************************/

#include "commandlinereader.h"
#include "contas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_AGORA "agora"
#define COMANDO_SAIR_AGORA "sairAgora"

#define MAXARGS 3
#define BUFFER_SIZE 100
#define MAXCHILDS 20

typedef struct PID{
    pid_t pid;
    int estado;
}pids;

/******************************************************************************************
* main()
*
* Arguments: nenhum
* Returns: 0
* Description:  ajhsgdhjsabjdbas
*****************************************************************************************/
int main (int argc, char** argv) {

    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    int numPids=0;
    inicializarContas();

    printf("Bem-vinda/o ao i-banco\n\n");
      
    while (1) {
        int numargs;
        pids pids[MAXCHILDS];
        numargs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);
        int estado,now;
        /* EOF (end of file) do stdin ou comando "sair" */
        if (numargs < 0 ||
            (numargs > 0  && (strcmp(args[0], COMANDO_SAIR) == 0))) {
            if (numargs < 2) {

        } else if (numargs == 2 && strcmp(args[1], COMANDO_AGORA) == 0) {
                for(int i=0;i<numPids;i++){
                    kill(pids[i].pid,SIGUSR1);
                }    
            } else {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SAIR);
                continue;
            }
            for(int i=0;i<numPids;i++){
                    now = waitpid(pids[i].pid,&estado,0); //0 OK, -1 NOT OK
                    pids[i].estado=estado;
            } 
            printf("i-banco vai terminar.\n--\n");
            for(int i=0;i<numPids;i++){
                printf("FILHO TERMINADO (PID=%d; terminou %s)\n",pids[i].pid,(pids[i].estado >= 0) ? "normalmente" : "abruptamente");
            }
            printf("--\n");
            exit(EXIT_SUCCESS); 
    
        }
        else if (numargs == 0)
            /* Nenhum argumento; ignora e volta a pedir */
            continue;
            
        /* Debitar */
        else if (strcmp(args[0], COMANDO_DEBITAR) == 0) {
            int idConta, valor;
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_DEBITAR);
	           continue;
            }

            idConta = atoi(args[1]);
            valor = atoi(args[2]);

            if (debitar (idConta, valor) < 0)
	           printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, idConta, valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, idConta, valor);
    }

    /* Creditar */
    else if (strcmp(args[0], COMANDO_CREDITAR) == 0) {
        int idConta, valor;
        if (numargs < 3) {
            printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_CREDITAR);
            continue;
        }

        idConta = atoi(args[1]);
        valor = atoi(args[2]);

        if (creditar (idConta, valor) < 0)
            printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, idConta, valor);
        else
            printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, idConta, valor);
    }

    /* Ler Saldo */
    else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
        int idConta, saldo;

        if (numargs < 2) {
            printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
            continue;
        }
        idConta = atoi(args[1]);
        saldo = lerSaldo (idConta);
        if (saldo < 0)
            printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, idConta);
        else
            printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, idConta, saldo);
    }

    /* Simular */
    else if (strcmp(args[0], COMANDO_SIMULAR) == 0) {
        int anos;
        pid_t pid;
        if ((anos = atoi(args[1])) < 0){
            printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SIMULAR);
        } else {
            pid = fork();
            if(pid < 0){
                printf("Erro");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                simular(anos);
                exit(EXIT_SUCCESS);
            } else if (pid > 0){
                pids[numPids++].pid = pid;
            }
        }
        continue;
    }
    else {
      printf("Comando desconhecido. Tente de novo.\n");
    }

  } 
}

