/**單向LinkList**/ 
#include<stdio.h> 
#include<stdlib.h> 

struct node 
{
  int data;
  struct node *next;
};

struct node *ptr,*head,*current,*prev,*tmp;
 
void add()
{
	char change[4];						 //因gets只接受char型態 
	ptr = (struct node*)malloc(sizeof(struct node)); 
	gets(change);
	ptr->data = atoi(change);				 //atoi()是將字串轉成int型態 
	ptr->next = NULL;
	if(head == NULL)
	{	
	  head=ptr;					 	 //設第一筆資料為head
	}
	else
	{
	  current = head; 					//從頭開始 
	  while(current->next != NULL)				//如果當前狀態為不為最後一筆 
	  {
		 current = current->next;			//指向下一筆
	  }
	  current->next = ptr; 					//等於新增資料
	}
}


void list()
{
	printf("-------------列表---------------------\n\n");
	if(head == NULL)
	{
	  printf("無資料\n");
	}
	else
	{
	  current=head;
	  while(current != NULL)
	  {
	    printf("%d\n",current->data);
	    current = current->next;
	  }
	}
	printf("---------------------------------\n");
}

//釋放整個鏈結空間 
//此函式有個問題是head無法free掉 
void free_list()
{
	if(head == NULL)
	{
	  printf("無資料\n");	
	}
	else
	{
	  current=head;
	  while(current != NULL) 
	  {
	  tmp = current;
	  current = current->next;
	  free(tmp);
	  }
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
		printf("(1)新增資料 (2)刪除資料 (3)顯示 (4)清除所有資料 \n\n");
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
			case'4':
				free_list();
				break;
			default:
				printf("error\n");
				break;
		}	
	}

	return 0;
} 
