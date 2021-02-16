#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct link
{
	char *data;	
	struct link * next;
}typedef list;

list * add(char *input, list * current_address);
list * my_command(char * cmd , char * data , list * current_node);
void show(list * hand_node);
void del(char * data,list * node);

int main()
{
	list *init_node = (list *)malloc(sizeof(list));			
	list *current;
	char *cmd,*input;
	int count = 0;
	cmd = (char *)malloc(100);
	input = (char *)malloc(100);

	init_node->data = NULL;	
	init_node->next = NULL;
	current = init_node;

/*鍵入資料測試    
 *Note:要測試時要記得把分配給 input 空間那行給 mark 掉
	for(int i = 0 ; i < 2; i++)
	{
		input = (char *)malloc(100);	
		scanf("%s",input);	
		current = my_command("ADD",input,current);	
		show(init_node);	
		free(input);
	}
*/

/*新增刪除data測試
	current = my_command("ADD","test01",current);	
	current = my_command("ADD","test02",current);	

	del("test01",init_node);
	
	show(init_node);	
*/	
	
	return 0;
}

list * my_command(char * cmd , char * data , list * current_node)
{
	if(strncmp(cmd,"ADD",3) == 0)
	{
		return add(data,current_node);	
	}
}

//新增 node 和資料
list * add(char *input,list * current_node)
{
	list *new_node = (list *)malloc(sizeof(list));

	current_node->next = new_node;
	current_node->data = (char *)malloc(strlen(input));
	current_node->data = input;
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
			node_temp->data = "";
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








