/******************************************************************************************
* File Name:    parte4.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include "parte4.h" - Prototipos e Estruturas usadas na entrega 4
* DESCRIPTION:  Funcoes relativas a parte 4 (somente)
* DIAGNOSTICS:  OK
*****************************************************************************************/

#include "parte4.h"

void escreverLog(int comando) {
	pid_t tid;
	char out[BUFFER_SIZE];
	tid = syscall(SYS_gettid); /* syscall para obter tid da tarefa */
	if (tid == -1) {
		printf("ERRO: syscall - SYS_gettid\n");
	}

	if (snprintf(out, sizeof(out), "%d: %s\n", tid, comandos(comando)) < 0) {
		printf("ERRO: snprintf\n");
	}

	if (write(fout, out, strlen(out)) < -1) {
		printf("ERRO: write - params: [fout,out,strlen(out]\n");
	}
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
	case OP_SIMULAR:
		return COMANDO_SIMULAR;
	case OP_SAIR:
		return COMANDO_SAIR;
	default:
		return "ERRO";
	}
}

void iniciaRedirecionarOutput() {
	char file[BUFFER_SIZE];
	if (snprintf(file, sizeof(file), "i-banco-sim-%d.txt",  getpid()) < 0) {
		printf("ERRO: snprintf\n");
	}
	/* abre ficheiro para redirecionar output das simulacoes */
	out = open(file, O_TRUNC | O_WRONLY | O_CREAT, 0666);

	if (out == -1) {
		printf("ERRO: open - params: [O_TRUNC | O_RDWR | O_CREAT, 0666]\n");
	}
	/* redireciona output */
	if (dup2(out, fileno(stdout)) == -1) {
		printf("ERRO: dup2 - params: [out, fileno(stdout)]\n");
	}
}

void pararRedirecionarOutput() {
	/* fechar tudo */

	if (fflush(stdout) != 0) {
		printf("ERRO: fflush - params: stdout\n");
	}

	if (close(out) != 0) {
		printf("ERRO: close - params: out\n");
	}

	if (close(save_out) != 0) {
		printf("ERRO: close - params: save_out\n");
	}
}

void escrever(int fileDescriptor, char eOutput[]) {
	/* escreve no i-banco-terminal correspondente ao filedescriptor */
	if (write(fileDescriptor, eOutput, BUFFER_SIZE) < -1) {
		printf("ERRO: write - params: [fileDescriptor,eOutput,BUFFER_SIZE]\n");
	}
}