#include<stdio.h>
#include<stdlib.h>

struct node
{
	int data;
	struct node *next;
};

struct node *head,*current,*node2,*node3,*current,*prev;

int main()
{
	printf("新增一筆資料->10");
	head = (struct node*)malloc(sizeof(struct node));	//第一個節點需先分配記憶體空間
	head->data = 10;					//head串下一筆資料，並給下一筆資料值
	current = head;						//當前指向head
	head->next=NULL;					//下一筆資料等於null
	printf("新增一筆資料->20");
	node2 = (struct node*)malloc(sizeof(struct node));	//再新增一個節點，並分配記憶體空間 
	node2->data = 20;					//賦值 
	head->next = node2;					
	node2->next = NULL;
	printf("新增一筆資料->30");
	node3 = (struct node*)malloc(sizeof(struct node));
	node3->data=30;
	node2->next=node3;
	node3->next = NULL;
	printf("印出資料");
	if(current !=NULL)
	{
	  printf("%d",current->data);
	  current=current->next;
	}
	printf("刪除資料->20");
	current=head;
	while((current != NULL) &&(current->data != 20))	//當現在不等於null或不等於目標資料
	{
	  prev=current;						//將prev指向current的位置
	  current=current->next;				//current移動到下一節點
	}
	if(current != NULL)
	{
	  prev->next=current->next;				//prev指向current的下一位置
	  free(current);					//釋放current空間
	}
	print("印出資料");
	current=head;
	if(current !=NULL)
	{
	  printf("%d",current->data);
	  current=current->next;
	}	
	return 0;	
}
