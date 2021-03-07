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
void show(list * hand_node);
void del(char * data,list * node);
void file_save(list * hand_node , FILE * fp);
list * file_load(list * node,FILE * fp);

int main()
{
	list *init_node = (list *)malloc(sizeof(list));			
	list *current;
	char *cmd,*input;
	FILE * fp;

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
		else if (strncmp(cmd,"save",4) == 0)
		{
			file_save(init_node,fp);	
			printf("save done\n\n");	
			printf("This is test.txt context\n");
			system("cat test.txt");	
			printf("\n");	
			continue;
		}
		else if(strncmp(cmd,"show",4) == 0)	
		{
			show(init_node);
			continue;	
		}
		else if(strncmp(cmd,"load",4) == 0)
		{
			current = file_load(current,fp);	
				
			continue;	
		}

		printf("input data : ");
		scanf(" %[^\n]",input);

		if(strcmp(cmd,"ADD") != 0 && strcmp(cmd,"DEL") != 0)
		{
			continue;	
		}

		if(current->data == NULL)	
		{
			current->data = input;	
		}
		else		
		{
			current = my_command(cmd,input,current,init_node);
		}	
		show(init_node);
	}

	free(input);
	free(cmd);
	
	show(init_node);

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
	strcpy(current_node->next->data,input);
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
		if(strncmp(node_temp->data,data,strlen(data)) != 0)
		{	
			node_temp = node_temp->next;	
		}	
		else
		{
			node_temp->data = " ";	
			break;
		}
	}
}

//顯示 list 
void show(list * hand_node)
{
	list * node_temp;
	
	node_temp = hand_node;	

	while(1)
	{
		printf("current = %p current->data = %s current->next = %p\n",node_temp,node_temp->data,node_temp->next);	
		
		if(node_temp->next == NULL)	
		{
			break;	
		}
		node_temp = node_temp->next;	
	}
	printf("\n");
}

void file_save(list * hand_node,FILE * fp)
{
	list * node_temp;
	fp = fopen("test.txt","w+");
	
	node_temp = hand_node;
	
	while(1)
	{
		fwrite(node_temp->data,strlen(node_temp->data),1,fp);
		fwrite("\n",1,1,fp);	
		
		if(node_temp->next == NULL)	
		{
			break;	
		}
		node_temp = node_temp->next;
	}
	fseek(fp,1,SEEK_END);	
	fwrite("\0",1,1,fp);

	fclose(fp);
}

list * file_load(list * node , FILE * fp)
{
	fp = fopen("test.txt","r+");		
	char * data, * temp;
	int count = 0 , count_temp = 0;
	list * current_node;

	current_node = node;
	temp = (char *)malloc(128);

	//不管多少 data 先收再說
	fread(temp,101,1,fp);
	//因為在 fread() 時 fp 中的指向的位址也會跟者變動，所以需要導向檔案中的第一個 data 的 address
	fseek(fp,0,SEEK_SET);

	//load data
	while(1)
	{
		if(temp[count] == '\n')	
		{
			data = (char *)malloc(128);
			fread(data,count - count_temp,1,fp);	
			current_node = add(data,current_node);	
		
			count_temp = count;			
			printf("%s\n",data);
			free(data);	
		}
	
		if(temp[count] == '\0')
		{
			break;	
		}	
		count++;	
	}

	free(temp);
	fclose(fp);
	return current_node;
}
