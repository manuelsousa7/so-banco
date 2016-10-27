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

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM_CONTAS 10
#define NUM_TRABALHADORAS 3
#define CMD_BUFFER_DIM (NUM_TRABALHADORAS * 2)


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


void *lerComandos(void *ptr);
void executarComando(comando_t c);
void inicializarThreads();
void killThreads();

#endif
