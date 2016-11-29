/******************************************************************************************
* File Name:    parte4.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     kjfsdjksdfjnksdfnjksfdjnkdfsnjksdfkndjsfe todas as operações relacionadas com contas
*               #include "parte1.h"gdfgdfgfdgdf
gdf
g
fd
gfd
g
df
g
df
 - Prototipos das funcoesdfsdfdsfsdfsfsdf da parte 1 - Defines (macros) dos comandos
* DESCRIPTION:  Defines (macros) e Prototipos das funcoes da parte 2 e 3 (tarefas)
* DIAGNOSTICS:  OK
*****************************************************************************************/

#ifndef PARTE4_H
#define PARTE4_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include "parte1.h"
#include "parte234.h"
#define LOG_FILE "log.txt"

int fout;


void escreverLog(int comando);
char* comandos(int comando);

#endif