#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stdlib.h>

#define MAX(a, b) (((a) >= (b))?(a):(b))

typedef struct node{
	void* elem;
	void* info;
	struct node *pt;
	struct node *lt;
	struct node *rt;
	struct node* next;
	struct node* prev;
	struct node* end;
	long height;
}TreeNode;

typedef struct TTree{
	TreeNode *root;
	TreeNode *nil;
	void* (*createElement)(void*);
	void (*destroyElement)(void*);
	void* (*createInfo)(void*);
	void (*destroyInfo)(void*);
	int (*compare)(void*, void*);
	long size;
}TTree;


TTree* createTree(void* (*createElement)(void*), void (*destroyElement)(void*),
		void* (*createInfo)(void*), void (*destroyInfo)(void*),
		int compare(void*, void*)){
	 //Aloc memorie pentru arbore 	
	 TTree *t = (TTree*) malloc (sizeof(TTree));
	 //Aloc moemrie pentru celula care va juca rolul de NULL
	 //spre care vor pointa prev al celui mai mic element
	 //si next-ul celui mai mare element din lista respectiv arbore
	 t->nil = (TreeNode*) calloc(1,sizeof(TreeNode));
	 //pentru inceput toate legaturile vor pointa catre acest t->nil
	 t->nil->lt = t->nil->rt=t->nil->pt = t->nil;
	 t->nil->next = t->nil->prev = t->nil;
	 t->root = t->nil;
	 t->root->lt = t->root->rt = t->root->pt = t->nil;
	 t->root->next = t->root->prev = t->root->end = t->nil;
	 //size-ul este initial 0
	 t->size = 0;
	 //Initializez functiile cu ajutorul carora creez elemente
	 //distrug elemente , compar elemente 
	 t->createElement  = createElement;
	 t->destroyElement  = destroyElement;
	 t->createInfo = createInfo;
	 t->destroyInfo = destroyInfo ;
	 t->compare = compare;	 
}

TreeNode* createTreeNode(TTree *tree, void* value, void* info){
	
	//Aloc memorie pentru nodul ce trebuie creat
	TreeNode *newNode = (TreeNode *) malloc(sizeof(TreeNode));
	//salvez in ele , elementul si informatia
	newNode->elem = tree->createElement(value);
	newNode->info = tree->createInfo(info);
	//initial pointeaza catre celula nil
	newNode->pt = newNode->lt = newNode->rt = tree->nil;
    //atat legaturile din arbore cat si legaturile din lista	
	newNode->prev = tree->nil;
	newNode->next = tree->nil;
	//de fievare data la inceput, end-ul va pointa catre sine
	newNode->end = newNode;
	//inaltimea este 1 
	newNode->height = 1;
	//returnez nodul proaspat creeat
	return newNode;
}

void destroyTreeNode(TTree *tree, TreeNode* node){

	 //eliberez elementul , eliberez informatia dupa care
	 //eliberez celula propriu-zisa
	 tree->destroyElement(node->elem);
	 tree->destroyInfo(node->info);
	 free(node);
}

int isEmpty(TTree* tree){
	
	//daca radacina pointeaza catre nodul nill
	//asta inseamna ca arborele estse gol
	if (tree->root == tree->nil)
		return 1;
	else
		return 0;
}

TreeNode* search(TTree* tree, TreeNode* x, void* elem){

	//cat timp nodul nu este nill si am ce sa compar
	//continui cautarea
    while (x != tree->nil && tree->compare(elem,x->elem))
   		//daca elem este mai mic atunci ma duc in stanga
   		if (tree->compare(elem,x->elem) == -1)
   			x = x->lt;
   		else //altfel ma duc sa caut in dreapta
   			x = x->rt;
   	//la final retunrez celula corespunzatoarea nodului dorit
   	//sau t->nil daca acesta nu a fost gasit
   	return x; 	
}

TreeNode* minimum(TTree*tree, TreeNode* x){
	
	//caut elementul in stanga , fara a permite acestuia sa fie nil
	while (x->lt != tree->nil)
		x = x->lt;
	//returnez elementul minim ( cel mai din stanga)
	return x;
}

TreeNode* maximum(TTree* tree, TreeNode* x){

	//elementul maxim este elementul cel mai din dreapta
	//asa ca ma duc cat de mult pot in dreapta
	while (x->rt != tree->nil)
		x = x->rt;
	//retunrez elementul maxim
	return x;
}

TreeNode* successor(TTree* tree, TreeNode* x){

	//daca are copil drept , succesorul este cel mai mic 
	//element din subarborele drept
	if (x->rt != tree->nil)
		return (minimum(tree,x->rt));
	//daca nu are copil drept incept sa caut cat de sus pot
	//cu conditia ca de fiecare data nodul aterior sa fie copilul
	//drept al nodului curent
	TreeNode *nod;
	nod = x->pt;
	while (nod != tree->nil && x == nod->rt)
	{
		//in x retin mereu copilul
		//care trebuie sa fie cel drept 
		x = nod;
		nod = nod->pt;
	}
	//returnez succesorul nodului x
	return nod;
}

TreeNode* predecessor(TTree* tree, TreeNode* x){

	//daca are subarbore stang, predecesorul este cel mai mare
	//element din subarborele stang
	if (x->lt != tree->nil)
		return (maximum(tree,x->lt));
	//altfel caut mereu intre parintii nodului pentru care trebuie
	//sa gasim predecesorul , dar cu o conditia ca nodul
	//anterior sa fie mereu copilul stang al nodului curent
	TreeNode *nod = x->pt;
	while (nod != tree->nil && x == nod->lt)
	{	
		//in x retin mereu copilul 
		x = nod;
		nod = nod->pt;	
	}
	//returnez predecesorul elementului dorit 
	return nod;
}

void avlRotateLeft(TTree* tree, TreeNode* x){
	TreeNode *y = x->rt;
	x->rt = y->lt;

	if(y->lt != tree->nil)
		y->lt->pt = x; // the parent of the y left subtree is x
	y->pt = x->pt; // update parent for y

	if(x->pt == tree->nil)
		tree->root = y;
	else if(x->pt->lt == x)
		x->pt->lt = y;
	else
		x->pt->rt = y;

	y->lt = x; // x is to the left of y
	x->pt = y; // x parent is y

	// Update heights
	y->height = MAX(y->lt->height, y->rt->height) + 1;
	x->height = MAX(x->lt->height, x->rt->height) + 1;

}

void avlRotateRight(TTree* tree, TreeNode* y){
	TreeNode *x = y->lt;

	y->lt = x->rt;
	if(x->rt != tree->nil)
		x->rt->pt = y;

	x->pt = y->pt;

	if(y->pt == tree->nil)
		tree->root = x;
	else if(y->pt->lt == y)
		y->pt->lt = x;
	else
		y->pt->rt = x;

	x->rt = y;
	y->pt = x;

	// Update heights
	y->height = MAX(y->lt->height, y->rt->height) + 1;
	x->height = MAX(x->lt->height, x->rt->height) + 1;
}



//functia auxiliara cu ajutorul careia actualizez inaltimea
void updateHeight(TTree *tree,TreeNode *y) {
   
    
	//daca este frunza atunci inaltimea este unu
	//si parasesc functia odata ce am acutalizat inaltimea
    if (y->lt == tree->nil && y->rt == tree->nil) 
    {
      y->height = 1;
      return;
    }
	//daca am doi copii nevizi iau inaltimea cea mai mare
    if(y->lt != tree->nil && y->rt != tree->nil)
    	if ( y->lt->height > y->rt->height)
    		y->height = y->lt->height+1;
    	else
    		y->height = y->rt->height+1;
    else
    	//altfel daca unul dintre ei pointeaza catre celula nil , 
    	//iau inaltimea copilului nevid + 1  
    	if (y->lt == tree->nil)
    		y->height = y->rt->height+1;
    	else
    		y->height = y->lt->height+1;
}


/* Get AVL balance factor for node x */
int avlGetBalance(TTree* tree, TreeNode *x){
	//daca nodul este nil returnez 0
	if (x == tree->nil) 
  		return 0;
  //daca nodul este frunza atunci este echilibrat deci returnez 0
  	if (x->lt == tree->nil && x->rt == tree->nil)
    	return 0;
  
	//daca se poate fac diferenta dintre inaltimile fiilor
	//in cazul in care ambii sunt nevizi
  	if(x->lt != tree->nil && x->rt != tree->nil)
    	return (x->lt->height - x->rt->height);
    else
    	//altfel daca unul dintre ei pointeaza catre celula nil , 
    	//fac diferenta 0-inaltimea copilului drept
    	if (x->lt == tree->nil)
    		return (0 - x->rt->height);
    	else //sau copilul stang - 0
    		return (x->lt->height);
}

void avlFixUp(TTree* tree, TreeNode* y){ 
   //aceasta functie are rolul de a fixa legaturile atat 
   //din arbore de al echilibra cat si legaturile dintre 
   //elementele listei suprapuse peste arbore
	while ( y != tree->nil)
	{	
		//actualizez inaltimea lui y la fiecare fixUp
		updateHeight(tree,y);
		//pentru fiecare nod , endu-l va pointa spre succesor
		//iar prev-ul va pointa spre endul predecesorului
		y->prev = predecessor(tree,y)->end;
		y->end->next = successor(tree,y);
		//daca arborele este dezechilibrat pe dreapta
		//atunci va trebui sa fac o rotatie la stanga
		if (avlGetBalance(tree,y) < -1)
		{	
				//rotatia la stanga poate fi dubla in cazul
				//in care nepotul interior este mai mare
				//decat nepotul exterior
				if (avlGetBalance(tree,y->rt) > 0)
				{
					avlRotateRight(tree,y->rt);
					avlRotateLeft(tree,y);
				}		
				else //sau rotatie simpla daca nepotul exterior este
					//mai mare decat nepotul interior
					avlRotateLeft(tree,y);
		}
		else
			//altfel daca este dezechilibrat la stanga
			//va trebuie sa facem o rotatie la dreapta
			if (avlGetBalance(tree,y) > 1)
			{
				//de asemenea aceasta poate fi dubla daca
				//nepotul interior este mai mic decat nepotul exterior
				//ca inaltime
				if (avlGetBalance(tree,y->lt) < 0)
				{
					avlRotateLeft(tree,y->lt);
					avlRotateRight(tree,y);
				}		
				else
					//sau simpla daca inaltimea nepotului stanga este 
					//mai mare 
					avlRotateRight(tree,y);
			}
		//de fiecare data avansez 
		y=y->pt;	
	}
	
	TreeNode *nod;
	//actualizez legaturile dintre maxim 
	//end-ul maximului trebuie sa pointeze la nill
   	nod = maximum(tree,tree->root);
   	if (nod == nod->end)
   		nod->next = tree->nil;
   	else
   		nod->end->next = tree->nil;
   	//de asemenea trebuie
   	//actualizate si legaturile nodului de la inceputul listei
   	//adica nodul minim
   	nod->prev = predecessor(tree,nod);
   	nod = minimum(tree,tree->root);
   	nod->end->next = successor(tree,nod);
   	nod->prev = tree->nil;	
}

void insert(TTree* tree, void* elem, void* info) {
	
	int sw = 0;
	//daca arborele este gol atunci returnam in radacina
	//primul nod
	if (isEmpty(tree) == 1)
	{
		tree->root = createTreeNode(tree,elem,info);		
	}
	//altfel incepem sa cautam nodul unde urmeaza sa fie
	//adaugat elementul elem
	else
	{
		//cu nod ne vom plimba prin arbore iar in newnod
		//vom retine nodul ce urmeaza a fi adaugat
		TreeNode *nod,*newnod;
		nod = tree->root ;
		newnod = createTreeNode (tree,elem,info);
		
		//cat timp nu am ajuns la final 
		while ( nod != tree->nil)
		{
			//mereu memoram parintele nodului nodului ce 
			//urmeaza a fi adaugat , deoarece nod va fi nul
			//si nu stim unde trebuie sa il adaugam
			newnod->pt = nod;
			//astfel daca elementul de adaugat 
			//este mai mic , mergem in stanga
			if (tree->compare(nod->elem,elem) == 1 )
			{
				nod = nod->lt;
			}
			else
			//daca elementul de adaugat este mai mare mergem
			//in dreapta
				if (tree->compare(nod->elem,elem) == -1)
					{
						nod = nod->rt;	
					}
				else
				{
				//altfel daca sunt egale 
					// cu ajutorul lui sw retiem faptul ca
					//elementul dej exista
					sw = 1;
					//nodul nostru va pointe catre succesorul
					//sau din arbore
					//iar succesorul lui nod va pointa
					//catre noul nod creaat
					newnod->next = successor(tree,nod);
					successor(tree,nod)->prev = newnod;
					//endul nod-ului va fi acum newnod
					//de asemenea refacem legaturile dintre
					//elementele arborelui
					nod->end->next = newnod;
					nod->end->next->prev = newnod;
					nod->end->next = newnod;
					newnod->prev = nod->end;
					nod->end = newnod;
					//de asemenea parasim while-ul
					break;
				}		
		}
		tree->size = tree->size+1;
		//daca nodul nou create este mai mic decat 
		//elementul parinte atunci acesta va fi nodul stanga
		if (tree->compare(newnod->elem,newnod->pt->elem) == -1 && sw == 0)
		{
			//actualizam legaturile dintre tata si fiu
			//si vedem daca arborele trebuie reechilibrat
			newnod->pt->lt = newnod;
			avlFixUp(tree,newnod);	
		}
		else
			//altfel acesta va fi elementul drept
			if (tree->compare(newnod->elem,newnod->pt->elem) == 1 && sw == 0)
			{
				//actualizam legatura dintre tata si fiu 
				//si vedem daca arborele trebuie reechilibrat
				newnod->pt->rt = newnod;
				avlFixUp(tree,newnod);			
			}	
			else
				avlFixUp(tree,nod);
	}	
}

void delete(TTree* tree, void* elem){

	TreeNode *a;
	TreeNode *b;
	TreeNode *c;
	 //in c salvez elementul pe care vreau sa il elimin
	c = search(tree, tree->root,elem);
	 //daca nu il gassc ies din functie 
	if (c == tree->nil)
	return ;
	//altfel verific daca acesta are mai multe copii 
	//sau este singur , cu alte cuvinte daca sterg din arbore
	//sau sterg din lista
	if (c->end  == c)
	{
		//daca elementul pe care vreau sa il sterg
		//are un singur copil 
		//atunci salvez in b adresa lui c altfel
		//daca c are doi copii salvez in c succesorul lui c
		if ((c->lt == tree->nil) || (c->rt == tree->nil)) 
			b = c;
		else
			b = successor(tree,c);
	 
		//in a salvam adresa copilului newvid al lui b 
		//verificam daca copilul stang exista daca da atunci salvam
		//adresa lui altfel salvam adresa copilului drept
		//care poate fi sau nu tree->nil
		if (b->lt != tree->nil) 
			a = b->lt;
		else
			a = b->rt;
 	
		//daca a este diferit ii vom schimba pentru inceput parintele
		//cu parintele lui b
		if(a != tree->nil)
			a->pt = b->pt;
		//daca b este radacina 
		//schimbam adresa radacinii
		if(b->pt == tree->nil) 
			tree->root = a;
		else 
		//altfel daca b este copilul stanga
	    //modificam copilul stanga al parintelui lui b
	 	if( b == b->pt->lt) 
			b->pt->lt = a;
		//altfel modificam copilul drept al parintelui lui a
		else	
			b->pt->rt = a;
	
		//sw ul intial e 0 presupunem ca nodul nostru
		//de sters nu avea doi copii
		int sw=0;
		//daca b este diferit de c asta inseamna ca 
		//c are doi copii si copii
	 	if (b != c)
		{
	 		//marcam faptul ca c a avut doi copii
	 		//initializam sw cu 1
			sw=1;
			//distrugem elementele lui c ( de pe heap )
       		tree->destroyElement(c->elem);
    		tree->destroyInfo(c->info);   	
    		//si adaugam in locul lor elementele lui b 
    		//cu alte cuvinte nu schimbam decat valorile din c
    		//nu ccelula propriuzisa de pe heap si leagturile ei
			c->info = b->info;
			c->elem = b->elem; 		
		}

		//b este nodul ce trebuie sa fie sters 
		//ii salvam adresa pentru a-l sterge dupa ce facem fixup-ul
	  	TreeNode* aux = b;
	  	//urcam cu o pozitie mai sus , pentru a incepe fixup-ul
		b = b->pt;
	 	//daca a fost facuta o inserare dubla , elemetele lui c au fost
	 	//deja sterse, asa ca stergem doar celule corespunzatoare nodului
		if (sw ==1)
			free(aux);
		//altfel trebuie sa stergem atat elementele cat si 
		//informatia pe langa celule propriuzisa
		else
		{
			tree->destroyElement(aux->elem);
    		tree->destroyInfo(aux->info);
			free(aux);
		}
 	
		//reechilibram arborele si legaturile 
		//dintre nodurile listei dublu inlantuite
		avlFixUp(tree,b);	
	}
	else
	{
		//altfel daca nodul de sters , nu este unic
		//si are duplicate , ce sunt retinute cu ajutorul
		//listei dublu inlantuite
		TreeNode *aux;
		//daca nodul de sters nu este maximul
		if ( c != maximum(tree,tree->root) )
		{
			//atunci celula care trebuie stearsa
			//este ultima si o memoram in aux
			aux = c->end;
			//refacem legaturile dintre elementele listei
			c->end->next->prev = c->end->prev;
			c->end->prev->next = c->end->next;
			//end ul lui c va fi acum predecesorul endului 
			//ce rumeaza a fi sters
			c->end = c->end->prev;
		 	//eliberam memoria pentru element
		 	tree->destroyElement(aux->elem);
		 	//eliberam memoria pentru informatie
		 	tree->destroyInfo(aux->info);
		 	//eliberam celula pentru nod
			free(aux);
			return;
		}
		//altfel daca nodul de sters este maximul
		if(c == maximum(tree,tree->root))
		{
			//atunci celula care trebuie stearsa , este ultima
			//cea care trebuie stearsa , asa ca o memoram in aux
			aux = c->end;
			//refacem legaturile dinter elementele listei dublu inalnuite
			c->end = c->end->prev;
			//face, ca ultimul element al maximului , end-ul maixmului
			//sa pointeze catre celula nil
			c->end->next = tree->nil;	
			//dupa care eliberam memoria pentru informatie elemente
			//si celula propriuzisa
 			tree->destroyInfo(aux->info);
 			tree->destroyElement(aux->elem);
			free(aux);
		}	
	}
}

void destroyTree(TTree* tree){

	//cat timp mai avem elemente de sters , stergem mereu radacina 
	//cu ajutorul functiei de stergere
	while(tree->root != tree->nil)
   		delete(tree, tree->root->elem);
	//odata sterse toate elementele si eliberata toata memoria
	//tot ce avem de facut este sa eliberam memoria
	//celulei nil si memoria alocata pentru arbore
	free(tree->nil);
	free(tree);
}


#endif /* AVLTREE_H_ */
