#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct link
{
	char *data;	
	struct link * next;
}typedef list;

list * add(char *input, list * current_address);
list * my_command(char * cmd , char * data , list * current_node,list * hand_node);
void show(list * hand_node,FILE * file);
void del(char * data,list * node);

int main()
{
	list *init_node = (list *)malloc(sizeof(list));			
	list *current;
	char *cmd,*input;
	FILE * file  = fopen("test.txt","r+");
	FILE * file2 = fopen("test.txt","r");

	init_node->data = (char *)malloc(128);
	init_node->data = NULL;
	init_node->next = NULL;
	current		 	= init_node;
	
	while(1)	
	{
		cmd   = (char *)malloc(128);
		input = (char *)malloc(128);
		
		printf("input cmd : ");	
		scanf("%s",cmd);
		
		//離開迴圈
		if(strncmp(cmd,"exit",4) == 0)
		{
			printf("bye~~\n");
			break;	
		}

		printf("input data : ");
		scanf(" %s",input);

		if(strcmp(cmd,"ADD") != 0 && strcmp(cmd,"DEL") != 0)
		{
			continue;	
		}

		if(current->data == NULL )	
		{
			current->data = input;	
		}
		else		
		{
			current = my_command(cmd,input,current,init_node);
		}	
		show(init_node,file);
	}

	
	fclose(file);
	
	char ch[128] = {'\0'};
	
	int count = 0;
	fgets(ch,127,file2);

	
	while(1)
	{
		
		printf("%c",ch[count]);		
			
		if(count == 128)
		{
			break;
		}
		count++;	
	}	

	free(cmd);	
	free(input);	
	fclose(file2);


	my_command("ADD",ch,current,init_node);
	show(init_node,file2);







	return 0;
}

//指令判斷的function
list * my_command(char * cmd , char * data , list * current_node,list * hand_node)
{
	if(strncmp(cmd,"ADD",3) == 0)
	{
		return add(data,current_node);	
	}
	else if(strncmp(cmd,"DEL",3) == 0)
	{
		del(data,hand_node);	
		return current_node;	
	}
	else
	{
		return current_node;	
	}
}

//新增 node 和資料
list * add(char *input,list * current_node)
{
	list *new_node = (list *)malloc(sizeof(list));

	current_node->next		 = new_node;
	current_node->next->data = (char *)malloc(strlen(input));
	current_node->next->data = input;
	current_node->next->next = NULL;

	return current_node->next;
}

//刪除資料
void del(char * data, list * node)
{
	//想法一 從頭找資料刪除
	list * node_temp;	
	node_temp = node;

	while(1)
	{
		if(strcmp(node->data,data) != 0)
		{	
			node_temp = node_temp->next;	
		}	
		else
		{
			node_temp->data = NULL;
			break;
		}
	}
}

//顯示 list 
void show(list * hand_node,FILE * file)
{
	list * node_temp;
	
	node_temp = hand_node;	

	while(1)
	{
		printf("current = %p current->data = %s current->next = %p\n",node_temp,node_temp->data,node_temp->next);	
		fprintf(file,"current = %p current->data = %s current->next = %p\n",node_temp,node_temp->data,node_temp->next);	
		
		if(node_temp->next == NULL)	
		{
			break;	
		}
		node_temp = node_temp->next;	
	}
	printf("\n");
}





