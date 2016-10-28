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
                #include "parte2.h" - Prototipos e Estruturas usadas na entrega 2 (tarefas)
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
#include "parte2.h"

/* Constantes */
#define MAXARGS 3
#define BUFFER_SIZE 100


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
    int numPids = 0;
    inicializarContas();
    inicializarThreadsSemaforos();

    printf("Bem-vinda/o ao i-banco\n\n");

    while (1) {
        int numargs;
        pids pids[MAXCHILDS];
        numargs = readLineArguments(args, MAXARGS + 1, buffer, BUFFER_SIZE);
        int estado, sairAgora = 0;
        /* EOF (end of file) do stdin ou comando "sair" , "sair agora"*/
        if (numargs < 0 || (numargs > 0  && (strcmp(args[0], COMANDO_SAIR) == 0))) {
            if (numargs < 2) {

                /* Sair Agora */
            } else if (numargs == 2 && strcmp(args[1], COMANDO_AGORA) == 0) {
                sairAgora = 1;
                //Ciclo que vai enviar um sinal individualmente para cada Processo Filho
                for (int i = 0; i < numPids; i++) {
                    if (kill(pids[i].pid, SIGUSR1) != 0) //Verifica se ocorreu um erro ao enviar um Sinal
                        printf("%s: Erro ao enviar sinal para o Processo.\n", strcat(COMANDO_SAIR , COMANDO_AGORA));
                }
            } else {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SAIR);
                continue;
            }

            /* Vai terminar e sincronizar todas as tarefas do sistema. Destroi tambem os respetivos semaforos */
            killThreadsSemaforos();

            /* Ciclo que vai terminar todos os Processos Filho */
            for (int i = 0; i < numPids; i++) {

                if (waitpid(pids[i].pid, &estado, 0) == -1) //Terminar Processo filho. Se ocorrer um erro vai cair no if statment
                    printf("%s: Erro ao terminar Processo.\n", (sairAgora == 1) ? strcat(COMANDO_SAIR , COMANDO_AGORA) : COMANDO_SAIR);
                if (WIFEXITED(estado) != 0) { //Se o processo sair com um exit corretamente (de que maneira for)
                    if (WEXITSTATUS(estado) == 2) //Vamos verificar se o exit retornou o termino por signal
                        printf("Simulacao terminada por signal\n");
                }
                pids[i].estado = WIFEXITED(estado) ? 1 : -1;
            }
            printf("i-banco vai terminar.\n--\n");
            for (int i = 0; i < numPids; i++) {
                printf("FILHO TERMINADO (PID=%d; terminou %s)\n", pids[i].pid, (pids[i].estado > 0) ? "normalmente" : "abruptamente");
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
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_DEBITAR);
                continue;
            }

            produtor(atoi(args[1]), atoi(args[2]), OP_DEBITAR);
        }

        /* Creditar */
        else if (strcmp(args[0], COMANDO_CREDITAR) == 0) {
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_CREDITAR);
                continue;
            }

            produtor(atoi(args[1]), atoi(args[2]), OP_CREDITAR);
        }

        /* Ler Saldo */
        else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
            if (numargs < 2) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
                continue;
            }
            produtor(atoi(args[1]), 0, OP_LERSALDO);
        }

        /* Simular */
        else if (strcmp(args[0], COMANDO_SIMULAR) == 0 && numargs == 2) {
            int anos;
            pid_t pid;
            if ((anos = atoi(args[1])) <= 0) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SIMULAR);
            } else {
                pid = fork();
                if (pid < 0) { // Erro ao fazer fork do processo PAI
                    printf("%s: ERRO ao criar processo.ID do fork %d\n", COMANDO_SIMULAR, pid);
                    exit(EXIT_FAILURE);
                } else if (pid == 0) { //Criou Processo filho com sucesso
                    simular(anos);
                    exit(EXIT_SUCCESS);
                } else if (pid > 0) { // Processo PAI
                    pids[numPids++].pid = pid; //Vamos guardar os PIDs de todos os processos filho que forem criados
                }
            }
            continue;
        } else {
            printf("Comando desconhecido. Tente de novo.\n");
        }

    }
}
