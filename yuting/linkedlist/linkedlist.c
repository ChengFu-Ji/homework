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
Node *del(Node *first , char *data);
void save(Node *save , char *fname);
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
		else if(!strncmp(cmd,"add,",4)){
			one = add(one , cmd+4);
			printf("Command done!\n");
		}
		else if(!strncmp(cmd ,"del," ,4)){
			if(!del(one ,cmd+4)){
				one = del(one ,cmd+4);
			}
			printf("Command done!\n");
		}

		else if(!strncmp(cmd , "save," ,5)){
			save(one ,cmd+5);	
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

Node *del(Node *first , char *data){
	Node *before , *current;

	if(first == NULL){
		return NULL;
	}

	current = first;

	while(strcmp(current -> data ,data) != 0){
		before = current;
		current = current -> next;
	}

	if(before  -> next == NULL){
		printf("no data");
	}

	before -> next = current-> next;
	free(current);

	return first;
}

void save(Node *current , char *fname){

	Node *save;

	FILE *fp;

	//尚未將檔名的$以及\n換成\0來結束

	fp = fopen(fname , "w+");


	save = current;

	while(1){
	
		fwrite(save -> data , sizeof(save -> data), 1, fp);	
		if(save -> next == NULL){
			break;
		}
		save = save -> next;
	}
	

	fclose(fp);
 
}




