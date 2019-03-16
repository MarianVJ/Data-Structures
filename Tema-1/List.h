#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct ListNode {
    struct ListNode *urmator;
    struct ListNode *anterior;
    int prioritate;
    char *nume;
} ListNode;

typedef struct List {
    ListNode *primul;
    ListNode *ultimul;
} List;

void adauga_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu);

void inchide_ghisee(List ***ghisee);

int Nglob = 0; //Variabila globala in care retin numarul de 
				//ghisee care sunt deschise
void deschide_ghisee(List ***ghisee, int N) {
	
	int i;
	if (Nglob == 0)   // verifica daca au mai fost deschise ghisee
	{             	//inainte cu ajutorul variabilei globale
		*ghisee = (List**)malloc(N*sizeof(List*)); //aloc memorie pentru 
		Nglob = N;									//vectorul de cozi 
		if ((*ghisee) == NULL) //verific daca alocarea a esuat
		{
			printf("Nu s-a alocat suficienta memorie");
			return;
		}
		for (i = 0 ; i < N;i++)
		{				//Aloc memorie pentru fiecare element al vectorului de
			(*ghisee)[i]=(List*)malloc(sizeof(List)); //cozi (pt fiecare coada)
			if ( (*ghisee)[i] == NULL) //verific daca esuare a esuat 
			{
				printf("NU s-a alocat suficienta memorie");
				return ;
			}
			(*ghisee)[i]->primul = NULL; //initializez cei doi pointeri 
			(*ghisee)[i]->ultimul = NULL; //la NULL
		}
	return ; // pentru a iesi automat din functie dupa ce deschiderea
	}        //ghiseelor are loc
	else //Acest else este pentru cazul in care deja sunt deschise 
	{		//un anumit numar de ghisee ( salvat in variabila Nglob
		List **ghiseu;    // este un vector de de ghisee , cu un singur
		ghiseu = (List**) malloc (sizeof(List*)); //ghiseu
		ghiseu[0] = (List*) malloc (sizeof(List));//pe care il aloc
		ghiseu[0]->primul = NULL;       			//dinamic
		ghiseu[0]->ultimul = NULL;
		ListNode *nod; //pointer catre o lista cu ajutorul caruia voi
						//adauga elementele in ghiseu[0]
		for (i = 0 ; i < Nglob ; i++)
		{	// Parcur ghiseele vechi ( pe toate)
			nod = (*ghisee)[i]->ultimul; 
			while (nod != NULL)
			{	//Adaug in vectorul ghiseu[0] toate elementele in ordine
				//crescatoare , cu ajutorul functiei adauga_ghiseu						
				adauga_persoana(&ghiseu, nod->nume, nod->prioritate, 0);
				nod = nod->anterior;	//trec la urmatorul element	
			}
		}
		
		ListNode *temp;  // eliberez toate memoria alocata pentru
		for ( i = 0 ; i < Nglob ; i++) //fostul vectorul de ghisee
		{			//elementele sunt stocate in ghiseu[0]
			nod = (*ghisee)[i]->primul;
			while (nod != NULL)
			{
				temp = nod;
				nod = nod->urmator;
				free(temp->nume);
				free(temp);
			}
			free((*ghisee)[i]);		
		}
		free((*ghisee));
		//Aloc din nou memorie pentru noul meu vector de ghisee
		//in mod similar ca atunci cand nu aveam niciun ghiseu deschis	
		*ghisee = (List**)malloc(N*sizeof(List*));
		int i;
		Nglob = N;
		if ((*ghisee) == NULL)
		{
			printf("Nu s-a alocat suficienta memorie");
			return;
		}
		for (i = 0 ; i < N;i++)
		{
			(*ghisee)[i]=(List*)malloc(sizeof(List));
			if ( (*ghisee)[i] == NULL)
			{
				printf("NU s-a alocat suficienta memorie");
				return ;
			}
			(*ghisee)[i]->primul = NULL;
			(*ghisee)[i]->ultimul = NULL;
		}
		//Odata alocata memorie pentru noul vector de ghisee
		//introduc pe rand in ordinea ceruta elementele
		//care sunt salvate in vectorul cu un singur element
		//ghiseu[0]
		nod = ghiseu[0]->primul; //pointer cu ajutorul caruia parcurg toate 
		int nr=0;  //elementele , iar nr este o variabila cu ajutorul
		while (nod != NULL) //careia pun in noile cozi elementele 
		{					//pe pozitiile potrivite
			//Adaug persoanele la cozile corespunzatoare cu ajutorul
			//functiei adauga persoane
			adauga_persoana(ghisee, nod->nume, nod->prioritate,nr%N);
			nr++;
			nod = nod->urmator;
		}
		 Nglob = 1;
		 inchide_ghisee(&ghiseu); //elibere memoria vectorului ghiseu
		 Nglob = N;	//salvez noul numar de cozi in variabila globala				 
	}	
}

void inchide_ghisee(List ***ghisee) {
	
	ListNode *nod,*temp; //pointeri auxiliari cu ajutorul carora parcurg
	int i;					// si eliberez memoria
	for ( i = 0 ; i < Nglob ; i++)
	{
		nod = (*ghisee)[i]->primul; //pointeru cu care parcurg fiecare coada
		while (nod != NULL)
		{
			temp = nod; //salvez in temp adresa pentru a elibera memoria
			nod = nod->urmator;
			free(temp->nume); //eliberez memoria pentru nume
			free(temp);     //eliberez celula pentru elementul respectiv
		}
		free((*ghisee)[i]);		//eliberez ghiseul
	}
	Nglob = 0; //noul numar de cozi este 0 ( nu mai am ghisee deschise)
	free((*ghisee)); //eliberez memoria alocata pentru vectorul de ghisee
	(*ghisee)=NULL; //initializez cu NULL 
}

void adauga_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {
		
	if ((*ghisee) == NULL) //verific daca nu sunt alocate ghisee
	{
		printf("Nu exista ghisee");
		return;
	}
	
	ListNode *nod,*temp,*aux;
	nod = (ListNode*) malloc (sizeof(ListNode)); //aloca memorie
	nod->urmator = nod->anterior = NULL; //pentru nodul ce urmeaza
	nod->prioritate = prioritate; // sa fie adaugat
	nod->nume = (char*) malloc ((strlen(nume)+1)*sizeof(char));
	int i;
	
	for (i = 0 ; i < strlen(nume) ;i++)
	{
		nod->nume[i] = nume[i]; //salvez numele 
	}
	nod->nume[i] = '\0'; // adaug la final NULL terminatorul
	//cazul in care ghiseul in care trebuie sa adaug
	//nu are niciun element
	if ( (*ghisee)[numar_ghiseu]->primul == NULL && 
		 (*ghisee)[numar_ghiseu]->ultimul == NULL )
	{	// atat primul cat si ultimul pointeaza catre acelasi element 
		(*ghisee)[numar_ghiseu]->primul = nod;
		(*ghisee)[numar_ghiseu]->ultimul = nod;
		return ;
	}
	else
	{	//pointer catre elementul cu cea mai mare prioritate din ghiseul curent
		temp = (*ghisee)[numar_ghiseu]->primul;
		//verific daca elementului care trebuie adaugat
		//ii corespunde prima pozitie  din ghiseu
		if ( (nod->prioritate > temp->prioritate) ||
			 ((nod->prioritate == temp->prioritate && 
				strcmp(nod->nume , temp->nume) < 0)) )
		{	//refac legaturile dintre elemente si pointez
			//catre primul element ( cel proaspat adauga)
			(*ghisee)[numar_ghiseu]->primul = nod;
			temp->anterior = nod;
			nod->urmator = temp;		
			return ;
		}
			// in cazul in care nu am gasit elementul 
			//cautam exact pozitia in care trebuie sa il adaugam
				
		while ( (temp != NULL) && ( (nod->prioritate < temp->prioritate) ||
          	    (nod->prioritate == temp->prioritate && 
			   	strcmp(nod->nume,temp->nume) > 0) ))	
				{	// in aux retin mereu penultimul element			
					aux = temp;
					temp = temp->urmator;
				}
				
		//verific daca elementul pe care trebuie sa il adaug
		//trebuie sa fie ultimul sau in mijlocul ghiseului
		if (temp != NULL)
		{  //trebie adaugat in interiorul ghiseului 
			nod->urmator = temp;			
			nod->anterior =temp->anterior;
			temp->anterior->urmator = nod;
			temp->anterior = nod;		
			return;
		}
		else
		{	// trebuie adaugat la finalul ghiseului
			nod->anterior = aux; 
			(*ghisee)[numar_ghiseu]->ultimul->urmator = nod;
			(*ghisee)[numar_ghiseu]->ultimul = nod;
			return ;
		}
				
	}
}

void sterge_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {
		
	ListNode *nod;
	if (numar_ghiseu < Nglob ) //verific daca exista ghiseul din care
	{							//doresc sa sterg
		nod = (*ghisee)[numar_ghiseu]->primul;
		if (nod == NULL) // verific daca exista elemente
		{	//pe care as putea sa le sterg, in caz contrar, parasesc functia
			
			return ;
		} // cu ajutorul urmatorului while parcurg toate
	  //elementele din ghiseul curent
		while (nod != NULL)
		{	//daca il gasesc il sterg in functie de cazul
			// in care se afla ( primul ,ultimul sau la mijloc)
			if (nod->prioritate == prioritate && 
				strcmp(nod->nume,nume) == 0)
			{	// verific daca este primul 
				if (nod->anterior == NULL)
				{	//modific pozitia primului element din ghiseu
					(*ghisee)[numar_ghiseu]->primul = nod->urmator;
					if (nod->urmator == NULL) //verific daca lista mai are
					{ //elemente
						(*ghisee)[numar_ghiseu]->ultimul = NULL;
						free(nod->nume);
						free(nod);
						return; //aici ies din functie
					} // in cazul in care nu am iesit din functie
					//asta inseamna ca mai sunt elementele si refac 
					//legaturile dintre liste corespunzator
					nod->urmator->anterior = NULL;
					free(nod->nume);
					free(nod);
					return; //si parasesc functia
				}
				//cazul in care nu este primul element
				if (nod->anterior != NULL)
				{	//verific daca este ultimul
					if (nod->urmator == NULL)
					{
						(*ghisee)[numar_ghiseu]->ultimul = nod->anterior;		
						nod->anterior->urmator = NULL;
						free(nod->nume);
						free(nod);
						return;			
					}
					else	
					{	//in acest caz elementul de eleminiat este in interiorul
						//ghiseului
						nod->urmator->anterior = nod->anterior;
						nod->anterior->urmator = nod->urmator;
						free(nod->nume);
						free(nod);
						return;
					}
				}	
			
			}
			nod = nod->urmator;
		}
	}
}

void sterge_primii_oameni(List ***ghisee) {
	int i;
	ListNode *nod;
	//parcurg toate ghiseele cu ajutorul variabilei globale
	for (i = 0 ; i < Nglob ; i++)
	{	// verific daca exista element de sters 
		if ((*ghisee)[i]->primul != NULL)
		{
			nod = (*ghisee)[i]->primul;
			
			if (nod->urmator != NULL)
				(*ghisee)[i]->primul->urmator->anterior = NULL;		
			
			(*ghisee)[i]->primul = nod->urmator;
			free(nod->nume);
			free(nod);
		}
	}
}

int calculeaza_numar_oameni_ghiseu(List **ghisee, int numar_ghiseu) {
	//initializez contor de la 0
	int count = 0;
	ListNode *nod;
	if (Nglob > 0 && numar_ghiseu < Nglob)
	{	//pointez catre primul element din ghiseu
		nod = ghisee[numar_ghiseu]->primul;
		while (nod != NULL)
		{
			count++; //numar atat timp cat am elemente
			nod = nod->urmator;
		}
	}
	return count; //la final returnez numarul de omaeni dni ghiseu
}

int calculeaza_numar_total_oameni(List **ghisee) {
	//initializez un contor de la 0
	int count = 0;
	ListNode *nod;
	int i;
	//parcurg cu un for toate ghiseele deschise 
	for (i = 0 ; i < Nglob; i++)
	{	//cu nod parcurg fiecare ghiseu
		nod = ghisee[i]->primul;
		while (nod != NULL)
		{	//atat timp cat exista oameni in ghiseul curent
			//ii numar
			count++;
			nod = nod->urmator;
		}
	}
	//la final returnez numarul total de oameni
	return count;
}

int gaseste_pozitie_persoana (List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {
	//prima pozitie poate fi 0
	int poz = 0; 
	if ( numar_ghiseu < Nglob) //verific daca ghiseul dorit exista
	{	//cu ajutorul lui nod parcurg toti oamenii din ghiseu
		ListNode *nod = ghisee[numar_ghiseu]->primul;	
		while (nod != NULL)
		{	//verific la fiecare pas daca am gasit persoana
			if (strcmp(nod->nume,nume)==0 && nod->prioritate == prioritate)
				return poz; // in caz afirmativ returnez pozitia
			nod = nod->urmator; // trec la persoana urmatoare
			poz++; //incrementez pozitia atat timp cat nu am gasit nimic
		}
	}	//in final returnez -1 daca persoana nu a fost gasita
	return -1;
}

char* gaseste_persoana_din_fata(List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu) {
	
	ListNode *nod;
	if ( numar_ghiseu < Nglob )
	{
		nod = ghisee[numar_ghiseu]->primul;
		while (nod != NULL)
		{	//verific daca persoana este cea cautata
			if (strcmp(nod->nume,nume)==0 && nod->prioritate == prioritate)
			{	//daca da verici daca are persoana in fata
				if (nod->anterior != NULL)
				{	// daca are persoana in fata returnez numele acesteia
					return nod->anterior->nume;
				}
			}//trec la elementul urmator
			nod = nod->urmator;
		} // in final returnez "nu exista" daca nu s a gasit
		return "Nu exista.";
	}	
}

void afiseaza_rand_ghiseu(FILE *f, List **ghisee, int numar_ghiseu) {
	
	ListNode *nod; // verific daca exista ghisee si daca
	//numarul ghiseului cerut este valid
	if (Nglob  > 0 && numar_ghiseu < Nglob)
	{	//parcurg toate elementele din ghiseu
		nod = ghisee[numar_ghiseu]->primul;
		if (nod != NULL)
		{	
			while (nod != NULL)
			{	//cat timp am elemente le afisez
				fprintf(f, "%s-%d;", nod->nume, nod->prioritate);
				nod = nod->urmator;
			}
			fprintf (f, "\n");
		}
		else
		{	// in cazul in care nodul este nul afisez urmatorul
			//mesaj
			fprintf(f,"Ghiseul %d nu are nicio persoana in asteptare.\n",
					 numar_ghiseu);
		}
	}
}

#endif
