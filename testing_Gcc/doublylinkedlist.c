
#include<stdio.h>
#include <stdlib.h>
#include "std_struct.h"
#include "doubly.h"




void push(Node* Header , Node* Trailer ,Position data)   
{
	Node* cur = Header->Next ;
	Node* u = (Node*)malloc(sizeof(Node)) ;
	u->cell = data ;
	u->Next = cur ;
	u->Prev = cur->Prev ;
	cur->Prev->Next = u ;
	cur->Prev = u ;	
}


int Doubly_Is_NOT_Empty(Node* Header,Node* Trailer)
{
	return(Header->Next != Trailer ) ;
}

void Doubly_Remove(Node* v)
{
	Node* u = v->Prev ;
	Node* w = v->Next ;
	u->Next = w ;
	w->Prev = u ;
	
	free(v) ;
}




Position Pop(Node* Header,Node* Trailer) //Get_From_Trailer 
{
	Position data  = {0} ;
	if (Doubly_Is_NOT_Empty(Header, Trailer))
	{
		Node* cur = Trailer->Prev;
		data = cur->cell ;
		Doubly_Remove(Trailer->Prev) ;
	}

	return data ;
}




/*

void main(void)
{
	Node* Header = (Node*)malloc(sizeof(Node)) ;
	Node* Trailer =(Node*)malloc(sizeof(Node)) ;
	
	Header->Next = Trailer ;
	Trailer->Prev = Header ;
	
	
	int flag = 1 ;
	int data = 0 ;
	while(flag)
	{
		printf("Please Enter The Number : ");
		scanf("%d", &data);
		push(Header,Trailer,data);
		printf("Do You Want To Contiune 1:yes 0:no ");
		scanf("%d", &flag);

	}
	
	printf("The LinkedList Contains : \n");
	printf("From HEADER : ");
	Display_From_Header(Header,Trailer);
	printf("\nFrom TRAILER : ");
	Display_From_Trailer(Header,Trailer);
	
	printf("\n	Min value %d : ",get_min(Header,Trailer));
	
	
}
*/
