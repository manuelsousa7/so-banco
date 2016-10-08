#include "contas.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define atrasar() sleep(ATRASO)
		     
int contasSaldos[NUM_CONTAS];

int flag=-1;

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

void nope(int flag){
  flag = 1;
  printf("SIGNAAAAAAAAALLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
  exit(EXIT_SUCCESS);
}

void simular(int numAnos) {
  int contasSaldosSimular[NUM_CONTAS];
  for(int i = 0;i < NUM_CONTAS;i++){
    contasSaldosSimular[i]=lerSaldo(i+1);
  }
  signal(SIGUSR1,nope);
  if(flag == 0) return;
  for(int i=0;i<numAnos;i++){
    signal(SIGUSR1,nope);
    if(flag == 0) return;
    printf("SIMULACAO: Ano %d\n=================\n",i);
    for(int ii=0;ii<NUM_CONTAS;ii++){
      contasSaldosSimular[i] = (contasSaldosSimular[i] * (1 + TAXAJURO) - CUSTOMANUTENCAO);
      printf("Conta %d, Saldo %d\n",ii+1,contasSaldosSimular[i] > 0 ? contasSaldosSimular[i] : -contasSaldosSimular[i]);
    }
    printf("\n");
  }
      
}
