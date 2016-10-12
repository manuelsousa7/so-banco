/******************************************************************************************
* File Name:    contas.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include <stdio.h>
                #include <unistd.h> - sleep()
                #include <stdlib.h> - exit()
                #include "contas.h" - Prototipos dos comandos do banco
* DESCRIPTION:  Funções que suportam todas as operações relacionadas com as contas
* DIAGNOSTICS:  tested
*****************************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "contas.h"

#define atrasar() sleep(ATRASO)
		     
int contasSaldos[NUM_CONTAS];

int flag = -1; //Flag que vai ser usada para tratamento de signals

int contaExiste(int idConta) {
  return (idConta > 0 && idConta <= NUM_CONTAS);
}

void inicializarContas() {
  int i;
  for (i=0; i<NUM_CONTAS; i++)
    contasSaldos[i] = 0;
}

int debitar(int idConta, int valor) {
  atrasar();
  if (!contaExiste(idConta))
    return -1;
  if (contasSaldos[idConta - 1] < valor)
    return -1;
  atrasar();
  contasSaldos[idConta - 1] -= valor;
  return 0;
}

int creditar(int idConta, int valor) {
  atrasar();
  if (!contaExiste(idConta))
    return -1;
  contasSaldos[idConta - 1] += valor;
  return 0;
}

int lerSaldo(int idConta) {
  atrasar();
  if (!contaExiste(idConta))
    return -1;
  return contasSaldos[idConta - 1];
}


/******************************************************************************************
* handler()
*
* Arguments:  Nenhum
*
* Returns: void
* Description:  Altera a variavel global (flag) para o valor 1. Indica que recebeu um Sinal!
*****************************************************************************************/
void handler(){
  flag = 1;
}

/******************************************************************************************
* simular()
*
* Arguments:  Nenhum
*
* Returns: void
* Description:  Verifica se a variavel global (flag) foi alterada pela funcão [handler]
                Se foi alterada, então houve um sinal
*****************************************************************************************/
void isDead(){
  if(flag == 1) {
    exit(2); //Houve Signal
  }
}

/******************************************************************************************
* simular()
*
* Arguments:  numAnos:  numero de anos para correr a simulacao
*
* Returns: void
* Description:  Faz uma simulacao (juros) de um numero de anos de todas as contas do banco
*****************************************************************************************/
void simular(int numAnos) {
  /* Se houver um problema a tratar do Signal vai ocorrer um erro*/
  if (signal(SIGUSR1, handler) == SIG_ERR){
    printf("simular: Não foi possivel tratar do sinal\n");
    exit(EXIT_FAILURE);
  }

  
  int contasSaldosSimular[NUM_CONTAS];
  //Copia dos saldos das contas
  for(int i = 0;i < NUM_CONTAS;i++){
    contasSaldosSimular[i] = lerSaldo(i+1);
  }

  isDead(); //Verifica se ocorreu um sinal

  for(int i = 0; i <= numAnos; i++){
    printf("SIMULACAO: Ano %d\n=================\n", i);
    for(int ii = 0; ii < NUM_CONTAS; ii++){
      if(i != 0 && contasSaldosSimular[ii] != 0){
        contasSaldosSimular[ii] = (contasSaldosSimular[ii] * (1 + TAXAJURO) - CUSTOMANUTENCAO);
      }
      printf("Conta %d, Saldo %d\n", ii+1, contasSaldosSimular[ii] > 0 ? contasSaldosSimular[ii] : -contasSaldosSimular[ii]);
    }
    isDead(); //Verifica se ocorreu um sinal
    printf("\n");
  }
  exit(1);    
}
