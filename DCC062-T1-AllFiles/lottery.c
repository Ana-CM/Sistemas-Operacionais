/*
*  lottery.c - Implementacao do algoritmo Lottery Scheduling e sua API
*
*  Autores: Ana Carolina Mendes Lino - 201865212AC
*			Bráulio do Amaral Barros - 201565500B
*
*  Projeto: Trabalho Pratico I - Sistemas Operacionais
*  Organizacao: Universidade Federal de Juiz de Fora
*  Departamento: Dep. Ciencia da Computacao
*/

#include "lottery.h"
#include <stdio.h>
#include <string.h>

//Nome unico do algoritmo. Deve ter 4 caracteres.
const char lottName[]="LOTT";
int slot;

//=====Funcoes Auxiliares=====

// função copiada da main.c
// conta o numero de processos prontos
int countReady(Process *plist) {
	Process *p, *next;
	int count = 0;
	for (p=plist; p!=NULL; p=next) {
		next = processGetNext(p);
		if (processGetStatus(p)==PROC_READY)
			count++;
	}
	return count;
}

//=====Funcoes da API=====

//Funcao chamada pela inicializacao do S.O. para a incializacao do escalonador
//conforme o algoritmo Lottery Scheduling
//Deve envolver a inicializacao de possiveis parametros gerais
//Deve envolver o registro do algoritmo junto ao escalonador
void lottInitSchedInfo()
{
    SchedInfo *schedInfo = malloc(sizeof(SchedInfo)); //SchedInfo inicializado

    //parametros do algoritmo
    strcpy(schedInfo->name, lottName);                 // recebe nome unico 'LOTT' definido anteriormente
    schedInfo->initParamsFn = lottInitSchedParams;    // recebe info dos parametros de um processo p, da func
    schedInfo->scheduleFn = lottSchedule;            // recebe info do processo que ira obter CUP, vem da func
    schedInfo->releaseParamsFn = lottReleaseParams; // recebe a info de qual parametro de escalonamento de um processo p, que ira ser liberado na func

    slot = schedRegisterScheduler(schedInfo); //Registra o algoritmo e a var slot recebe qual slot foi ganho, no nosso caso vai ser 0 pq ta vazio o vetor de escalonamento
}

//Inicializa os parametros de escalonamento de um processo p, chamada 
//normalmente quando o processo e' associado ao slot de Lottery

/**
 * é chamada para um certo processo quando o simulador decide qual algoritmo de escalomento vai ser utilizado
 * o void *params é usado pois não se deseja que o tipo esteja pre-estabelecido, isso permite que o tipo seja decido no uso
 **/
void lottInitSchedParams(Process *p, void *params)
{
	int aux = schedSetScheduler(p, params, slot); //Associa um processo a um algoritmo de escalonamento especifico // 'atalho' para setar tanto o parametro, tanto o algoritmo de escalonamento

	if( aux == -1){
		printf ("ERRO - Algoritmo de escalonamento não encontrado!");
		exit(0);
	}
}

//Retorna o proximo processo a obter a CPU, conforme o algortimo Lottery 
Process* lottSchedule(Process *plist) 
{
	Process *p = plist;
	LotterySchedParams *aux;
	int ready = countReady(plist), totalTickets = 0, index = 0, sort, **M;

	M = malloc (ready * sizeof (int *));

   	for (int i = 0; i < ready; i++){
		M[i] = malloc (2 * sizeof (int)); 
   	}
      
	while (p != NULL)
	{
		if(processGetStatus(p) == PROC_READY)
		{
			aux = processGetSchedParams(p);
			totalTickets = totalTickets + aux->num_tickets;

			M[index][1]= processGetPid(p);

			if(index == 0){
			   M[index][2] = aux->num_tickets;
			}else{
			   M[index][2] = aux->num_tickets + M[index-1][2];
			}

			index++;
		}
		p = processGetNext(p);
	}

	sort = (rand() % totalTickets) + 1; 

	for(int k = 0; k < ready; k++)
	{	
		if(sort <= M[k][2]){
		  p =  processGetByPid(plist, M[k][1]);
		  return p;
		}
	}
	
	return NULL;
}

//Libera os parametros de escalonamento de um processo p, chamada 
//normalmente quando o processo e' desassociado do slot de Lottery
//Retorna o numero do slot ao qual o processo estava associado
int lottReleaseParams(Process *p) {

    free(processGetSchedParams(p));
	
	return slot;
}

//Transfere certo numero de tickets do processo src para o processo dst.
//Retorna o numero de tickets efetivamente transfeirdos (pode ser menos)
int lottTransferTickets(Process *src, Process *dst, int tickets) {
	/**
	 * dado um processo de origem (src) e um de destino (dst), transferir ticket do origem para destino
	 * tomar cuidado com os casos q pede p transf mais ticket q tem
	 * decrementar src e imcrementar dst
	 * */
	return 0;
}
