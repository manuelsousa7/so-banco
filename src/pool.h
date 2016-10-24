/******************************************************************************************
* File Name:    pool.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     Nenhum
* DESCRIPTION:  Pool Comment!U"&#%&/"!%#&!"!"&#%&/!%"#/&%!"#/&!" (i-banco)
* DIAGNOSTICS:  tested
*****************************************************************************************/

#ifndef POOL_H
#define POOL_H

#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 100
#define NUM_TRABALHADORAS 3
#define CMD_BUFFER_DIM (NUM_TRABALHADORAS * 2)


pthread_t tid[NUM_TRABALHADORAS+1];
pthread_mutex_t semExtMut;
sem_t posicoesComInfo,posicoesSemInfo;

void* doSomeThing(void *arg);
void inicializarThreads();

#endif
