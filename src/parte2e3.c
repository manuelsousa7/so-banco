/******************************************************************************************
* File Name:    parte2.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include "parte2.h" - Prototipos e Estruturas usadas na entrega 2 (tarefas)
* DESCRIPTION:  Contem todas as funções relativas à parte2 do projeto sobre tarefas,
*               sistema Produtor - Consumidor e buffer circular de comandos
* DIAGNOSTICS:  OK
*****************************************************************************************/

#include "parte2e3.h"

/******************************************************************************************
* executarComando()
*
* Arguments:  c:  comando do buffer a executar
*
* Returns: void
* Description:  Executa um comando que recebe como argumento (Não executa 2 comandos da mesma conta ao mesmo tempo)
*****************************************************************************************/
void executarComando(comando_t c) {
	switch (c.operacao) {
	case OP_LERSALDO:
		if (!contaExiste(c.idConta)) {
			printf("A conta %d nao existe\n", c.idConta);
			break;
		}
		/* Fechar */
		if (pthread_mutex_lock(&threadsContas[c.idConta - 1]) != 0) {
			printf("ERRO: thread_mutex_lock - &threadsContas[c.idConta-1]\n");
		}

		int saldo = lerSaldo (c.idConta);
		if (lerSaldo(c.idConta) < 0)
			printf("%s(%d): Erro.\n\n", COMANDO_LER_SALDO, c.idConta);
		else
			printf("%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, c.idConta, saldo);

		/* Abrir */
		if (pthread_mutex_unlock(&threadsContas[c.idConta - 1]) != 0) {
			printf("ERRO: thread_mutex_unlock - &threadsContas[c.idConta-1]\n");
		}
		break;

	case OP_CREDITAR:
		if (!contaExiste(c.idConta)) {
			printf("A conta %d nao existe\n", c.idConta);
			break;
		}
		/* Fechar */
		if (pthread_mutex_lock(&threadsContas[c.idConta - 1]) != 0) {
			printf("ERRO: thread_mutex_lock - &threadsContas[c.idConta-1]\n");
		}

		if (creditar (c.idConta, c.valor) < 0)
			printf("%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, c.idConta, c.valor);
		else
			printf("%s(%d, %d): OK\n\n", COMANDO_CREDITAR, c.idConta, c.valor);

		/* Abrir */
		if (pthread_mutex_unlock(&threadsContas[c.idConta - 1]) != 0) {
			printf("ERRO: thread_mutex_unlock - &threadsContas[c.idConta-1]\n");
		}
		break;

	case OP_DEBITAR:
		if (!contaExiste(c.idConta)) {
			printf("A conta %d nao existe\n", c.idConta);
			break;
		}
		/* Fechar */
		if (pthread_mutex_lock(&threadsContas[c.idConta - 1]) != 0) {
			printf("ERRO: thread_mutex_lock - &threadsContas[c.idConta-1]\n");
		}

		if (debitar (c.idConta, c.valor) < 0)
			printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);
		else
			printf("%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);

		/* Abrir */
		if (pthread_mutex_unlock(&threadsContas[c.idConta - 1]) != 0) {
			printf("ERRO: thread_mutex_unlock - &threadsContas[c.idConta-1]\n");
		}

		break;
	case OP_TRANSFERIR:
		if ((!contaExiste(c.idConta) || !contaExiste(c.idConta2)) || c.idConta == c.idConta2) {
			printf("Erro ao transferir valor da conta %d para a conta %d.\n\n", c.idConta, c.idConta2);
			break;
		}
		/* Fechar Contas relativas a operacao */
		if (pthread_mutex_lock(&threadsContas[MIN(c.idConta - 1, c.idConta2 - 1)]) != 0) {
			printf("ERRO: thread_mutex_lock - &threadsContas[MIN(c.idConta-1, c.idConta2-1)]\n");
		}
		if (pthread_mutex_lock(&threadsContas[MAX(c.idConta - 1, c.idConta2 - 1)]) != 0) {
			printf("ERRO: thread_mutex_lock - &threadsContas[MAX(c.idConta-1, c.idConta2-1)]\n");
		}

		if (transferir(c.idConta, c.idConta2, c.valor) < 0)
			printf("Erro ao transferir valor da conta %d para a conta %d.\n\n", c.idConta, c.idConta2);
		else
			printf("%s(%d, %d, %d): OK\n\n", COMANDO_TRANSFERIR, c.idConta, c.idConta2, c.valor);

		/* Abrir Contas relativas a operacao */
		if (pthread_mutex_unlock(&threadsContas[MAX(c.idConta - 1, c.idConta2 - 1)]) != 0) {
			printf("ERRO: thread_mutex_unlock - &threadsContas[MAX(c.idConta-1,c.idConta2-1)]\n");
		}
		if (pthread_mutex_unlock(&threadsContas[MIN(c.idConta - 1, c.idConta2 - 1)]) != 0) {
			printf("ERRO: thread_mutex_unlock - &threadsContas[MIN(c.idConta-1,c.idConta2-1)]\n");
		}
		break;
	case OP_SAIR:
		pthread_exit(NULL); //Termina tarefa - ESTA FUNCAO TEM SEMPRE SUCESSO
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
void *lerComandos(void *args) {
	while (1) {
		/* Esperar */
		if (sem_wait(&podeCons) != 0) {
			printf("ERRO: sem_wait - &podeCons\n");
		}
		/* Fechar */
		if (pthread_mutex_lock(&semExMut) != 0) {
			printf("ERRO: pthread_mutex_lock - &semExMut\n");
		}
		comando_t consumido = cmd_buffer[buff_read_idx];
		buff_read_idx = (buff_read_idx + 1) % CMD_BUFFER_DIM; // Incrementa / Reinicia cursor que guarda indice de leitura


		/* Abrir */
		if (pthread_mutex_unlock(&semExMut) != 0) {
			printf("ERRO: pthread_mutex_unlock - &semExMut\n");
		}

		/* Assinalar */
		if (sem_post(&podeProd) != 0) {
			printf("ERRO: sem_post - &podeProd\n");
		}
		/* Após adquirir o comando a executar do buffer circular de dados, vamos executa-lo */

		executarComando(consumido);
		StuffInside--; //Decrementa items no contador

		/* Fechar */
		if (pthread_mutex_lock(&semExMut) != 0) {
			printf("ERRO: pthread_mutex_lock - &semExMut\n");
		}
		if (StuffInside == 0) {
			pthread_cond_signal(&cheio);
		}
		/* Fechar */
		if (pthread_mutex_unlock(&semExMut) != 0) {
			printf("ERRO: pthread_mutex_lock - &semExMut\n");
		}
	}
}

/******************************************************************************************
* inicializarThreadsSemaforos()
*
* Arguments:  Nenhum
*
* Returns: void
* Description:  Inicializa todos os mutexes (de cada conta e do sistema Produtor - Consumidor)
*               Inicializa os 2 semaforos do sistema Produtor - Consumidor.
*               Cria as tarefas trabalhadoras (definidas em macro - NUM_TRABALHADORAS)
*****************************************************************************************/
void inicializarThreadsSemaforosMutexes() {
	/* Incia Mutex geral do Produtor - Consumidor */
	if (pthread_mutex_init(&semExMut, NULL) != 0) {
		printf("ERRO: pthread_mutex_init - params: &semExMut\n");
	}
	/* Incia Mutexes das contas */
	for (int i = 0; i < NUM_CONTAS; i++) {
		if (pthread_mutex_init(&threadsContas[i], NULL) != 0) {
			printf("ERRO: pthread_mutex_init - params: &threadsContas[i]\n");
		}
	}
	/* Incia Semáforos */
	if (sem_init(&podeProd, 0, CMD_BUFFER_DIM) != 0) {
		printf("ERRO: sem_init - params: [&podeProd, 0, CMD_BUFFER_DIM]\n");
	}
	if (sem_init(&podeCons, 0, 0) != 0) {
		printf("ERRO: sem_init - params: [&podeCons, 0, 0]\n");
	}

	/* Inicia Variavel de Condicão */
	pthread_cond_init(&cheio, NULL);

	/* Inicia Tarefas */
	for (int i = 0; i < NUM_TRABALHADORAS ; i++) {
		int err = pthread_create(&(tid[i]), NULL, &lerComandos, NULL); // Cria tarefa e guarda o Thread ID num vetor, e atribui a função lerComandos à tarefa
		if (err != 0)
			printf("Falha ao criar Thread :[%s]\n", strerror(err));
	}
}

/******************************************************************************************
* produtor()
*
* Arguments:	idConta: id da conta sobre a qual queremos efetuar a operacao
*               idConta2: id da conta sobre a qual queremos efetuar a operacao (usado apenas no comando transferir)
				valor: valor correspondente à conta com idConta
*               OP: operacao a efetuar (codigo das OP's definidas em macro)
*
* Returns: void
* Description:  Acrescenta novo comando a executar no buffer circular de dados
*               {Produtor} do sistema {Produtor} - Consumidor
*****************************************************************************************/
void produtor(int idConta, int idConta2, int valor, int OP) {

	/* Esperar */
	if (sem_wait(&podeProd) != 0) {
		printf("ERRO: sem_wait - &podeProd\n");
	}
	/* Fechar */
	if (pthread_mutex_lock(&semExMut) != 0) {
		printf("ERRO: pthread_mutex_lock - &semExMut\n");
	}

	cmd_buffer[buff_write_idx].operacao = OP;
	cmd_buffer[buff_write_idx].valor = valor;
	cmd_buffer[buff_write_idx].idConta = idConta;
	cmd_buffer[buff_write_idx].idConta2 = idConta2;
	buff_write_idx = (buff_write_idx + 1) % CMD_BUFFER_DIM; // Incrementa / Reinicia cursor que guarda indice de escrita
	StuffInside++;//Incrementa numero de comandos no contador
	
	/* Abrir */
	if (pthread_mutex_unlock(&semExMut) != 0) {
		printf("ERRO: pthread_mutex_unlock - &semExMut\n");
	}
	/* Assinalar */
	if (sem_post(&podeCons) != 0) {
		printf("ERRO: sem_post - &podeCons\n");
	}
}

/******************************************************************************************
* killThreadsSemaforos()
*
* Arguments:	Nenhum
*
* Returns: void
* Description:  Destroi todos os mutexes (de cada conta e do sistema Produtor - Consumidor)
*               Força todas as tarefas a sair
*               Após a saida de todas as tarefas, sincroniza-as
*               Destroi os 2 semaforos do sistema Produtor - Consumidor.
*****************************************************************************************/
void killThreadsSemaforosMutexes() {
	/* Percorre as tarefas todas e força para dar exit */
	for (int i = 0; i < NUM_TRABALHADORAS ; i++) {
		produtor(-1, -1, -1, OP_SAIR);
	}

	/* Sincroniza as tarefas todas */
	for (int i = 0; i < NUM_TRABALHADORAS ; i++) {
		int err = pthread_join(tid[i], NULL);
		if (err != 0)
			printf("Falha ao criar Thread :[%s]\n", strerror(err));
	}

	/* Destroi Mutex geral do Produtor - Consumidor */
	if (pthread_mutex_destroy(&semExMut) != 0) {
		printf("ERRO: pthread_mutex_destroy - params: &semExMut\n");
	}

	/* Destroi Mutexes das contas */
	for (int i = 0; i < NUM_CONTAS; i++) {
		if (pthread_mutex_destroy(&threadsContas[i]) != 0) {
			printf("ERRO: pthread_mutex_destroy - params: &threadsContas[i]\n");
		}
	}

	/* Destroi Variável de Condição */
	pthread_cond_destroy(&cheio);

	/* Destroi Semáforos */
	if (sem_destroy(&podeProd) != 0) {
		printf("Erro ao destruir semaforo podeProd\n");
	}
	if (sem_destroy(&podeCons) != 0) {
		printf("Erro ao destruir semaforo podeProd\n");
	}
}




