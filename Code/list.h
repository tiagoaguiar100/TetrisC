/*
 * list.h - defenições e declarações da lista jogo
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAXCOL 10

typedef struct lst_item{
	int indice;				/*posiçao da linha de jogo */
	char value[MAXCOL+1];	/* linha de jogo */
	struct lst_item *next;
	struct lst_item *prev;
	struct lst_pair *tetraminos;
} item;

typedef struct{		
   item *first;			
   item *last;				/*Estrutura principal do jogo*/
   int Lmax;
   int pontuacao;
   int activos;
} list;

typedef struct lst_pair{
	int tetra;
	char cor;				/*Lista de pares de tetraminos*/
	int indice;
	int pos;
	struct lst_pair *next;
}pair;

typedef struct{
	int B, G, O, R, V, Y;	/*Estrutura que contém as cores dos tetraminos*/
} tetramino;

list *removidas;
tetramino *um, *dois, *tres, *quatro, *cinco;

/* lst_new - Aloca memoria para a lista e inicializa-a */
list* lst_new();

/* lst_destroy - Liberta da memória a lista e todos os seus elementos */
void lst_destroy(list *lista);

/* lst_insert - Insere uma nova linha em branco na lista */
void lst_add_line(list *lista);

/* lst_insere - Insere um jogada no jogo */
void lst_insere(list *jogo,int jogada[2],char cor);

/* lst_remove - Remove a linha de indice 'value' da lista */
void lst_remove(list *jogo, item *ptr);

/* lst_print - Faz print da lista para a STDOUT */
void lst_print_jogo(list *lista);

/*------------------------*/
/* Funções para a parte 3 */
/*------------------------*/
/* tetraminos_print - Faz print das estruturas tetramino */
void tetraminos_print(list *jogo);

/* get_tetramino - Devolve a estrutura tetramino correspondente ao inteiro dado */
tetramino *get_tetramino(int tetra);

/* tetramino_init - Inicializa a estrutura tetramino */
void tetramino_init();

/* insere_ordenada - Insere por ordem alfabetica */
void insere_ordenada(list *lista, item *linha);

/* mediana - Imprime a posição central da lista de linhas removidas */
void mediana(list *lista);
