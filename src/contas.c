#include "contas.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define atrasar() sleep(ATRASO)
		     
int contasSaldos[NUM_CONTAS];


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


void simular(int numAnos) {
  int saldo;
  int contasSaldosSimular[NUM_CONTAS];
  for(int i = 0;i < NUM_CONTAS;i++){
    contasSaldosSimular[i]=lerSaldo(i+1);
  }
  for(int i=0;i<numAnos;i++){
    printf("SIMULACAO: Ano %d\n=================\n",i);
    for(int ii=0;ii<NUM_CONTAS;ii++){
      saldo = (contasSaldosSimular[i] * (1 + TAXAJURO) - CUSTOMANUTENCAO);
      printf("Conta %d, Saldo %d\n",ii+1,saldo > 0 ? saldo : -saldo);
    }
    printf("\n");
  }
      
}
