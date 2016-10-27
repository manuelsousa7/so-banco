/******************************************************************************************
* File Name:    parte2.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     Nenhum
* DESCRIPTION:  Prototipos das funcoes de leitura dos comandos
* DIAGNOSTICS:  tested
*****************************************************************************************/

#ifndef PARTE2_H
#define PARTE2_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <string.h>
#include "parte1.h"
#include "contas.h"

typedef struct{
    int operacao;
    int idConta;
    int valor;
} comando_t;

/* Operações - Comandos */
#define OP_SAIR 1
#define OP_SAIRAGORA 2
#define OP_LERSALDO 3
#define OP_CREDITAR 4
#define OP_DEBITAR 5

#define NUM_TRABALHADORAS 2
#define CMD_BUFFER_DIM (NUM_TRABALHADORAS * 2)


pthread_t tid[NUM_TRABALHADORAS+1];
pthread_mutex_t semExMut,threadsContas[NUM_CONTAS];
sem_t podeProd,podeCons;

int buff_write_idx, buff_read_idx;

comando_t cmd_buffer[CMD_BUFFER_DIM];


void executarComando(comando_t c);
void *lerComandos(void *args);
void inicializarThreadsSemaforos();
void produtor(int idConta, int valor, int OP);
void killThreadsSemaforos();


#endif
