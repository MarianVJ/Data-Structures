/*
 ============================================================================
 Name        : Tema2.c
 Author      : Dan Novischi
 Version     :
 Copyright   : Your copyright notice
 Description : Dictionare
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVLTree.h"


/* Lungimea maxima a unui buffer */
#define BUFLEN 1024

/* Lungimea unui element din arbore */
#define ELEMENT_TREE_LENGTH 3
#define _GNU_SOURCE

/* Range of models*/
typedef struct Range{
	int *index; // Array of models in the range
	int size; // Number of array elements
	int capacity; // Array capacity
}Range;

void* createStrElement(void* str){
	
	char *sir = (char*) malloc(sizeof(char) *4);
	//pastrez din sirul dat ca parametru doar primele 3 elemente
	*(char*)(str+3) = '\0';
	strcpy(sir, (char*)(str));
	return sir;
}

void destroyStrElement(void* elem){
	
	free((char*)elem);
}


void* createPriceElement(void* price){
	
	long *l = malloc(sizeof(long));
	*l = *((long*) (price));
	return l;
}

void destroyPriceElement(void* price){
	
	free((long*)price);
}

void* createIndexInfo(void* index){
	
	int *l = malloc(sizeof(int));
	*l = *((int*) (index));
	return l;
}

void destroyIndexInfo(void* index){
	
	free((int*)index);
}

int compareStr(void* str1, void* str2){
	
	if( strcmp(((char*)str1),((char*)str2)) < 0)
		return -1;
	if( strcmp(((char*)str1),((char*)str2)) > 0)
		return 1;
	
	return 0;
}

int comparePrice(void* price1, void* price2){
	
	if(*((long*)price1) < *((long*)price2))
		return -1;
	if(*((long*)price1) > *((long*)price2))
		return 1;
	
	return 0;
	
}


void buildTreesFromFile(char* fileName, TTree* modelTree, TTree* priceTree){
	
	//deschid fisierul pentru citire
	FILE *f = fopen(fileName,"r");
	long aux;
	long i, j;
	int *index = (int*)malloc(sizeof(long));
	*index = 0;
	char *buf = (char*) malloc(BUFLEN+1);
	long *pret = (long*)malloc(sizeof(long));
	
	//cat timp mai am linii
	while(fgets(buf, BUFLEN, f) != NULL)
	{	
		//salvez in aux de fiecare data dimensiunea buferului
		aux = strlen(buf);
		//in model salvez modelul
		char* model = strtok(buf,",");
		//in price salvez pretul
		long price = atol(strtok(NULL,","));
		//pointerul pret il folosesc pentru a adaugat in priceTree		
		*pret = price;	
		
		//adaug pe rand in cei doi arbori modelul respectiv pretul
		//si indexul
		insert(modelTree,model,index);
		insert(priceTree,pret, index);
		//noul index va fi vechiul index plus aux ( care este 
		//(dimensiunea liniei curent)	
		*index = *index + aux;		
	}	
	//eliberez meoria alocata pentru bufer	
	free(buf);
	fclose(f);
}

Range* modelGroupQuery(TTree* tree, char* q){
	
	int i, sw;
	//aloc memorie pentru range
	Range *range = (Range*)malloc(sizeof(Range));
	//capacitatea va fi size-ul arborelui
	range->capacity = tree->size;
	//aloc memorie pentru vectorul de indexi
	range->index = (int *)malloc(sizeof(int) *range->capacity);
	//initial size ul rangelului este 0
	range->size = 0;
	//in nod salvez pozitia celui mai mare element
	//din arbore , mai mic decat q 
	//am folosit acesta metoda deoarece putem primi ca paramtru
	//si o singura litera si search ul nu ar fi mers aici
	TreeNode *nod = tree->root;
	while (tree->compare(nod->elem , q) > 0 )
	{
		nod = nod->lt;
	}
	//odata ce am cel mai apropiat element de 
	//q incep cautarea tuturor modeleleor 
	while (nod != tree->nil)
	{	//presupunem initial ca sunt egale nod->elem si q
		sw = 1;
		for (i = 0 ; i < strlen(q) ; i++)
		{
			//daca macar una din litere difera
			//atunci sw va fi 0 si nu vom adauga
			if (q[i] != ((char*)(nod->elem))[i] )
				sw = 0;		
		}
		//daca sw este 1 inseamna ca elementul este bun
		if (sw == 1)
		{			
			//adaugam elementul
			range->index[range->size] = *(int*)(nod->info);
			//si marim size ul
			range->size = range->size +1;
		}
		//cand nu mai gasesc elemente adica am trecut deja
		//de cel mai mare element care indeplineste conditia ceruta
		//nu mai am elemente in lista asa ca parasesc functia
		if ( sw == 0 && range->size > 0)
			break;
		//trecem la nodul urmator
		nod = nod->next;
	}
	//returnam range ul
	return range;
}

Range* modelRangeQuery(TTree* tree, char* q, char* p){
	 
	int i, sw1, sw2, flag = 0, flag_2 =0 ;
	//Aloc memorie pentru range si pentru vectorul de indexi
	Range *range = (Range*) malloc (sizeof(Range));
	range->capacity = tree->size;
	range->index = (int *)malloc(sizeof(int) *range->capacity);
	range->size = 0;
	//in nod salvez pozitia celui mai mare element
	//din arbore , mai mic decat q 
	//am folosit acesta metoda deoarece putem primi ca paramtru
	//si o singura litera si search ul nu ar fi mers aici
	TreeNode *nod = tree->root;
	while (tree->compare(nod->elem , q) > 0 )
	{
		nod = nod->lt;
	}
	
	while (nod != tree->nil)
	{
		//sw1 imi indixa daca nod->elem difera de primul model
		//sw2 imi indica daca nod->elem difera de al doilea model
		sw1 = 1;
		sw2 = 1;
		//Verific daca nod->elem difera de primul model
		for (i = 0 ; i < strlen(q) ; i++)
		{
			if (q[i] != ((char*)(nod->elem))[i] )
				sw1 = 0;		
		}
		//verific daca nod->elem difera de al doilea model
		for (i = 0 ; i < strlen(p) ; i++)
		{
			if (p[i] != ((char*)(nod->elem))[i] )
				sw2 = 0;		
		}
		//daca sw1 este 1 inseamna ca este vorba de primul model
		if (sw1 == 1)
		{			
			range->index[range->size] = *(int*)(nod->info);
			range->size = range->size +1;
		}
		//daca sw2 este 1 inseamna ca estse vorba de al doilea model
		if (sw2 == 1)
		{			
			flag = flag+1;
			range->index[range->size] = *(int*)(nod->info);
			range->size = range->size +1;
		}
		
		//daca nod->elem difera de q si de p dar totusi
		//avem deja elemente introduse in range ul de indecsi
		//inseamna ca ne aflam intre acestea , asa ca il adaugam
		//si cu flag stim ca nu am apucat sa adaugam indecsi de la p
		if (sw2 == 0 && sw1 == 0 && range->size > 0 && flag ==0)
		{
			flag_2 = flag_2 +1;
			range->index[range->size] = *(int*)(nod->info);
			range->size = range->size +1;
		}
		//daca am trecut si de mijloc si de seriile de elemente formate
		//din marginea finala , atunci am terminat de cautat
		//asa ca parasesc while ul
		if (flag_2 > 0 && flag > 0 && sw2 ==0)
			break;
	
		//trecem la nodul urmator
		nod = nod->next;
	}
	
	return range;
}

Range* priceRangeQuery(TTree* tree, long q, long p){
	
	//Aloc memorie pentru range si pentru range ul de indecsi
	Range *range = (Range*) malloc (sizeof(Range));
	range->capacity = 100;
	range->index = (int *)malloc(sizeof(int) *range->capacity);
	range->size = 0;
	//in nod salvez pozitia celui mai mare element
	//din arbore , mai mic decat q 
	//am folosit acesta metoda deoarece putem primi ca paramtru
	//si un numar ce nu exista in arbore , asa ca nu puteam folosi search
	TreeNode *nod = tree->root;
	long pret = *(long*)(nod->elem);
	while (pret  > q && nod->lt != tree->nil)
	{
		nod = nod->lt;
		pret = *(long*)(nod->elem);
	}
	
	while (nod != tree->nil)
	{
		//daca pretul nodului se afla in intervalul cerut atunci
		//il adaug in range 
		if (q <= *(long*)(nod->elem) && p >= *(long*)(nod->elem))
		{
			range->index[range->size] = *(int*)(nod->info);
			range->size = range->size +1;
		}
		//daca am trecut de limita superioara atunci ies 
		if (*(long*)(nod->elem) > p)
			break;
			
		//cat timp nu am depasit limita superioara continui sa caut
		nod = nod->next;
	}
	//returnez range ul final
	return range;
}

Range* modelPriceRangeQuery(char* fileName, TTree* tree, char* m1, char* m2, long p1, long p2){
	
	//pentru inceput apelez functia modelRangeQuery 
	//pentru a determina in prima faza cele corespunzatoare
	//pentru model dupa care dintre ele voi face selectia dupa pret
	Range *range = modelRangeQuery(tree,m1,m2);
	//range_final este range ul final , range este doar o
	//variabila temporara
	Range *range_final = (Range*)malloc(sizeof(Range));
	range_final->capacity = tree->size;
	range_final->index = (int *)malloc(sizeof(int) *range_final->capacity);
	range_final->size = 0;
	//deschid fisierul pentru a verifica fiecare model
	//si pretul acestuia sa vedem daca corespunde 
	//intervalului dorit
	FILE * file = fopen(fileName,"r");
	//aloc memorie pe heap pentru buferul 
	//unde vi tine pe rand fiecare linie pe care o voi citi
	//din fisier, citesc doar liniile corespunzatoare modelelor
	//gasita in prima faza , nu citesc toate liniile
	char *buf = (char*) malloc(BUFLEN + 1);

	for (int i = 0; i < range->size ; i++){
		//ma pozitionez exact la inceputul liniei pe care
		//doresc sa o citesc folosinduma de range 
		fseek(file , range->index[i] , SEEK_SET);
		if (fgets(buf , BUFLEN , file) != NULL)
		{
			//extrag pretul 
			char* model = strtok(buf,",");
			long price = atol(strtok(NULL,","));
			//daca pretul corespunza intervalului cerut
			//atunci il adaug in range ul meu final
			if (price >= p1 && price <= p2)
			{
				range_final->index[range_final->size] = range->index[i];
				range_final->size = range_final->size+1; 
			}
		}
	}
	//eliberez memoria alocata pentru range ul temporar in care am
	//tinut toate modelele din range ul cerut indiferent de pret-ul lor
	free(range->index);
	free(range);
	//eliberez memoria alocata pentru buferul in care retineam
	//fiecare linie
	free(buf);
	fclose(file);
	//returnez range ul final
	return range_final;
}


void printFile(char* fileName);
void inorderModelTreePrint(TTree* tree, TreeNode* node);
void inorderPriceTreePrint(TTree* tree, TreeNode* node);
void printRange(Range* range, char* fileName);


int main(void) {
	
	TTree* modelTree = createTree(createStrElement, destroyStrElement, createIndexInfo, destroyIndexInfo, compareStr);
	TTree* priceTree = createTree(createPriceElement, destroyPriceElement, createIndexInfo, destroyIndexInfo, comparePrice);

	buildTreesFromFile("input.csv", modelTree, priceTree);

	printf("Model Tree In Order:\n");
	inorderModelTreePrint(modelTree, modelTree->root);
	printf("\n\n");

	printf("Price Tree In Order:\n");
	inorderPriceTreePrint(priceTree, priceTree->root);
	printf("\n\n");

	printf("Group Model Search:\n");
	Range *range = modelGroupQuery(modelTree,"MG3");
	printRange(range,"input.csv");
	printf("\n\n");

	printf("Price Range Search:\n");
	Range *range2 = priceRangeQuery(priceTree,100,400);
	printRange(range2,"input.csv");
	printf("\n\n");

	printf("Model Range Search:\n");
	Range *range3 = modelRangeQuery(modelTree,"L2","M");
	printRange(range3,"input.csv");
	printf("\n\n");

	printf("Model Price Range Search:\n");
	Range *range4 = modelPriceRangeQuery("input.csv",modelTree,"L2","M", 300, 600);
	printRange(range4,"input.csv");
	
	free(range->index);
	free(range);
	free(range2->index);
	free(range2);
	free(range3->index);
	free(range3);
	free(range4->index);
	free(range4);
	destroyTree(priceTree);
	destroyTree(modelTree);
	return 0;
}



void inorderModelTreePrint(TTree* tree, TreeNode* node){
	if(node != tree->nil){
		inorderModelTreePrint(tree, node->lt);
		TreeNode* begin = node;
		TreeNode* end = node->end->next;
		while(begin != end){
			printf("%d:%s  ",*((int*)begin->info),(char*)begin->elem);
			begin = begin->next;
		}
		inorderModelTreePrint(tree, node->rt);
	}
}

void inorderPriceTreePrint(TTree* tree, TreeNode* node){
	if(node != tree->nil){
		inorderPriceTreePrint(tree, node->lt);
		TreeNode* begin = node;
		TreeNode* end = node->end->next;
		while(begin != end){
			printf("%d:%ld  ",*((int*)begin->info),*((long*)begin->elem));
			begin = begin->next;
		}
		inorderPriceTreePrint(tree, node->rt);
	}
}

void printRange(Range* range, char* fileName){
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL) return;

	char *buf = (char*) malloc(BUFLEN+1);

	for(int i = 0; i < range->size;i++){
		fseek(file,range->index[i],SEEK_SET);
		if(fgets(buf,BUFLEN,file) != NULL){
			char* model = strtok(buf,",");
			long price = atol(strtok(NULL,","));
			printf("%s:%ld  ", model, price);
		}
	}
	printf("\n");
	free(buf);
	fclose(file);
}

void printFile(char* fileName){
	printf("---------\n");
	if(fileName == NULL) return;
	FILE * file = fopen(fileName,"r");
	if (file == NULL)
		return;

	char *buf = (char*) malloc(BUFLEN+1);
	while(fgets(buf,BUFLEN,file) != NULL){
		buf[strlen(buf) - 1] = '\0';
		printf("%s",buf);
	}
	printf("\n");
	printf("---------\n");
	free(buf);
	fclose(file);
}
