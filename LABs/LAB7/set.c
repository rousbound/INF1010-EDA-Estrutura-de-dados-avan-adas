#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "set.h"

struct set {
  int n;
  unsigned int members;
};

static int setMemberValid(int i) {
  if ((i<0) || (i>=MAX_MEMBERS)) return 0;
  return 1;
}

/* cria um conjunto com n elementos */
Set *setCreate(void){
  Set *set;
  set = (Set *)malloc(sizeof(Set));
  if (set != NULL) {
    set->n = MAX_MEMBERS;
    set->members = 0;
  }
  return set;
}

/* destroi (desaloca) o conjunto */
void setDestroy(Set *set) {
  if (set) free(set);
}

/* testa se o elemento i pertence ao conjunto */
int setIsMember(Set *set, int i){
  if (set==NULL) return 0;
  if (!setMemberValid(i)) return 0;
  return ((1<<i) & (set->members));
}

/* mostra os elementos do conjunto */
void setShow(char* title, Set *set){
  int i, first=1;
  printf("%s = {", title);
  for (i = 0; i < MAX_MEMBERS; i++) {
    if (setIsMember(set, i)) {
      if (first) { 
      	printf("%d", i); 
      	first = 0; 
      }
      else 
      	printf(",%d", i);
    }
  }
  printf("}\n\n");
}

/* cria uma copia do conjunto */
Set* setCopy(Set *set){
  if (set==NULL) return NULL;
	Set * temp = malloc(sizeof(Set));
		
  return temp; 
}

/* verifica se dois conjuntos sao iguais */
int setIsEqual(Set *set1,  Set *set2) {
  if (set1==NULL || set2 == NULL) return 0;
	if(set1->members == set2->members) return 1;
	
  return 0;
}

/* insere o elemento i no conjunto */
void setInsert(Set *set, int i) {
  if (set == NULL) return;
	int a = 0x01;
	a = a<<i;
	set->members = set->members | a;
}

/* remove o elemento i do conjunto */
void setRemove(Set *set, int i){
  if (set == NULL) return;
	int a = 0x01;
	a = a<<i;
	a = ~a;
	set->members = set->members & a;
	

}

/* cria um novo conjunto que e' complementar ao primeiro*/
Set *setComplement(Set *set){
  if (set == NULL) return NULL;
	Set * temp = setCopy(set);
	temp->members = ~set->members;
  return temp;
}

/* cria a uniao de dois conjunto que podem ter tamanhos diferentes */
/* naturalmente o numero de elementos do universo e' o do maior dos dois */
Set *setUnion(Set *set1,  Set *set2){
  if (set1==NULL || set2 == NULL) return NULL;
	Set * temp = setCreate();
	temp->members = set1->members | set2->members;
  return temp;
}

/* calcula a intersecao de dois conjuntos */
Set *setIntersection(Set *set1,  Set *set2){
  if (set1==NULL || set2 == NULL) return NULL;
	Set * temp = setCreate();
	temp->members = set1->members & set2->members;
  return temp;
}

/* calcula a diferenca de set1-set2 */
Set *setDifference(Set *set1, Set *set2){
  if (set1==NULL || set2 == NULL) return NULL;
	Set * temp = setCreate();
	temp->members = set1->members & ~set2->members;
		
  return temp;
}

/* verifica de set2 e' um subconjunto de set1 */
int setIsSubset(Set *set1,  Set *set2) {
  if (set1==NULL || set2 == NULL) return 0;
	int a;
	a = (~set1->members)&set2->members;
	//if (b == (a&b));
	if(a == 0)	
		return 1;
	else
  	return 0;
}

/* informa a cardinalidade do conjunto */
int setNumberOfElements(Set *set){
  if (set==NULL) return 0;
		
  int i, counter=0;
  for (i = 0; i < MAX_MEMBERS; i++) {
    if (setIsMember(set, i)) {
			counter++;
		}
	}	
  return counter;
}

