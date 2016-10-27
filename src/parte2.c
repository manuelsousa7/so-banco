/******************************************************************************************
* File Name:    parte2.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     Nenhum
* DESCRIPTION:  Contem funcao que lê os comandos
* DIAGNOSTICS:  tested
*****************************************************************************************/


#include "parte2.h"

void executarComando(comando_t c){
    switch (c.operacao) {
        case OP_LERSALDO:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            //printf("OP_LERSALDO %d\n",pthread_self() == tid[0]);
            int saldo = lerSaldo (c.idConta);
            //sleep(10);
            if (lerSaldo(c.idConta) < 0)
                printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, c.idConta);
            else
                printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, c.idConta, saldo);
            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;

        case OP_CREDITAR:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            //printf("OP_CREDITAR %d\n",pthread_self() == tid[0]);
            if (creditar (c.idConta, c.valor) < 0)
                printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, c.idConta, c.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, c.idConta, c.valor);

            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;

        case OP_DEBITAR:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            //printf("OP_DEBITAR %d\n",pthread_self() == tid[0]);
            if (debitar (c.idConta, c.valor) < 0)
               printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);

            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;
        case OP_SAIR:
            pthread_exit(NULL);
            exit(EXIT_SUCCESS);
            break;
        default:
            
            break;
    }

}

void *lerComandos(void *args){
    while(1){
        sem_wait(&podeCons);
        pthread_mutex_lock(&semExMut);
        comando_t consumido = cmd_buffer[buff_read_idx];
        buff_read_idx = (buff_read_idx+1) % CMD_BUFFER_DIM;
        pthread_mutex_unlock(&semExMut);
        sem_post(&podeProd);
        executarComando(consumido);
    }
}

void inicializarThreadsSemaforos(){
	sem_init(&podeProd, 0, CMD_BUFFER_DIM);
    sem_init(&podeCons, 0, 0);
    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        int err = pthread_create(&(tid[i]), NULL, &lerComandos, NULL);
        if (err != 0)
            printf("Falha ao criar Thread :[%s]\n", strerror(err));
        //else
        //    printf("Thread criado com sucesso\n");
    }
}

void produtor(int idConta, int valor, int OP){
    sem_wait(&podeProd);
    pthread_mutex_lock(&semExMut);
    cmd_buffer[buff_write_idx].operacao = OP;
    cmd_buffer[buff_write_idx].valor = valor;
    cmd_buffer[buff_write_idx].idConta = idConta;
    buff_write_idx = (buff_write_idx+1) % CMD_BUFFER_DIM;
    pthread_mutex_unlock(&semExMut);
    sem_post(&podeCons);
}

void killThreadsSemaforos(){
    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        produtor(0,0,OP_SAIR);
    }

    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        int err = pthread_join(tid[i], NULL);
        if (err != 0)
            printf("Falha ao criar Thread :[%s]\n", strerror(err));
        //else
        //    printf("Thread morto com sucesso\n");
    }
    sem_destroy(&podeProd);
    sem_destroy(&podeCons);
}




