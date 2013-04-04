#include "list.h"



list *lst_new() {
   list *lista = (list*) malloc(sizeof(list));
   lista->first = NULL;
   lista->last = NULL;
   lista->Lmax = 0;
   lista->pontuacao = 0;
   lista->activos = 0;
   return lista;
}/*Cria uma estrutura que contém dois ponteiros para a primeira e para a última linha, o valor de LMax e a pontuação total.*/


void lst_destroy(list *lista)
{
	pair *par;
	item *ptr;
	for(ptr = lista->first; lista->first != NULL; ptr = lista->first){ 
	   lista->first = lista->first->next;
		for(par = ptr->tetraminos; ptr->tetraminos != NULL; par = ptr->tetraminos){
			ptr->tetraminos = ptr->tetraminos->next;
			free(par);  
		}
	   free(ptr);
	}
	free(lista->first);
	free(lista);
}/*Liberta o jogo todo da memória.*/


void lst_add_line(list *jogo) {
	
	int i;
	item *new = (item*)malloc(sizeof(item));
	
	jogo->Lmax++;
	new->indice = jogo->Lmax;
	new->tetraminos = NULL;
	
	for (i = 0; i < MAXCOL ; i++)
		new->value[i] = ' ';
	new->value[10] = '\0'; 
	new->next = NULL;
	new->prev = jogo->last;
	
	if(jogo->first == NULL){  /*Caso a lista esteja vazia*/
		jogo->first = jogo->last = new;
		new->prev = NULL;
	}
	else{
	jogo->last->next = new;
	jogo->last = new;
	}
}/*Aloca memória para um novo item dentro do jogo.*/

void organiza_indices(list *jogo, item *ptr){
	
	for (; ptr != NULL; ptr = ptr->next)
		ptr->indice--;
}/*Organiza os índices correspondentes às linhas do jogo.*/

tetramino *get_tetramino(int tetra){
		
	switch (tetra){
		case 1: 
			return um;
		case 2:
			return dois;
		case 3:
			return tres;
		case 4:
			return quatro;
		case 5:
			return cinco;
		default:
			printf(" get_tetramino - Estrutura inexistente\n");
			exit(1);	
	}
}/*Recebe um inteiro e devolve a estrutura associada ao tetramino*/

void tetramino_init(){
		
		int i;
		tetramino *ptr;
		
		um = (tetramino*)malloc(sizeof(tetramino));
		dois = (tetramino*)malloc(sizeof(tetramino));
		tres = (tetramino*)malloc(sizeof(tetramino));
		quatro = (tetramino*)malloc(sizeof(tetramino));
		cinco = (tetramino*)malloc(sizeof(tetramino));
		
		for(i = 1; i<=5; i++){
			ptr = get_tetramino(i);
			ptr->B = ptr->G = ptr->O = ptr->R = ptr->V = ptr->Y = 0;
		}
		
}/*Inicia as 5 estruturas do tetramino*/

void add_tetramino(item *linha, int tetramino, char cor, int pos, int indice){
	
		pair *ptr, *aux;
		
		if((ptr = linha->tetraminos) == NULL){
			ptr = (pair*)malloc(sizeof(pair));
			ptr->tetra = tetramino;
			ptr->cor = cor;
			ptr->pos = pos;
			ptr->indice = indice;
			ptr->next = NULL;
			linha->tetraminos = ptr;
			return;
			}
		for(; ptr->next != NULL; ptr = ptr->next);
		
		aux = ptr;
		ptr = ptr->next;
		ptr = (pair*)malloc(sizeof(pair));
		ptr->tetra = tetramino;
		ptr->cor = cor;
		ptr->pos = pos;
		ptr->indice = indice;
		ptr->next = NULL;
		aux->next = ptr;
}/*Adiciona o tetramino á lista de tetraminos da linha dada*/

int remove_tetramino(item *linha, int indice){
	
	pair *ptr = linha->tetraminos, *p;
	
	if(linha->tetraminos == NULL)
		return 1;
	
	if(ptr->indice == indice){
		linha->tetraminos = linha->tetraminos->next;
		free(ptr);
		return 0;
	}
	while(ptr->next->indice != indice)
		ptr = ptr->next;
	
	p = ptr;
	ptr = ptr->next;
	p->next = ptr->next;
	free(ptr);
	return 0;
}/*Remove o tetramino á lista de tetraminos da linha dada*/

void soma_tetramino(int tetra ,char cor, int soma){
		
		tetramino *ptr;
		ptr = get_tetramino(tetra);
		
		switch (cor){
			case 'B': 
				ptr->B += soma;
				break;
			case 'G':
				ptr->G += soma;;
				break;
			case 'O':
				ptr->O += soma;
				break;
			case 'R':
				ptr->R += soma;
				break;
			case 'V':
				ptr->V += soma;
				break;
			case 'Y':
				ptr->Y += soma;;
				break;
			default:
				printf(" soma_tetramino - COR Inexistente\n");
				exit(1);	
		}
}/*Recebe o valor a somar ao nº de tetraminos existentes*/

void lst_remove(list *jogo, item *ptr)
{
   item *before, *after;
   pair *p;
   int protect = 0;
   
   if(jogo->first == NULL)
		return;
	
	if(ptr == NULL)
		return;
	
	jogo->Lmax--;
	
	for(p = ptr->tetraminos; p != NULL; p = p->next){
		if(p->tetra != 1){
			if(p->pos == 0)
				protect = remove_tetramino(ptr->next, p->indice);
			else
				protect = remove_tetramino(ptr->prev, p->indice);
		}
		if (!protect)
			soma_tetramino(p->tetra, p->cor, -1);
		jogo->activos--;
	}
	
	insere_ordenada(removidas, ptr);
	
	/*Liberta da memória a lista dos pares de tetraminos*/
	for(p = ptr->tetraminos; ptr->tetraminos != NULL; p = ptr->tetraminos){
			ptr->tetraminos = ptr->tetraminos->next;
			free(p); 
	}
	
	if(jogo->first == ptr){
		jogo->first = jogo->first->next;
		if(jogo->first == NULL){
			jogo->last = jogo->first;
			free(ptr);
			return;
		}
		jogo->first->prev = NULL;
		free(ptr);
		organiza_indices(jogo, jogo->first);
		return;}
		
	if(jogo->last == ptr){
		jogo->last = jogo->last->prev;
		jogo->last->next= NULL;
		free(ptr);
		return;}
   
   before = ptr->prev;
   after = ptr->next;
   
   before->next = after;
   after->prev = before;
   free(ptr);
   organiza_indices(jogo, after);
}/*Remove um item pertencente ao jogo.*/

void lst_print_jogo(list *lista) {
	int i,j;
	item *ptr = lista->last;
	
	i = lista->Lmax;

	while(ptr != NULL){
		printf("|");
		for (j = 0; j < MAXCOL; j++)
			printf("%c",ptr->value[j]);
		printf("| %d\n", i--);
		ptr = ptr->prev;
	}
	 printf(" ----------\n");
}/*Imprime o jogo no STDOUT.*/

item *posicaoVaga(list *jogo, int coluna){

	item *ptr;
	
	if(jogo->last->value[coluna] != ' '){		/* Aloca espaço caso seja perciso */
		lst_add_line(jogo);						
		return jogo->last;
	}

	ptr = jogo->last;

	if(ptr->prev != NULL)
		while(ptr->prev != NULL){
			if(ptr->prev->value[coluna] != ' ')
				return ptr;
		ptr = ptr->prev;
	
	}

	return ptr;
}/*Procura a posição vaga da referida coluna no jogo para a colocação das peças.*/

int pertence(char elem, char vect[]){
	
	int i;
	
	for (i = 0; i < 10; i++)
		if (vect[i] == elem)
			return 1;
	
	return 0;
}/*Verifica se um dado elemento faz parte do vector.*/

int ElemDiferentes(char vect[]){
	
	int i,j,res;
	char aux[10];
	
	for (i = 0; i < 10; i++)
		aux[i]=' ';

	for (j = 0,i = 0,res = 0; i < 10; i++)
		if(pertence(vect[i],aux) == 0){
			aux[j++]=vect[i];
			res++;
		}	
				
	return res;
}/*Retorna o número de elementos diferentes de um vector.*/

int existe_espaco(item *linha){
	
	int i;
	
	for (i = 0; i < MAXCOL; i++)
		if(linha->value[i] == ' ')
			return 1;
	
	return 0;
}/*Retorna 1 caso exista espaço, e 0 em caso contrário.*/



void linha_preenchida(list *jogo, item *linha, int dupla){

	item *ptr, *aux = linha->next;
	
	if (existe_espaco(linha)){
		if(dupla)
			linha_preenchida(jogo, aux, 0);
		return;
	}
	if(ElemDiferentes(linha->value) > 1){  /* Caso seja multicolor remove apenas uma linha  */
		lst_remove(jogo, linha);
		jogo->pontuacao++;
		if(dupla)  /* Se tetramino tiver o tanho de duas linhas "dupla" vem a 1 para verificar a linha e a linha->next */
			linha_preenchida(jogo, aux, 0);
	}
	else{ /* Caso se monocolor */
		jogo->pontuacao += 3;
		lst_remove(jogo, linha->prev);
			if((ptr = linha->next) != NULL)
				if(!existe_espaco(ptr))
					while(ElemDiferentes(ptr->value) == 1){ /* Ciclo que precorre o jogo até encontrar uma linha que não é monocolor */
						jogo->pontuacao += 3;
						if(ptr->next != NULL)
							if(!existe_espaco(ptr->next))
								ptr=ptr->next;
							else{
								ptr=ptr->next;
								break;
							}
						else
							break;
					}
				if (ptr != NULL)
					while(ptr != linha){ /* ptr nesta situação está na linha máxima a ser eliminada, e vai eliminando até chegar á monocolor original */
						aux = ptr;
						ptr=ptr->prev;
						lst_remove(jogo, aux);
					}
		lst_remove(jogo, linha);/* Remove e linha que originou o ciclo de monocolores */
	}
}/* Esta função verifica se após a colocaçao do tetramino, ficaram linhas monocolores ou multicolores para a remoção */

void lst_insere(list *jogo,int jogada[2],char cor){
	
	static int indice = 0;
	int i,tetramino = jogada[0],
		  coluna = jogada[1]-1;
	
	item *linha_actual, *ptr;
	
	indice++;
	
	if(jogo->first == NULL)
		lst_add_line(jogo);
		
	linha_actual = jogo->first;
		
	switch (tetramino){													/*Desenhos dos cinco tetraminós.*/
		case 1:															/*cccc*/
			for (i = coluna; i < coluna + 4; i++)
				if((ptr = posicaoVaga(jogo,i))->indice > linha_actual->indice)
					linha_actual = ptr;
			for (i = coluna; i < coluna + 4; i++)
				linha_actual->value[i] = cor;
			break;
		case 2:
			for (i = coluna; i < coluna + 2; i++)
				if((ptr = posicaoVaga(jogo,i))->indice > linha_actual->indice)
					linha_actual = ptr;
			if(linha_actual->indice == jogo->Lmax)
				lst_add_line(jogo);
			for (i = coluna; i < coluna + 2; i++)						/*cc*/
				linha_actual->next->value[i] = cor;						/*cc*/
			for (i = coluna; i < coluna + 2; i++)						
				linha_actual->value[i] = cor;						
			break;
		case 3:			
			i = coluna;
			for (i = coluna + 1; i < coluna + 3; i++)
				if((ptr = posicaoVaga(jogo,i))->indice > linha_actual->indice - 1)
					linha_actual = ptr;
			if((ptr = posicaoVaga(jogo,coluna))->indice > linha_actual->indice)
				linha_actual = ptr->prev;
			if(linha_actual->indice == jogo->Lmax)
				lst_add_line(jogo);											/*cc */
			for (i = coluna; i < coluna + 2; i++)							/* cc*/
				linha_actual->next->value[i] = cor;
			for (i = coluna + 1; i < coluna + 3; i++)
				linha_actual->value[i] = cor;	
			break;
		case 4:	
			if((ptr = posicaoVaga(jogo,coluna+1))->indice > linha_actual->indice)
				linha_actual = ptr;
			for (i = coluna; i < coluna + 3; i = i+2)
				if((ptr = posicaoVaga(jogo,i))->indice > linha_actual->indice + 1)
					linha_actual = ptr->prev;			
			if(linha_actual->indice == jogo->Lmax)
				lst_add_line(jogo);														
			for (i = coluna; i < coluna + 3; i++)						/*ccc*/
				linha_actual->next->value[i] = cor;						/* c */
			linha_actual->value[coluna+1] = cor;	
			break;
		case 5:
			i = coluna;
			if((ptr = posicaoVaga(jogo,i++))->indice > linha_actual->indice)
				linha_actual = ptr;
			for (; i < coluna + 3; i++)
				if((ptr = posicaoVaga(jogo,i))->indice > linha_actual->indice + 1)
					linha_actual = ptr->prev;			
			if(linha_actual->indice == jogo->Lmax)
				lst_add_line(jogo);										/*ccc*/
			for (i = coluna; i < coluna + 3; i++)						/*c  */
				linha_actual->next->value[i] = cor;
			linha_actual->value[coluna] = cor;
			break;				
			
		}	
		jogo->activos++;
		soma_tetramino(tetramino ,cor, 1);
		add_tetramino(linha_actual,tetramino,cor,0, indice);
		if(tetramino != 1)
			add_tetramino(linha_actual->next,tetramino,cor, 1, indice);
		
		if(tetramino == 1)
			linha_preenchida(jogo, linha_actual, 0);
		else
			linha_preenchida(jogo, linha_actual, 1);
}/*Esta função insere os vários tetraminos no jogo e també verificar se é perciso alocar espaço*/

void elimina_espacos(item *new){
	
	int i,j;
	char aux[MAXCOL+1];
	
	for(i = j = 0; i != MAXCOL; i++)
		if(new->value[i] != ' ')		/* Copia todas as letras */
			aux[j++] = new->value[i];
			
	aux[j] = '\0'; 
	
	strcpy(new->value, aux); /* Manda a resposta para o item */
}/*Elimina os espaços da string do item dado*/

void insere_ordenada(list *lista, item *linha) {

	item *ptr, *new = (item*)malloc(sizeof(item));
	
	strcpy(new->value, linha->value);
	elimina_espacos(new);  /* Modifica directamente no Item */
	
	new->prev = new->next = NULL;
	new->tetraminos = NULL;

	lista->Lmax++;
	
	if(lista->first == NULL){  /* Caso a lista esteja vazia */
		lista->first = lista->last = new;
		new->prev = new->next = NULL;
		return;
	}

	if(strcmp(new->value, lista->first->value) <= 0){ /* Caso seja menor que o 1º */
		new->next = lista->first;
		new->prev = NULL;
		lista->first->prev = new;
		lista->first = new;
		return;
	}
		

	for(ptr = lista->first;ptr->next != NULL; ptr = ptr->next) { /* Caso seja no meio */
		if(strcmp(new->value, ptr->value) > 0)
			continue;
		
		new->next = ptr;
		new->prev = ptr->prev;
		ptr->prev->next = new;
		ptr->prev = new;
		return;
	}
	/* Caso seja no fim */
	ptr->next = new;
	new->prev = ptr;
	new->next = NULL;
	lista->last = new;

}/*Esta função insere ordenadamente as várias linhas eliminadas*/

void mediana(list *lista){

	int i = lista->Lmax/2;
	item *ptr;
	
	if(lista->first==NULL){ 		/*Caso nao haja eliminações*/
		printf("\nNo eliminations\n");
		return;
	}

	if((lista->Lmax%2) > 0)
		i++;

	for(ptr = lista->first; i > 1; ptr = ptr->next, i--);
	
	printf("\n%s\n",ptr->value);
}/*Esta função calcula a mediana e escreve-a no STDOUT*/

void tetraminos_print(list *jogo){
	
	int i;
	tetramino *ptr;
	
	printf("\n%d\n",jogo->activos);
	
	for(i = 1; i <=5; i++){
		ptr = get_tetramino(i);
		for(; ptr->B > 0; ptr->B--)
			printf("%d B\n",i );
		for(; ptr->G > 0; ptr->G--)
			printf("%d G\n",i );
		for(; ptr->O > 0; ptr->O--)
			printf("%d O\n",i );
		for(; ptr->R > 0; ptr->R--)
			printf("%d R\n",i );
		for(; ptr->V > 0; ptr->V--)
			printf("%d V\n",i );
		for(; ptr->Y > 0; ptr->Y--)
			printf("%d Y\n",i );
	}
}/*Esta funçao imprime no STDOUT o nº de tetraminos activos, 
e tambem lista os varios tetraminos activos como a sua cor*/
