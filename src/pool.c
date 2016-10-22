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

//consumidor
void* consumidor(void *arg)
{
    while(1){
        sem_wait(&posicoesComInfo);
        pthread_mutex_lock(&semExMut);
        if(haNovos != 0){
            getRequest()
        }
        pthread_mutex_unlock(&semExMut);
        sem_post(&posicoesSemInfo);
    }
    return NULL;
}

void* produtor(void *arg)
{
    while(1){
        sem_wait(&posicoesSemInfo);
        pthread_mutex_lock(&semExMut);
        if(haNovos != 0){

        }
        pthread_mutex_unlock(&semExMut);
        sem_post(&posicoesComInfo);
    }
    return NULL;
}

void inicializarThreads(){
	for(int i = 0; i < NUM_TRABALHADORAS ; i++){
		int err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
		if (pthread_create(&(tid[i]), NULL, &doSomeThing, NULL) != 0)
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
