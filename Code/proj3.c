#include "list.h"


int main(int argc, char *argv[]){
 
	list *jogo;
 
 	int i,NJogadas,jogada[2];
	char cor;
 
	jogo = lst_new();	/* Inicialização da lista */
	removidas = lst_new();
	tetramino_init();
	
	scanf("%d",&NJogadas);
	
	if(NJogadas == 0){   
		printf("%d\n",jogo->Lmax);
		printf("%d\n\n",jogo->pontuacao);
		lst_print_jogo(jogo);
		printf("\n%d\n", jogo->activos);
		printf("\nNo eliminations\n");
		lst_destroy(jogo);
		lst_destroy(removidas);
		return 0;
	}/*Se o nº de jogadas for 0 imprime o limite inferior, o LMax, a pontuação e retorna 0, terminando assim a função.*/
	
	lst_add_line(jogo);		/*Se não for 0, adiciona uma nova linha*/
	
	for (i = 0; i < NJogadas; i++){
		scanf("%d %d %c",&jogada[0],&jogada[1],&cor);
		lst_insere(jogo,jogada,cor);
	}/*Este ciclo é percorrido até ao número de jogadas, e faz a inserção das jogadas e elimina-as ao mesmo tempo.*/

	printf("%d\n",jogo->Lmax);
	printf("%d\n\n",jogo->pontuacao);
	
	lst_print_jogo(jogo);
	tetraminos_print(jogo);
	mediana(removidas);
	
	/*Liberta da memória as estruturas dos tetraminos*/
	free(um);
	free(dois);
	free(tres);
	free(quatro);
	free(cinco);
	
	/*Liberta da memória as varias listas e sub-listas*/
	lst_destroy(jogo);
	lst_destroy(removidas);
	return 0;
}
