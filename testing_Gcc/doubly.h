
#ifndef DOUBLYLINKED_LIST_H_
#define DOUBLYLINKED_LIST_H_


typedef struct Node_Type
{
Position cell;
struct Node_Type* Next;
struct Node_Type* Prev;
}Node;

void push(Node* Header , Node* Trailer ,Position data) ;  // push in front
int Doubly_Is_NOT_Empty(Node* Header,Node* Trailer);	
void Doubly_Remove(Node* v) ;
Position Pop(Node* Header,Node* Trailer);				  // get the first node and remove it



#endif 