/******************************************************************************************
* File Name:    parte2e3.c
* Author:       Beatriz Correia (84696) / Manuel Sousa (84740)
* Revision:
* NAME:         Banco - IST/SO - 2016/2017 1º Semestre
* SYNOPSIS:     #include "parte2e3.h" - Prototipos e Estruturas usadas na entrega 2 e 3
* DESCRIPTION:  Contem todas as funções relativas à parte 2 e 3 do projeto sobre tarefas,
*               sistema Produtor - Consumidor e buffer circular de comandos
* DIAGNOSTICS:  OK
*****************************************************************************************/

#include "parte234.h"

/******************************************************************************************
* executarComando()
*
* Arguments:  c:  comando do buffer a executar
*
* Returns: void
* Description:  Executa um comando que recebe como argumento (Não executa 2 comandos da mesma conta ao mesmo tempo)
*****************************************************************************************/
void executarComando(comando_t c) {
	char output[100];
	switch (c.operacao) {
	case OP_LERSALDO:
		if (!contaExiste(c.idConta)) {
			snprintf(output, sizeof(output), "A conta %d nao existe\n", c.idConta);
			escrever(search(c.terminalPid)->data, output);
			break;
		}
		/* Fechar */
		if (pthread_mutex_lock(&threadsContas[c.idConta - 1]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_lock - &threadsContas[c.idConta-1]\n");
			escrever(search(c.terminalPid)->data, output);
		}

		int saldo = lerSaldo (c.idConta);
		if (lerSaldo(c.idConta) < 0) {
			snprintf(output, sizeof(output), "%s(%d): Erro.\n\n", COMANDO_LER_SALDO, c.idConta);
			escrever(search(c.terminalPid)->data, output);
		} else {
			snprintf(output, sizeof(output), "%s(%d): O saldo da conta é %d.\n\n", COMANDO_LER_SALDO, c.idConta, saldo);
			escrever(search(c.terminalPid)->data, output);
		}

		/* Abrir */
		if (pthread_mutex_unlock(&threadsContas[c.idConta - 1]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_unlock - &threadsContas[c.idConta-1]\n");
			escrever(search(c.terminalPid)->data, output);
		}
		break;

	case OP_CREDITAR:
		if (!contaExiste(c.idConta)) {
			snprintf(output, sizeof(output), "A conta %d nao existe\n", c.idConta);
			escrever(search(c.terminalPid)->data, output);
			break;
		}
		/* Fechar */
		if (pthread_mutex_lock(&threadsContas[c.idConta - 1]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_lock - &threadsContas[c.idConta-1]\n");
			escrever(search(c.terminalPid)->data, output);
		}

		if (creditar (c.idConta, c.valor) < 0) {
			snprintf(output, sizeof(output), "%s(%d, %d): Erro\n\n", COMANDO_CREDITAR, c.idConta, c.valor);
			escrever(search(c.terminalPid)->data, output);
		} else {
			snprintf(output, sizeof(output), "%s(%d, %d): OK\n\n", COMANDO_CREDITAR, c.idConta, c.valor);
			escrever(search(c.terminalPid)->data, output);
		}

		/* Abrir */
		if (pthread_mutex_unlock(&threadsContas[c.idConta - 1]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_unlock - &threadsContas[c.idConta-1]\n");
			escrever(search(c.terminalPid)->data, output);
		}
		break;

	case OP_DEBITAR:
		if (!contaExiste(c.idConta)) {
			snprintf(output, sizeof(output), "A conta %d nao existe\n", c.idConta);
			escrever(search(c.terminalPid)->data, output);
			break;
		}
		/* Fechar */
		if (pthread_mutex_lock(&threadsContas[c.idConta - 1]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_lock - &threadsContas[c.idConta-1]\n");
			escrever(search(c.terminalPid)->data, output);
		}

		if (debitar (c.idConta, c.valor) < 0) {
			snprintf(output, sizeof(output), "%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);
			escrever(search(c.terminalPid)->data, output);
		} else {
			snprintf(output, sizeof(output), "%s(%d, %d): OK\n\n", COMANDO_DEBITAR, c.idConta, c.valor);
			escrever(search(c.terminalPid)->data, output);
		}

		/* Abrir */
		if (pthread_mutex_unlock(&threadsContas[c.idConta - 1]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_unlock - &threadsContas[c.idConta-1]\n");
			escrever(search(c.terminalPid)->data, output);
		}

		break;
	case OP_TRANSFERIR:
		if ((!contaExiste(c.idConta) || !contaExiste(c.idConta2)) || c.idConta == c.idConta2) {
			snprintf(output, sizeof(output), "Erro ao transferir valor da conta %d para a conta %d.\n\n", c.idConta, c.idConta2);
			escrever(search(c.terminalPid)->data, output);
			break;
		}
		/* Fechar Contas relativas a operacao */
		if (pthread_mutex_lock(&threadsContas[MIN(c.idConta - 1, c.idConta2 - 1)]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_lock - &threadsContas[MIN(c.idConta-1, c.idConta2-1)]\n");
			escrever(search(c.terminalPid)->data, output);
		}
		if (pthread_mutex_lock(&threadsContas[MAX(c.idConta - 1, c.idConta2 - 1)]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_lock - &threadsContas[MAX(c.idConta-1, c.idConta2-1)]\n");
			escrever(search(c.terminalPid)->data, output);
		}

		if (transferir(c.idConta, c.idConta2, c.valor) < 0) {
			snprintf(output, sizeof(output), "Erro ao transferir valor da conta %d para a conta %d.\n\n", c.idConta, c.idConta2);
			escrever(search(c.terminalPid)->data, output);
		} else {
			snprintf(output, sizeof(output), "%s(%d, %d, %d): OK\n\n", COMANDO_TRANSFERIR, c.idConta, c.idConta2, c.valor);
			escrever(search(c.terminalPid)->data, output);
		}

		/* Abrir Contas relativas a operacao */
		if (pthread_mutex_unlock(&threadsContas[MAX(c.idConta - 1, c.idConta2 - 1)]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_unlock - &threadsContas[MAX(c.idConta-1,c.idConta2-1)]\n");
			escrever(search(c.terminalPid)->data, output);
		}
		if (pthread_mutex_unlock(&threadsContas[MIN(c.idConta - 1, c.idConta2 - 1)]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_unlock - &threadsContas[MIN(c.idConta-1,c.idConta2-1)]\n");
			escrever(search(c.terminalPid)->data, output);
		}
		break;
	case OP_SACOAZUL:
		if (c.idConta == 1) {
			snprintf(output, sizeof(output), "Saldo do saco azul %d\n\n", lerSaldo(1));
			escrever(search(c.terminalPid)->data, output);
			break;
		}
		else if (!contaExiste(c.idConta)) {
			snprintf(output, sizeof(output), "Erro Saco Azul\n\n");
			escrever(search(c.terminalPid)->data, output);
			break;
		}
		/* Fechar Contas relativas a operacao */
		if (pthread_mutex_lock(&threadsContas[0]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_lock - &threadsContas[MIN(c.idConta-1, c.idConta2-1)]\n");
			escrever(search(c.terminalPid)->data, output);
		}
		if (pthread_mutex_lock(&threadsContas[c.idConta - 1]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_lock - &threadsContas[MAX(c.idConta-1, c.idConta2-1)]\n");
			escrever(search(c.terminalPid)->data, output);
		}

		if (transferirSacoAzul(c.idConta) < 0) {
			snprintf(output, sizeof(output), "Erro %s\n\n", COMANDO_SACOAZUL);
			escrever(search(c.terminalPid)->data, output);
		} else {
			snprintf(output, sizeof(output), "%s OK\n\n", COMANDO_SACOAZUL);
			escrever(search(c.terminalPid)->data, output);
		}


		/* Abrir Contas relativas a operacao */
		if (pthread_mutex_unlock(&threadsContas[c.idConta - 1]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_unlock - &threadsContas[MAX(c.idConta-1,c.idConta2-1)]\n");
			escrever(search(c.terminalPid)->data, output);
		}
		if (pthread_mutex_unlock(&threadsContas[0]) != 0) {
			snprintf(output, sizeof(output), "ERRO: thread_mutex_unlock - &threadsContas[MIN(c.idConta-1,c.idConta2-1)]\n");
			escrever(search(c.terminalPid)->data, output);
		}
		break;
	case OP_SAIR:
		pthread_exit(NULL); /* Termina tarefa - ESTA FUNCAO TEM SEMPRE SUCESSO */
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
	int comando;
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
		buff_read_idx = (buff_read_idx + 1) % CMD_BUFFER_DIM; /* Incrementa / Reinicia cursor que guarda indice de leitura */
		comando = consumido.operacao;
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
		comandosNoBuffer--; /* Decrementa numero de comandos no contador */

		/* Fechar */
		if (pthread_mutex_lock(&semExMut) != 0) {
			printf("ERRO: pthread_mutex_lock - &semExMut\n");
		}

		escreverLog(comando);

		if (comandosNoBuffer == 0) {
			pthread_cond_signal(&cheio);
		}

		/* Abrir */
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
	int i;
	/* Incia Mutex geral do Produtor - Consumidor */
	if (pthread_mutex_init(&semExMut, NULL) != 0) {
		printf("ERRO: pthread_mutex_init - params: &semExMut\n");
	}
	/* Incia Mutexes das contas */
	for (i = 0; i < NUM_CONTAS; i++) {
		if (pthread_mutex_init(&threadsContas[i], NULL) != 0) {
			printf("ERRO: pthread_mutex_init - params: &threadsContas[i]\n");
		}
	}

	fout = open(LOG_FILE, O_CREAT | O_WRONLY | O_APPEND, 0666);

	/* Incia Semáforos */
	if (sem_init(&podeProd, 0, CMD_BUFFER_DIM) != 0) {
		printf("ERRO: sem_init - params: [&podeProd, 0, CMD_BUFFER_DIM]\n");
	}
	if (sem_init(&podeCons, 0, 0) != 0) {
		printf("ERRO: sem_init - params: [&podeCons, 0, 0]\n");
	}

	/* Inicia Variavel de Condicão */
	if (pthread_cond_init(&cheio, NULL) != 0) {
		printf("ERRO: pthread_cond_init - params: [&cheio, NULL]\n");
	}

	/* Inicia Tarefas */
	for (i = 0; i < NUM_TRABALHADORAS ; i++) {
		/* Cria tarefa e guarda o Thread ID num vetor, e atribui a função lerComandos à tarefa */
		int err = pthread_create(&(tid[i]), NULL, &lerComandos, NULL);
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
void produtor(comando_t comando) {

	/* Esperar */
	if (sem_wait(&podeProd) != 0) {
		printf("ERRO: sem_wait - &podeProd\n");
	}
	/* Fechar */
	if (pthread_mutex_lock(&semExMut) != 0) {
		printf("ERRO: pthread_mutex_lock - &semExMut\n");
	}

	cmd_buffer[buff_write_idx] = comando;
	buff_write_idx = (buff_write_idx + 1) % CMD_BUFFER_DIM; /* Incrementa / Reinicia cursor que guarda indice de escrita */
	comandosNoBuffer++; /* Incrementa numero de comandos no contador */

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
	int i;
	comando_t sair;
	sair.idConta = -1;
	sair.idConta2 = -1;
	sair.valor = -1;
	sair.operacao = OP_SAIR;
	/* Percorre as tarefas todas e força para dar exit */
	for (i = 0; i < NUM_TRABALHADORAS ; i++) {
		produtor(sair);
	}

	/* Sincroniza as tarefas todas */
	for (i = 0; i < NUM_TRABALHADORAS ; i++) {
		int err = pthread_join(tid[i], NULL);
		if (err != 0)
			printf("Falha ao criar Thread :[%s]\n", strerror(err));
	}

	close(fout);

	/* Destroi Mutex geral do Produtor - Consumidor */
	if (pthread_mutex_destroy(&semExMut) != 0) {
		printf("ERRO: pthread_mutex_destroy - params: &semExMut\n");
	}

	/* Destroi Mutexes das contas */
	for (i = 0; i < NUM_CONTAS; i++) {
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




