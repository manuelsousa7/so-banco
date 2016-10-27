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
#include <semaphore.h>
#include <pthread.h>
#include <sys/wait.h>
#include "contas.h"
#include "commandlinereader.h"

/* Macros - Comandos */
#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_AGORA "agora"

/* Operações - Comandos */
#define OP_SAIR 1
#define OP_SAIRAGORA 2
#define OP_LERSALDO 3
#define OP_CREDITAR 4
#define OP_DEBITAR 5

/*Constantes*/
#define MAXARGS 3
#define BUFFER_SIZE 100
#define MAXCHILDS 20
#define NUM_TRABALHADORAS 2
#define CMD_BUFFER_DIM (NUM_TRABALHADORAS * 2)


typedef struct PID{
    pid_t pid;
    int estado;
}pids;


typedef struct{
    int operacao;
    int idConta;
    int valor;
} comando_t;

pthread_t tid[NUM_TRABALHADORAS+1];
pthread_mutex_t semExMut,threadsContas[NUM_CONTAS];
sem_t podeProd,podeCons;

int buff_write_idx = 0, buff_read_idx = 0;

comando_t cmd_buffer[CMD_BUFFER_DIM];

void executarComando(comando_t c){
    switch (c.operacao) {
        case OP_LERSALDO:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            //printf("OP_LERSALDO %d\n",pthread_self() == tid[0]);
            int saldo = lerSaldo (c.idConta);
            //sleep(10);
            if (lerSaldo(c.idConta) < 0)
                printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, c.idConta);
            else
                printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, c.idConta, saldo);
            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;

        case OP_CREDITAR:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            //printf("OP_CREDITAR %d\n",pthread_self() == tid[0]);
            if (creditar (c.idConta, c.valor) < 0)
                printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, c.idConta, c.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, c.idConta, c.valor);

            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;

        case OP_DEBITAR:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            //printf("OP_DEBITAR %d\n",pthread_self() == tid[0]);
            if (debitar (c.idConta, c.valor) < 0)
               printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);

            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;
        case OP_SAIR:
            pthread_exit(NULL);
            exit(EXIT_SUCCESS);
            break;
        default:
            
            break;
    }

}

void *lerComandos(void *args){
    while(1){
        sem_wait(&podeCons);
        pthread_mutex_lock(&semExMut);
        comando_t consumido = cmd_buffer[buff_read_idx];
        buff_read_idx = (buff_read_idx+1) % CMD_BUFFER_DIM;
        pthread_mutex_unlock(&semExMut);
        sem_post(&podeProd);
        executarComando(consumido);
    }
}

void inicializarThreads(){
    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        int err = pthread_create(&(tid[i]), NULL, &lerComandos, NULL);
        if (err != 0)
            printf("Falha ao criar Thread :[%s]\n", strerror(err));
        //else
        //    printf("Thread criado com sucesso\n");
    }
}

void produtor(int idConta, int valor, int OP){
    sem_wait(&podeProd);
    pthread_mutex_lock(&semExMut);
    cmd_buffer[buff_write_idx].operacao = OP;
    cmd_buffer[buff_write_idx].valor = valor;
    cmd_buffer[buff_write_idx].idConta = idConta;
    buff_write_idx = (buff_write_idx+1) % CMD_BUFFER_DIM;
    pthread_mutex_unlock(&semExMut);
    sem_post(&podeCons);
}
void killThreads(){
    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        produtor(0,0,OP_SAIR);
    }

    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        int err = pthread_join(tid[i], NULL);
        if (err != 0)
            printf("Falha ao criar Thread :[%s]\n", strerror(err));
        //else
        //    printf("Thread morto com sucesso\n");
    }
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
    int numPids = 0;
    sem_init(&podeProd, 0, CMD_BUFFER_DIM);
    sem_init(&podeCons, 0, 0);
    inicializarContas();
    inicializarThreads();

    printf("Bem-vinda/o ao i-banco\n\n");
    
    while (1) {
        int numargs;
        pids pids[MAXCHILDS];
        numargs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);
        int estado, sairAgora = 0;
        /* EOF (end of file) do stdin ou comando "sair" , "sair agora"*/
        if (numargs < 0 || (numargs > 0  && (strcmp(args[0], COMANDO_SAIR) == 0))) {
            if (numargs < 2) {

            /* Sair Agora */    
            } else if (numargs == 2 && strcmp(args[1], COMANDO_AGORA) == 0) {
                sairAgora = 1;
                //Ciclo que vai enviar um sinal individualmente para cada Processo Filho
                for(int i = 0; i < numPids; i++){
                    if (kill(pids[i].pid, SIGUSR1) != 0) //Verifica se ocorreu um erro ao enviar um Sinal
                        printf("%s: Erro ao enviar sinal para o Processo.\n", strcat(COMANDO_SAIR , COMANDO_AGORA));
                }    
            } else {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SAIR);
                continue;
            }

            /* Vai terminar e sincronizar todas as tarefas do sistema */    
            killThreads();

            /* Ciclo que vai terminar todos os Processos Filho */    
            for(int i=0;i<numPids;i++){
                
                if(waitpid(pids[i].pid,&estado,0) == -1) //Terminar Processo filho. Se ocorrer um erro vai cair no if statment
                    printf("%s: Erro ao terminar Processo.\n", (sairAgora == 1) ? strcat(COMANDO_SAIR , COMANDO_AGORA) : COMANDO_SAIR);
                if(WIFEXITED(estado) != 0){ //Se o processo sair com um exit corretamente (de que maneira for)
                    if(WEXITSTATUS(estado) == 2) //Vamos verificar se o exit retornou o termino por signal
                        printf("Simulacao terminada por signal\n");
                } 
                pids[i].estado = WIFEXITED(estado) ? 1 : -1;
            } 
            printf("i-banco vai terminar.\n--\n");
            for(int i = 0; i < numPids; i++){
                printf("FILHO TERMINADO (PID=%d; terminou %s)\n",pids[i].pid, (pids[i].estado > 0) ? "normalmente" : "abruptamente");
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
            //int idConta, valor;
            if (numargs < 3) {
                printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_DEBITAR);
               continue;
            }

            produtor(atoi(args[1]),atoi(args[2]),OP_DEBITAR);
    }

    /* Creditar */
    else if (strcmp(args[0], COMANDO_CREDITAR) == 0) {
        //int idConta, valor;
        if (numargs < 3) {
            printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_CREDITAR);
            continue;
        }

        produtor(atoi(args[1]),atoi(args[2]),OP_CREDITAR);
    }

    /* Ler Saldo */
    else if (strcmp(args[0], COMANDO_LER_SALDO) == 0) {
        //int idConta, saldo;

        if (numargs < 2) {
            printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_LER_SALDO);
            continue;
        }
        produtor(atoi(args[1]),0,OP_LERSALDO);
    }

    /* Simular */
    else if (strcmp(args[0], COMANDO_SIMULAR) == 0 && numargs == 2) {
        int anos;
        pid_t pid;
        if ((anos = atoi(args[1])) <= 0){
            printf("%s: Sintaxe inválida, tente de novo.\n", COMANDO_SIMULAR);
        } else {
            pid = fork();
            if(pid < 0){ // Erro ao fazer fork do processo PAI
                printf("%s: ERRO ao criar processo.ID do fork %d\n",COMANDO_SIMULAR,pid);
                exit(EXIT_FAILURE);
            } else if (pid == 0) { //Criou Processo filho com sucesso
                simular(anos);
                exit(EXIT_SUCCESS);
            } else if (pid > 0){ // Processo PAI
                pids[numPids++].pid = pid; //Vamos guardar os PIDs de todos os processos filho que forem criados
            }
        }
        continue;
    } else {
      printf("Comando desconhecido. Tente de novo.\n");
    }

  } 
}
