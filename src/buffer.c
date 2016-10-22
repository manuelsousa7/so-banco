/******************************************************************************************
* File Name:    buffer.c
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
#include "buffer.h"


void changeBuffer(comando_t cmd_buffer[], int idConta, int valor, int OP, int *index){
    cmd_buffer[*index].operacao = OP;
    cmd_buffer[*index].valor = valor < 0 ? cmd_buffer[*index].valor : valor;
    cmd_buffer[*index].idConta = idConta;
    if(((*index) + 1) == BUFFER_SIZE)
    	*index = 0;
    else
    	(*index)++; 
}

comando_t getRequest(comando_t cmd_buffer[], int *index){
	comando_t temp = cmd_buffer[*index];
	(*index)++;
	return temp; 
}
