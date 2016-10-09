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

void nope(){
  flag = 1;
}

void simular(int numAnos) {
  signal(SIGUSR1,nope);
  int contasSaldosSimular[NUM_CONTAS];
  for(int i = 0;i < NUM_CONTAS;i++){
    contasSaldosSimular[i]=lerSaldo(i+1);
  }
  if(flag == 1) exit(EXIT_FAILURE);
  for(int i=0;i<numAnos;i++){
    if(flag == 1) exit(EXIT_FAILURE);
    printf("SIMULACAO: Ano %d\n=================\n",i);
    for(int ii=0;ii<NUM_CONTAS;ii++){
      if(i != 0){
        if(contasSaldosSimular[ii] != 0){
          contasSaldosSimular[ii] = (contasSaldosSimular[ii] * (1 + TAXAJURO) - CUSTOMANUTENCAO);
        }
      }
    }
    printf("\n");
  }
      
}
