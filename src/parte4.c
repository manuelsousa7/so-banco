/******************************************************************************************
* File Name:    parte4.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include "parte4.h" - Prototipos e Estruturas usadas na entrega 4
* DESCRIPTION:  fdsjnkjnsfdjndfskjnsdfjknsdfnjkfsdkjndsfknjsdfjnksdfkjn
* DIAGNOSTICS:  OK
*****************************************************************************************/

#include "parte4.h"


void escreverLog(int comando) {
	pid_t tid;
	tid = syscall(SYS_gettid);
	if (tid == -1) {
		printf("ERRO: syscall - SYS_gettid\n");
	}
	int fout = open(LOG_FILE, O_WRONLY | O_APPEND);
	char out[100];
	snprintf(out, sizeof(out), "%d: %s\n", tid, comandos(comando));
	write(fout, out, strlen(out));
	close(fout);
}

char* comandos(int comando) {
	switch (comando) {
	case OP_LERSALDO:
		return COMANDO_LER_SALDO;
	case OP_CREDITAR:
		return COMANDO_CREDITAR;
	case OP_DEBITAR:
		return COMANDO_DEBITAR;
	case OP_TRANSFERIR:
		return COMANDO_TRANSFERIR;
	case OP_SACOAZUL:
		return COMANDO_SACOAZUL;
		break;
	case OP_SAIR:
		return COMANDO_SAIR;
	default:
		return "ERRO";
	}
}