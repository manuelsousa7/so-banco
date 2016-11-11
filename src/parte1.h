/******************************************************************************************
* File Name:    parte1.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <unistd.h> - fork(), pid_t type
*
* DESCRIPTION:  Defines (macros) e estruturas dos processos (Parte 1 do projeto)
* DIAGNOSTICS:  OK
*****************************************************************************************/

#ifndef PARTE1_H
#define PARTE1_H

#include <unistd.h>

/* Macros - Comandos */
#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_AGORA "agora"
#define COMANDO_TRANSFERIR "transferir" //Parte 3

/* Número Máximo de Processos */
#define MAXCHILDS 20

/* Estrutura que guarda os ID's dos processos */
typedef struct PID {
	pid_t pid;
	int estado;
} pids;

#endif
