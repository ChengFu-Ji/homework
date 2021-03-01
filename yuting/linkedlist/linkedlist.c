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

Node *add(Node *first , char *data);
void show(Node *one);


int main(){

	Node *one , *current , *next;
		
	one = (Node *)malloc(sizeof(Node));

	one -> next = NULL;

	char *cmd;
	char spc[103];
	cmd = spc;

	printf("Welcome\n");
	printf("<CMD , DATA>\n");

	while(1){

		printf(">>");
		
		fgets(cmd,103,stdin);
		if(!strncmp(cmd , "exit" ,4)){
			break;
		}
		else if(!strncmp(cmd,"add",3)){
			one = add(one , cmd+4);
			printf("Command done!\n");
		}
		else if(!strncmp(cmd , "show" ,4)){
		
			show(one);
		}
		else{
			printf("error");
		}
	}
 	
	
	return 0;
	
}

void show(Node *one){
	Node *current;

	current = one;
	
	
	while(current != NULL){
		printf("current: [%p], data: [%s], next: [%p]\n", current, current->data, current->next);
		current = current -> next;
	}


}
	
Node  *add(Node *first , char *data){
	Node *new , *current;	

	if(first == NULL){
		return NULL;
	}

	current = first;

	while(current -> next != NULL){
		current = current -> next;
	}

	new = (Node *)malloc(sizeof(Node));
	
	strcpy(new -> data , data);
	new -> next = NULL;
	current ->next = new;
	
	return first;
}	




