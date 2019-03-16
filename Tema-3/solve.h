#ifndef __SOLVE_H__
#define __SOLVE_H__
#include<stdio.h>
#include<stdlib.h>
#define MAX_MAX 1024

typedef struct {
  int nn; // numar noduri
  int **Ma; // matrice de adiacenta
} TGraphM;

typedef struct node {
  int v; // indicele nodului destinatie
  int c; // cost asociat arcului
  struct node *next;
} TNode, *ATNode;

typedef struct {
  int nn; // numar noduri
  ATNode *adl; // vector de pointeri alocati dinamici la vecini
} TGraphL;

//functia de eliberare a memoriei alocate pentru o matrice de adiacenta
void free_matrix(TGraphM *G)
{
	int i ;
	for (i = 0  ; i < G->nn; i ++)
		free(G->Ma[i]);
	free(G->Ma);
}

//functie care aloca matricea de adiacenta 
void alloc_matrix(TGraphM *G, int n)
{	
	G->nn = n;
	G->Ma = (int **)malloc(n*sizeof(int*));
	int i;
	for (i = 0 ; i < n ; i++)
		G->Ma[i] = (int*) malloc (n*sizeof(int));	
}
//G este matricea de adiacenta , iar G2 este matricea
//in care voi retine zerorurile incadrate si barate 
int rezolva(TGraphM *G,TGraphM *G2)
{
	//linie si coloana sunt doi vectori cu ajutorul carora
	//retin la fiecare pas liniie si coloanele cu anumite proprietati
	//pe care eu trebuie se la marchez ca avand acea proprietate
	int coloana[G->nn],linie[G->nn];
	int i,j;
	int n = G2->nn;
	int min, aux, sw, count, aux2, incadrat;
	//Voi parasi acest while doar atunci cand va fi determinata
	//solutia optima pentru problema 
	while (1)
	{
		//La fiecare pas atat timp cat nu gasim sollutia optima
		//matricea de zerouri trebuie initializata cu 0 
		for (i = 0  ; i < n ;i++)
			for (j = 0  ; j < n ; j++)
				G2->Ma[i][j] = 0;
		
		//variabila care retine numarul de zerouri incadrate
		incadrat = 0;
		//UN ZERO INCADRAT --> REPREZENTAT IN G2 prin 1
		//UN ZERO BARAT --> REPREZENTAT IN G2 prin -1
		while(1)
		{
			min = MAX_MAX;
			sw =0; 
			//In cadrul acestui for gasesc de fiecare data numarul
			//minim de zerouri de pe linie , atat timp
			//cat mai am zeroruri nebarate sau nemarcate
			for (i = 0 ; i < n ; i++)
			{	
				sw = 0;
				count = 0;
				for (j = n ; j < 2 * n ; j ++)
					if (G->Ma[i][j] == 0 && G2->Ma[i][j-n] == 0)
					{
						count++;
						sw = 1;
					}
				if (count < min && sw == 1)
				{
					min = count;
					aux = i;
				}
			}
			//minimul initial este o mie daca acesta ramane
			//neschimbat in urma forului inseamna ca toate
			//zerourile sunt fie marcate fie barate
			if (min == MAX_MAX)
				break;
			//daca nu continui sa marchez zeroul de pe liniea curenta
			//si sa barez restul de zerouri de pe linia si coloana pe
			//care se afla elementul	
			sw = 0;
			for (i = 0 ; i < n ; i++)
				if ( G->Ma[aux][i+n] == 0 && sw == 1&& G2->Ma[aux][i] == 0)
					G2->Ma[aux][i] = -1;
				else
					if (G->Ma[aux][i+n] == 0 && sw == 0 && G2->Ma[aux][i] ==0)
					{	
						G2->Ma[aux][i] = 1;
						sw = 1;
						aux2 = i; 
						//de fiecare data cand incadrez un zero
						//contorizez acest lucru 
						incadrat ++;
					}
					
			//Cu alte cuvinte in matricea G2 un zero marcat este reprezentat
			//prin 1 iar un zero barat este reprezentat prin -1	
			for (i = 0 ; i < n ; i ++)
				if (G->Ma[i][aux2+n] == 0 && G2->Ma[i][aux2] == 0)
					G2->Ma[i][aux2] = -1;
		}

		//Daca exista n zerouri incadrate atunci solutia optima
		//pentru problema noastra a fost gasita asa ca 
		//parasim while ul 		
		if (incadrat == n)
			break;
		//in vectorii linie si coloana retin numarul liniei sau 
		//coloanei pe care urmeaza sa o marchez 		
		for(i = 0 ; i < n ; i ++)
			linie[i] = coloana[i] = 0;
		//Marchez mai intai toate liniile care nu au zero incadrat	
		for (i = 0  ; i < n ; i ++)
		{
			sw = 0 ;
			for (j = 0 ; j < n ; j++)
				if (G2->Ma[i][j] == 1)
					sw = 1;
			if ( sw == 0)
				linie[i] = 1;
		}
		//in acest while marchez toate liniile si coloanele
		//cat timp nu mai am linii sau coloane de marcat
		while(1)
		{
			//dupa care marchez toate coloanele care au zerouri barate
			//intr o linie marcata
			for (i =  0 ;  i < n ; i++)
				for (j = 0  ; j < n ; j ++)
					if (G2->Ma[i][j] == -1 && linie[i] == 1 )
					{
						coloana[j] = 1;
					}
			sw = 0;
			//Marchez toate liniile care au un zero incadrat intr o 
			//coloana marcata
			for (i = 0  ; i < n ; i++)
				for (j = 0  ; j < n ; j++)
					if (G2->Ma[i][j] == 1 && coloana[j] == 1 && linie[i] == 0)
					{
						linie[i] = 1;
						sw ++;
					}
			//cand nu mai am ce linie sau coloana sa mai marchez 
			//parasesc si aceasta etapa de marcare a liniilor si a coloanelor		
			if(sw == 0)
				break;		
		}
		
		min = MAX_MAX;
		//gasesc minimul de pe liniile marcate si coloanele nemarcate
		for (i = 0 ; i < n ; i ++)
			for (j = n ;  j < 2 * n ; j++)
				if (G->Ma[i][j] < min && linie[i] == 1 && coloana[j-n] == 0)
					min  = G->Ma[i][j] ;
					
		//Scad minimul gasit de pe toate liniile care sunt marcate			
		for (i = 0  ; i < n ; i++)
			if (linie[i] == 1)
				for (j = n ; j < 2 * n ; j++)
					G->Ma[i][j] = G->Ma[i][j] - min;
		
		//Adun minimul pe coloanele marcate 
		for (j = n ; j < 2 * n ; j++)
			if (coloana[j-n] == 1)
				for (i = 0  ; i < n ; i ++)
					G->Ma[i][j] = G->Ma[i][j] + min;
	}
	//Odata ce am parasit functia de gasire a solutiei optime
	//avem stocata in stanga jos a matricei G si adunam 
	//suma drumurilor minime de la fiecare iepuras la vizuina sa
	int suma=0;
	for (i = 0  ; i < n ; i++)
		for (j = 0  ; j < n ; j++)
				if (G2->Ma[i][j] == 1)
					suma = suma + G->Ma[j+n][i] ;
	//returnam suma 
	return suma;
}

int solve(char *testInputFileName) {

	int i,j,N,n,temp;
	FILE *f;
	f = fopen(testInputFileName,"r");	
	fscanf(f,"%d",&N);
	TGraphM G1,G2;
	n = 2*N;
	//G1 este matricea de adiacenta
	alloc_matrix(&G1, n);
	//G2 este o matrice auxiliara necesara aplicarii algorimului lui Khun
	alloc_matrix(&G2, N);
	
 	for (i = 0 ; i < n/2 ; i++)
		for (j = n/2 ;  j < n ;j++)
		{
			//Citesc matricea de adiacenta din fisier
			//pe care o consider neorientata
			fscanf(f,"%d",&temp);
			G1.Ma[i][j] = temp;
			G1.Ma[j][i] = temp;
		
		}
	fclose(f);
	int min = MAX_MAX;
	for(i = 0  ; i < n/2 ; i++)
	{	//Gasesc minimul de pe fiecare linie
		min = MAX_MAX;	
		for (j = n/2  ;  j < n ; j++)
			if (G1.Ma[i][j] < min)
				min = G1.Ma[i][j];
		//Scad minimul gasit de pe linia respectiva
		for (j = n/2 ; j < n ; j++)
			G1.Ma[i][j] -= min;
  	}
  	
  	min = MAX_MAX;
	for(j = n/2  ; j < n ; j++)
	{
		//Gasesc minimul de pe fiecare coloana
		min = MAX_MAX;	
		for (i = 0  ;  i < n/2 ; i++)
			if (G1.Ma[i][j] < min)
				min = G1.Ma[i][j];
		//Scad minimul gasit de pe coloana respectiva
		for (i = 0 ; i < n/2 ; i++)
			G1.Ma[i][j] -= min;
  	}
	//Aplic functia de gasire a solutiei optime 
	int HAPPY = rezolva(&G1,&G2);
	free_matrix(&G1);
	free_matrix(&G2);
	//Returnez suma drumurilor minime
    return HAPPY;
}

#endif
