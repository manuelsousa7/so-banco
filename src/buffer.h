/******************************************************************************************
* File Name:    buffer.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     Nenhum
* DESCRIPTION:  Pool Comment!U"&#%&/"!%#&!"!"&#%&/!%"#/&%!"#/&!" (i-banco)
* DIAGNOSTICS:  tested
*****************************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

typedef struct{
	int operacao;
	int idConta;
	int valor;
} comando_t;

int buff_write_idx = 0, buff_read_idx = 0;

comando_t cmd_buffer[CMD_BUFFER_DIM];

void changeBuffer(int idConta, int valor, int OP);

#endif
