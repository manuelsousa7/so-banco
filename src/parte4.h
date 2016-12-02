/******************************************************************************************
* File Name:    parte4.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <stdio.h> - fork(), pid_t type
*				#include <pthread.h> - fork(), pid_t type
*				#include <unistd.h> - fork(), pid_t type
*				#include <sys/stat.h> - fork(), pid_t type
*				#include <sys/syscall.h> - syscall() e SYS_gettid para obter pid
*				#include <fcntl.h> - open()
*				#include "parte234.h" - fork(), pid_t type
*				#include "parte1.h"- fork(), pid_t
*               #include "hashtable.h" - Prototipos e Estruturas da hashtable usada
*
* DESCRIPTION:  Defines (macros) e Prototipos das funcoes APENAS da parte 4
* DIAGNOSTICS:  OK
*****************************************************************************************/

#ifndef PARTE4_H
#define PARTE4_H

#include <stdio.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include "parte234.h"
#include "parte1.h"
#include "hashtable.h"

/* nome do ficheiro que contera os logs dos comandos do banco */
#define LOG_FILE "log.txt"

/* tamanho do buffer - outputs */
#define BUFFER_SIZE 100

int fout; /* descritor do LOG_FILE */
int save_out; /* output */
int out; /* output */

void escreverLog(int comando);
char* comandos(int comando);
void iniciaRedirecionarOutput();
void pararRedirecionarOutput();
void escrever(int fileDescriptor, char eOutput[]);

#endif