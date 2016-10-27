/******************************************************************************************
* File Name:    parte1.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     Nenhum
* DESCRIPTION:  Prototipos das funcoes de leitura dos comandos
* DIAGNOSTICS:  tested
*****************************************************************************************/

#ifndef PARTE1_H
#define PARTE1_H

#include <unistd.h>

#define MAXCHILDS 20


typedef struct PID{
    pid_t pid;
    int estado;
}pids;

/* Macros - Comandos */
#define COMANDO_DEBITAR "debitar"
#define COMANDO_CREDITAR "creditar"
#define COMANDO_LER_SALDO "lerSaldo"
#define COMANDO_SIMULAR "simular"
#define COMANDO_SAIR "sair"
#define COMANDO_AGORA "agora"


#endif
