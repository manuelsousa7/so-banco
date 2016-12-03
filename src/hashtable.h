/******************************************************************************************
* File Name:    hashtable.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <stdio.h> - I/O regular
*				#include <stdlib.h> - mallocs e frees
* DESCRIPTION:  Defines (macros) e Prototipos das funcoes da hashtable usada no i-banco
* DIAGNOSTICS:  OK
*****************************************************************************************/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>

/* Dimensao da Hashtable - Equivalente a 50 terminais*/
#define SIZE 50

/* Estrutura de dados da hashtable */
struct DataItem {
	int data; /* Guarda o descritor do fiheiro */
	int key; /* key - sera a chave da hashtable, que sera o pid de cada i-banco-terminal */
};

struct DataItem* hashArray[SIZE]; /* Hashtabl aka Tabela de Dispersao */
struct DataItem* dummyItem; /* item que e atribuido quando se remove algo da hastable */
struct DataItem* item; /* variavel que vai receber conteudo retornado pelo search() */

void display();
struct DataItem *search(int key);
void insert(int key, int data);
struct DataItem* delete(struct DataItem* item);
void freeHash() ;
#endif
