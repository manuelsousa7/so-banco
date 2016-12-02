/******************************************************************************************
* File Name:    contas.h
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <stdio.h> - I/O regular
*               #include <unistd.h> - fork()
*               #include <stdlib.h> - exit()
*               #include <signal.h> - signal(), kill()
* DESCRIPTION:  Prototipos e constantes das operações das contas (i-banco)
* DIAGNOSTICS:  OK
*****************************************************************************************/

#ifndef CONTAS_H
#define CONTAS_H

#define NUM_CONTAS 10
#define TAXAJURO 0.1
#define CUSTOMANUTENCAO 1

#define ATRASO 1 /* Numero de segundos do SLEEP */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/* Protótipos das Funções */
void inicializarContas();
int contaExiste(int idConta);
int debitar(int idConta, int valor);
int creditar(int idConta, int valor);
int lerSaldo(int idConta);
void simular(int numAnos);
void handler();
int transferir(int idConta1, int idConta2, int valor);

#endif
