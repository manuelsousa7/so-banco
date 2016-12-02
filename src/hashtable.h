/******************************************************************************************
* File Name:    hashtable.h
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

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define SIZE 20

struct DataItem {
	int data;
	int key;
};

struct DataItem* hashArray[SIZE];
struct DataItem* dummyItem;
struct DataItem* item;


void display();
struct DataItem *search(int key);
void insert(int key, int data);
struct DataItem* delete(struct DataItem* item);

#endif
