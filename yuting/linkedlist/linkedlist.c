#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*
 define node
 */
typedef struct node{
	char data[100];
	struct node *next;
} Node;

int main(){

	/*Node a,b,c;
	Node *p = &a;
	strcpy(a.data,"dddd");
	a.next=&b;
	strcpy(b.data,"ddddddd");
	b.next=&c;
	strcpy(c.data,"eeee");
	c.next=NULL;

	while(p != NULL){
		printf("address = %p ", p);
		printf("data =  %s ",p->data);
		printf("next= %p ",p->next);
		printf("\n");
		p=p->next;
	}*/

	

	Node *first , *current , *pervious;
		
	first = (Node *)malloc(sizeof(Node));

	current = first;

	while(1){
		
		if(first ->next != NULL){
			strcpy(first->data ,"fffff" );
		}
		
		
		Node *new;
		new = (Node *)malloc(sizeof(Node));

		current -> next = new;
		current = new;
		

		scanf("%s" , &new->data);	


		if(strcmp((new->data),"exit") == 0){
			printf("first data = %s\n" , first -> data);
			printf("first  address= %p\n" , first);
			break;
		}			
		
		

		printf("new ptr = %p\n" , current);
		printf("new data = %s\n" , current->data);

		printf("next = %p",current->next );
			 

	}

	
}
	
	
	


