#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int simula(int a){
	pid_t Pid,pidTerminou;	
	int Estado;
	Pid=fork();
	if(Pid<0){
		printf("Erro");
		exit (EXIT_FAILURE);
	}
	if (Pid == 0) {
		printf("FILHO %d\n",Pid);
		sleep(5);
		printf("FILHO2 %d\n",Pid);
		exit(EXIT_SUCCESS);
	} else if (Pid > 0){
		printf("Cricou um subprocesso de identificador = %d\n",Pid);
		//pidTerminou = wait(&Estado);
		pidTerminou = waitpid(pid,&Estado,0)
		printf("Terminou o Subprocesso de Pid = %d\n",Pid);
		return a;
	}
	return a;
}

int main(){
	int achas,a;
	while(1){
		scanf("%d",&achas);
		if (achas == 1){
			simula(a);
		}
		else if(achas == 2){
			printf("ABSDJBASDJSA\n");
		}
	}
	return 0;
}