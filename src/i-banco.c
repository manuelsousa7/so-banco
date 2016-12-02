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
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "contas.h"
#include "commandlinereader.h"
#include "parte1.h"
#include "parte234.h"
#include "parte4.h"
#include "hashtable.h"


/* Constantes */
#define MAXARGS 4
#define BUFFER_SIZE 100


void han() {
    printf("asdads\n");
}

/******************************************************************************************
* main()
*
* Arguments:    Nenhum
* Returns: int  0
* Description:  Leitura dos comandos do banco e criação de processos nas simulações
*****************************************************************************************/
int main (int argc, char** argv) {
    comando_t comando;
    pids pids[MAXCHILDS];
    int numPids = 0;
    int i;
    //int terminals[100];
    //int nterminais = 0;
    dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
    dummyItem->data = -1;
    dummyItem->key = -1;

    inicializarContas();
    inicializarThreadsSemaforosMutexes();

    int client_to_server;
    int server_to_client;
    //char myfifo2[100];
    char fifoDelete[100];
    //char terminalPid[100];
    char *myfifo = "i-banco-pipe";
    if (unlink(myfifo) == -1) {
        printf("ajbdshajsbhdajbhds\n");
    }

    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    //char *myfifo2 = "/tmp/client_to_server_fifo";

    /* open, read, and display the message from the FIFO */
    client_to_server = open(myfifo, O_RDONLY);
    //printf("aaa %d\n", client_to_server);
    printf("Bem-vinda/o ao i-banco\n\n");

    while (1) {
        printf("apos while\n");
        //display();
        while (read(client_to_server, &comando, sizeof(comando)) <= 0);


        item = search(comando.terminalPid);
        printf("dasjbhasdhjbadsbhjasdjbh %d\n", comando.terminalPid);

        if (item == NULL) {
            printf("item==NULL ultrapassado\n");
            //snprintf(myfifo2, sizeof(myfifo2), "%s%d", "server_to_client_fifo_", comando.terminalPid);
            //printf("comando.path%s\n", (comando.path));

            //mkfifo(comando.path, 0777);
            printf("antes\n");
            server_to_client = open(comando.path, O_WRONLY);
            printf("apos\n");
            if (server_to_client == -1) {
                printf("ERRO\n");
            } else {
                printf("inseriu %d\n", server_to_client);
                insert(comando.terminalPid, server_to_client);
            }
        }

        if (comando.operacao == OP_SIMULAR) {
            int anos;
            pid_t pid;
            if ((anos = comando.idConta) <= 0) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SIMULAR);
            } else {
                /* Abrir */
                if (pthread_mutex_lock(&semExMut) != 0) {
                    printf("ERRO: pthread_mutex_lock - &semExMut\n");
                }

                /* Veririca se ha comandos no buffer */
                while (comandosNoBuffer != 0) {
                    pthread_cond_wait(&cheio, &semExMut); //Espera
                }

                pid = fork();

                /* Fechar */
                if (pthread_mutex_unlock(&semExMut) != 0) {
                    printf("ERRO: pthread_mutex_unlock - &semExMut\n");
                }

                if (pid < 0) { // Erro ao fazer fork do processo PAI
                    printf("%s: ERRO ao criar processo.ID do fork %d\n", COMANDO_SIMULAR, pid);
                    exit(EXIT_FAILURE);
                } else if (pid == 0) { //Criou Processo filho com sucesso
                    iniciaRedirecionarOutput();
                    simular(anos);
                    pararRedirecionarOutput();
                    exit(EXIT_SUCCESS);
                } else if (pid > 0) { // Processo PAI
                    pids[numPids++].pid = pid; //Vamos guardar os PIDs de todos os processos filho que forem criados
                    //escrever(search(comando.terminalPid)->data, "simulado");
                }
            }
        } else if (comando.operacao == OP_SAIR || comando.operacao == OP_SAIRAGORA) {
            int estado, sairAgora = 0;

            /* Sair Agora */
            if (comando.operacao == OP_SAIRAGORA) {
                sairAgora = 1;
                /* Ciclo que vai enviar um sinal individualmente para cada Processo Filho */
                for (i = 0; i < numPids; i++) {
                    if (kill(pids[i].pid, SIGUSR1) != 0) /* Verifica se ocorreu um erro ao enviar um Sinal */
                        printf("%s: Erro ao enviar sinal para o Processo.\n", strcat(COMANDO_SAIR , COMANDO_AGORA));
                }
            }

            /* Vai terminar e sincronizar todas as tarefas do sistema. Destroi tambem os respetivos semaforos */
            killThreadsSemaforosMutexes();

            /* Ciclo que vai terminar todos os Processos Filho */
            for (i = 0; i < numPids; i++) {

                if (waitpid(pids[i].pid, &estado, 0) == -1) /* Terminar Processo filho. Se ocorrer um erro vai cair no if statment */
                    printf("%s: Erro ao terminar Processo.\n", (sairAgora == 1) ? strcat(COMANDO_SAIR , COMANDO_AGORA) : COMANDO_SAIR);
                if (WIFEXITED(estado) != 0) { /* Se o processo sair com um exit corretamente (de que maneira for) */
                    if (WEXITSTATUS(estado) == 2) /* Vamos verificar se o exit retornou o termino por signal */
                        printf("Simulacao terminada por signal\n");
                }
                pids[i].estado = WIFEXITED(estado) ? 1 : -1;
            }
            printf("i-banco vai terminar.\n--\n");
            for (i = 0; i < numPids; i++) {
                printf("FILHO TERMINADO (PID=%d; terminou %s)\n", pids[i].pid, (pids[i].estado > 0) ? "normalmente" : "abruptamente");
            }
            printf("--\n");
            close(client_to_server);
            unlink(myfifo);
            exit(EXIT_SUCCESS);
        } else if (comando.operacao == OP_SAIRTERMINAL) {
            printf("terminal saiu1\n");
            //snprintf(fifoDelete, sizeof(fifoDelete), "%s%d", "/tmp/server_to_client_fifo_" , comando.terminalPid);
            printf("terminal saiu2 %d\n", search(comando.terminalPid)->data);

            close(search(comando.terminalPid)->data);
            printf("terminal saiu3 %s %d\n", fifoDelete, search(comando.terminalPid)->data);


            delete(search(comando.terminalPid));
            //asd

            //close(comando.idConta); // termina client_to_server vindo do terminal (idConta e usado apenas para transportar)
            printf("terminal saiuu\n");
        } else {
            produtor(comando);
        }
        //printf("fim do while\n");
        //printf("%d\n", comando.idConta);
        // numargs = readLineArguments(args, MAXARGS + 1, buffer, BUFFER_SIZE);

    }
    printf("fim dos fins\n");
}