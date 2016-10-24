/******************************************************************************************
* File Name:    pool.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <stdio.h>
                #include <unistd.h> - sleep()
                #include <stdlib.h> - exit()
                #include "contas.h" - Prototipos dos comandos do banco
* DESCRIPTION:  POOOL JABDHJSABHDJBDJ BASHJBDASBH DASHJB DJSAH DAHJSBJ 
* DIAGNOSTICS:  tested
*****************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "pool.h"

pthread_mutex_t threadsContas[NUM_CONTAS];

//consumidor
void produtor()
{
    while(1){
        comando_t item = produz();
        sem_wait(&podeProd); //esperar
        pthread_mutex_lock(&semExMut); //fechar
        buffer[prodptr] = item;
        prodptr = (prodptr+1) % N;
        pthread_mutex_unlock(&semExMut);//abrir
        sem_post(&podeCons);//assinalar
    }
    return NULL;
}

void consumidor()
{
    while(1){
        int item = produz();
        sem_wait(&podeCons);
        pthread_mutex_lock(&semExMut);
        buf[prodptr] = item;
        prodptr = (prodptr+1) % N;
        pthread_mutex_unlock(&semExMut);
        sem_post(&podeProd);
    }
    return NULL;
}


void iniciaLeitura(int idConta)
{
    pthread_mutex_lock(&threadsContas[idConta]);
    if (em_escrita || escritores_espera > 0) {
        leitores_espera++;
        pthread_mutex_unlock(&threadsContas[idConta]);
        sem_wait(&semaforos[idConta]);
        pthread_mutex_lock(&threadsContas[idConta]);
        if(leitores_espera > 0){
            nleitores++;
            leitores_espera--;
            sem_post(&semaforos[idConta]);
        } 
    } else {
        nleitores++;
    }
    pthread_mutex_unlock(&threadsContas[idConta]);
}


void acabaLeitura(int idConta)
{
    pthread_mutex_lock(&threadsContas[idConta]);
    nleitores--;
    if (nleitores == 0 && escritores_espera > 0){
        sem_post(&semaforos[idConta]);
        em_escrita=TRUE;
        escritores_espera--;
    }
    pthread_mutex_unlock(&threadsContas[idConta]);
}

void iniciaEscrita(int idConta){
    pthread_mutex_lock(&threadsContas[idConta]);
    if (em_escrita || nleitores > 0 || leitores_espera > 0) {
        escritores_espera++;
        pthread_mutex_unlock(&threadsContas[idConta]);
        sem_post(escritores);
        pthread_mutex_lock(&threadsContas[idConta]);
    }
    em_escrita = TRUE;
    pthread_mutex_unlock(&threadsContas[idConta]);
}

void acaba_escrita(int idConta){
    pthread_mutex_lock(&threadsContas[idConta]);
    em_escrita = FALSE;
    if (leitores_espera > 0){
        sem_post(&semaforos[idConta]);
        nleitores++;
        leitores_espera--;
    } else if (escritores_espera > 0) {
        sem_post(&semaforos[idConta]);
        em_escrita=TRUE;
        escritores_espera--;
    }
    pthread_mutex_unlock(&threadsContas[idConta]);
}


void *lerComandos(void *ptr){
    while(1){
        sem_wait(&podeCons);
        pthread_mutex_lock(&semExMut);
        comando_t consumido = cmd_buffer[buff_read_idx];
        buff_read_idx = (buff_read_idx+1) % N;
        pthread_mutex_unlock(&semExMut);
        sem_post(&podeProd);

        executarComando(consumido);
    }
}


void executarComando(comando_t c){
    switch (c.operacao) {
        case OP_LERSALDO:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            
            if (c.valor < 0)
                printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, c.idConta);
            else
                printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, c.idConta, c.saldo);
            
            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;

        case OP_CREDITAR:
            pthread_mutex_lock(&threadsContas[c.idConta]);

            if (creditar (c.idConta, c.valor) < 0)
                printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, c.idConta, c.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, c.idConta, c.valor);

            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;

        case OP_DEBITAR:
            pthread_mutex_lock(&threadsContas[c.idConta]);

            if (debitar (c.idConta, c.valor) < 0)
               printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);

            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;
        case OP_SAIR:
            //something
            exit(EXIT_SUCCESS);
            break;
        case OP_SAIRAGORA:
            //something
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("oiix");
            break;
}

}

void inicializarThreads(){
	for(int i = 0; i < NUM_TRABALHADORAS ; i++){
		int err = pthread_create(&(tid[i]), NULL, &lerComandos, NULL);
		if (err != 0)
            printf("Falha ao criar Thread :[%s]\n", strerror(err));
        else
            printf("Thread criado com sucesso\n");
	}
}

void killThreads(){
    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        int err = pthread_join(tid[i], NULL);
        if (err != 0)
            printf("Falha ao criar Thread :[%s]\n", strerror(err));
        else
            printf("Thread criado com sucesso\n");
    }
}
