/**單向LinkList**/ 
#include<stdio.h> 
#include<stdlib.h>
#include<conio.h>

struct node 
{
  int data;
  struct node *next;
};

struct node *ptr,*head,*current,*prev,*tmp;
 
void add()
{
	char change[101];						 //因gets只接受char型態 
	ptr = (struct node*)malloc(sizeof(struct node)); 
	fgets(change, sizeof(change),stdin);			 //stdin為標準輸入 
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
	char del_data[101];
	printf("刪除資料\n");
	fgets(del_data,sizeof(del_data),stdin);		//目標刪除的資料

	//1.目標資料在第一個
	//2.目標資料在尾
	//3.目標資料在中間
	
	prev=head;					//重頭開始
	current = prev->next;
	if(strcmp(del_data,current->data) == 0)		//strcmp為兩字串相符回傳0
	{
		prev->next = current->next;		//前一個指向刪除資料的下一個
		free(ptr);
		printf("已刪除資料%d\n",del_data);
	}
	else
		printf("找不到資料\n");	

}

int save()
{
	FILE *fp;
	prev = head;
	current = prev->next;
	fp = fopen("FileName.txt","w+");			//開啟要讀文件,w+為建新空白文件,讀取和寫入

	while(current != NULL)
	{
		fprintf(save,"%s",current->data);
		current = current->next;
	}
	fclose(save);
}

int load()
{
	FILE *fd;

}

int main()
{

	int option;
	while(1)
	{
		printf("(1)新增資料 (2)刪除資料 (3)顯示 (4)清除所有資料 \n\n");
		option=getche();			//隸屬conio.h 
		printf("\n");
		switch(option)
		{
			case'1':
				printf("輸入資料(最多只能輸入100位元)\n\n");
				add();
				save();
				break;
			case'2':
				del();
				break;
			case'3':
				load();
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
