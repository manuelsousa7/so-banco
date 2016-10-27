/******************************************************************************************
* File Name:    parte2.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     Nenhum
* DESCRIPTION:  Contem todas as funções relativas à parte2 do projeto sobre tarefas, 
*               sistema Produtor - Consumidor e buffer circular de comandos
* DIAGNOSTICS:  tested
*****************************************************************************************/

#include "parte2.h"

/******************************************************************************************
* executarComando()
*
* Arguments:  c:  comando do buffer a executar
*
* Returns: void
* Description:  Executa um comando que recebe como argumento
*****************************************************************************************/
void executarComando(comando_t c){
    switch (c.operacao) {
        case OP_LERSALDO:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            int saldo = lerSaldo (c.idConta);
            if (lerSaldo(c.idConta) < 0)
                printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, c.idConta);
            else
                printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, c.idConta, saldo);
            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;

        case OP_CREDITAR:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            if (creditar (c.idConta, c.valor) < 0)
                printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, c.idConta, c.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, c.idConta, c.valor);

            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;

        case OP_DEBITAR:
            pthread_mutex_lock(&threadsContas[c.idConta]);
            if (debitar (c.idConta, c.valor) < 0)
               printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);
            else
                printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);

            pthread_mutex_unlock(&threadsContas[c.idConta]);
            break;
        case OP_SAIR:
            pthread_exit(NULL); //Termina tarefa
            exit(EXIT_SUCCESS);
            break;
        default:
            
            break;
    }

}

/******************************************************************************************
* lerComandos()
*
* Arguments:  Nenhum
*
* Returns: void*
* Description:  << [Função executada pelas tarefas TRABALHADORAS] >>
*               Vai buscar comando a executar ao buffer circular de dados
*               {Consumidor} do sistema Produtor - {Consumidor}
*****************************************************************************************/
void *lerComandos(void *args){
    while(1){
        sem_wait(&podeCons); // Esperar
        pthread_mutex_lock(&semExMut); //Fechar
        comando_t consumido = cmd_buffer[buff_read_idx];
        buff_read_idx = (buff_read_idx + 1) % CMD_BUFFER_DIM;
        pthread_mutex_unlock(&semExMut); // Abrir
        sem_post(&podeProd); // Assinalar
        
        /* Após adquirir o comando a executar do buffer circular de dados, vamos executa-lo */
        executarComando(consumido); 
    }
}

/******************************************************************************************
* inicializarThreadsSemaforos()
*
* Arguments:  Nenhum
*
* Returns: void
* Description:  Inicializa os 2 semaforos do sistema Produtor - Consumidor.
*               Cria as tarefas trabalhadoras (definidas em macro - NUM_TRABALHADORAS)
*****************************************************************************************/
void inicializarThreadsSemaforos(){
	/* Incia Semáforos */
	sem_init(&podeProd, 0, CMD_BUFFER_DIM);
    sem_init(&podeCons, 0, 0);

    /* Inicia Tarefas */
    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        int err = pthread_create(&(tid[i]), NULL, &lerComandos, NULL); // Cria tarefa e guarda o Thread ID num vetor, e atribui a função lerComandos à tarefa
        if (err != 0)
            printf("Falha ao criar Thread :[%s]\n", strerror(err));
    }
}

/******************************************************************************************
* produtor()
*
* Arguments:	idConta: id da conta sobre a qual queremos efetuar a operacao
*               valor: valor correspondente à conta com idConta
*               OP: operacao a efetuar (codigo das OP's definidas em macro)
*
* Returns: void
* Description:  Acrescenta novo comando a executar no buffer circular de dados
*               {Produtor} do sistema {Produtor} - Consumidor
*****************************************************************************************/
void produtor(int idConta, int valor, int OP){
    sem_wait(&podeProd); // Esperar
    pthread_mutex_lock(&semExMut); // Fechar
    cmd_buffer[buff_write_idx].operacao = OP;
    cmd_buffer[buff_write_idx].valor = valor;
    cmd_buffer[buff_write_idx].idConta = idConta;
    buff_write_idx = (buff_write_idx + 1) % CMD_BUFFER_DIM; // Incrementa / Reinicia cursor que guarda indice de escrita
    pthread_mutex_unlock(&semExMut); // Abrir
    sem_post(&podeCons); // Assinalar
}

/******************************************************************************************
* killThreadsSemaforos()
*
* Arguments:	Nenhum
*
* Returns: void
* Description:  Força todas as tarefas a sair
*               Após a saida de todas as tarefas, sincroniza-as
*               Destroi os 2 semaforos do sistema Produtor - Consumidor.
*****************************************************************************************/
void killThreadsSemaforos(){
	/* Percorre as tarefas todas e força para dar exit */
    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        produtor(0, 0, OP_SAIR);
    }

    for(int i = 0; i < NUM_TRABALHADORAS ; i++){
        int err = pthread_join(tid[i], NULL); //Sincroniza a i tarefa
        if (err != 0)
            printf("Falha ao criar Thread :[%s]\n", strerror(err));
    }

    /* Destroi Semáforos */
    sem_destroy(&podeProd); 
    sem_destroy(&podeCons);
}




