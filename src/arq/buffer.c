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

#include "buffer.h"


void writeBuffer(int idConta, int valor, int OP){
    cmd_buffer[buff_write_idx].operacao = OP;
    cmd_buffer[buff_write_idx].valor = valor;
    cmd_buffer[buff_write_idx].idConta = idConta;
    buff_write_idx = (buff_write_idx+1) % CMD_BUFFER_DIM;
}
