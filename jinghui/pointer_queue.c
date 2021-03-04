/**佇列加入和刪除**/ 
#include<stdio.h> 
#include<stdlib.h> 

struct node //建立節點 
{
	int data;
	struct node *next;
};

struct node *ptr,*head,*current,*prev;
 

void add()
{
	//建立第一個 node //
	ptr = (struct node*)malloc(sizeof(struct node));
	gets(ptr->data);	//輸入 
	ptr->next = NULL;	//下一節點接null 
		
	if(ptr == NULL)
	{
		ptr = head; //指針為空值時指向head 
	}
	else 
	{
		current = ptr;
		while(current->next != NULL) 
		{
			current = current->next; 
		}
		current->next = head;
	}		
}


void list()
{
	int count = 0;

	//if(head->data == NULL)
	//{
	//	printf("無資料\n\n");
	//}
	//else
	//{
		printf("-------------列表---------------------\n\n");
		current=head;
		while(current != NULL)
		{
			printf("%d\n",current->data);
			current = current->next;
		}
		printf("---------------------------------\n");
	//}
}

void free_list()
{ 
	current = head;
	while(current != NULL) {
		prev = current;
		current = current->next;
		free(prev);
	}
} 

void del()
{
	char del_data[20];
	printf("刪除資料\n");
	gets(del_data);
	
	if(del_data == current->data) 
	{
		prev = current->next;
		free(current);
		return;
	}

	while (current != NULL)
	{
		if(current->next->data == del_data)
		{
			ptr = current->next;
			current->next = current->next->next;
			free(current);
			printf("%d已被刪除\n",del_data);
		}
		else
		{
			current = current->next;
			printf("找不到該資料\n");
		}	
	}
	getch();
} 

int main()
{
	int option;
	while(1)
	{
		printf("(1)新增資料 (2)刪除資料 (3)顯示 \n\n");
		option=getche();
		printf("\n");
		switch(option)
		{
			case'1':
				printf("輸入資料\n\n");
				add();
				break;
			case'2':
				del();
				break;
			case'3':
				list();
				break;
			default:
				printf("error\n");
				break;
		}
		
	}
	return 0;
} 
