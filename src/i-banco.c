/******************************************************************************************
* File Name:    i-banco.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <stdio.h> - I/O regular
                #include <string.h>  - char strings
                #include <stdlib.h>  - exit(), atoi()
                #include <unistd.h> - fork()
                #include <signal.h> - signal(), kill()
                #include <errno.h> - 
                #include <sys/wait.h> - waitpid()
                #include "commandlinereader.h" - Prototipos das funcoes de leitura dos comandos
                #include "contas.h" - Prototipos de todas as operações relacionadas com contas
* DESCRIPTION:  funcao main (i-banco)
* DIAGNOSTICS:  tested
* USAGE:        make clean
                make all
                make run
*****************************************************************************************/

/*Bibliotecas*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "commandlinereader.h"
#include "contas.h"

/*Macros - Comandos*/
#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_AGORA "agora"

/*Constantes*/
#define MAXARGS 3
#define BUFFER_SIZE 100
#define MAXCHILDS 20

/*Estruturas*/

typedef struct PID{
    pid_t pid;
    int estado;
    int signal;
}pids;

/******************************************************************************************
* main()
*
* Arguments:    Nenhum
* Returns: int  0
* Description:  Leitura dos comandos do banco e criação de de processos nas simulações
*****************************************************************************************/
int main (int argc, char** argv) {

    char *args[MAXARGS + 1];
    char buffer[BUFFER_SIZE];
    int numPids = 0;
    //pid_t wpid;
    inicializarContas();

    printf("Bem-vinda/o ao i-banco\n\n");
      
    while (1) {
        int numargs;
        pids pids[MAXCHILDS];
        numargs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);
        int estado, sairAgora = 0;
        /* EOF (end of file) do stdin ou comando "sair" */
        if (numargs < 0 || (numargs > 0  && (strcmp(args[0], COMANDO_SAIR) == 0))) {
            if (numargs < 2) {

            } else if (numargs == 2 && strcmp(args[1], COMANDO_AGORA) == 0) {
                sairAgora = 1;
                for(int i=0;i<numPids;i++){
                    if (kill(pids[i].pid,SIGUSR1) != 0)
                        printf("%s: Erro ao enviar sinal para o Processo.\n", strcat(COMANDO_SAIR , COMANDO_AGORA));
                }    
            } else {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SAIR);
                continue;
            }

            for(int i=0;i<numPids;i++){
                
                if(waitpid(pids[i].pid,&estado,0) == -1)
                    printf("%s: Erro ao terminar Processo.\n", (sairAgora == 1) ? strcat(COMANDO_SAIR , COMANDO_AGORA) : COMANDO_SAIR);
                if(errno == ECHILD || errno == EINTR || errno == EINVAL)
                    printf("%s: Erro ao terminar Processo.\n", (sairAgora == 1) ? strcat(COMANDO_SAIR , COMANDO_AGORA) : COMANDO_SAIR);
                if(WIFEXITED(estado) != 0){
                    if(WEXITSTATUS(estado) == EXIT_FAILURE)
                        pids[i].signal = 1;
                    else
                        pids[i].signal = 0;
                }
                pids[i].estado = WIFEXITED(estado) ? 1 : -1;
            } 
            for(int i=0;i<numPids;i++){
                if(pids[i].signal == 1){
                    printf("Simulacao terminada por signal\n");
                }
            }
            printf("i-banco vai terminar.\n--\n");

            for(int i=0;i<numPids;i++){
                printf("FILHO TERMINADO (PID=%d; terminou %s)\n",pids[i].pid,(pids[i].estado >= 0) ? "normalmente" : "abruptamente");
            }

            printf("--\n");
            sairAgora = 0;
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
                printf("%s: ERRO ao criar processo.ID do fork %d\n",COMANDO_SIMULAR,pid);
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                simular(anos);
                exit(EXIT_SUCCESS);
            } else if (pid > 0){
                pids[numPids++].pid = pid;
            }
        }
        continue;
    } else {
      printf("Comando desconhecido. Tente de novo.\n");
    }

  } 
}
