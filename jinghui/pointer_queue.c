/**��VLinkList**/ 
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
	char change[4];						 //�]gets�u����char���A 
	ptr = (struct node*)malloc(sizeof(struct node)); 
	gets(change);
	ptr->data = atoi(change);				 //atoi()�O�N�r���নint���A 
	ptr->next = NULL;
	if(head == NULL)
	{	
	  head=ptr;					 	 //�]�Ĥ@����Ƭ�head
	}
	else
	{
	  current = head; 					//�q�Y�}�l 
	  while(current->next != NULL)				//�p�G��e���A�������̫�@�� 
	  {
		 current = current->next;			//���V�U�@��
	  }
	  current->next = ptr; 					//����s�W���
	}
}


void list()
{
	printf("-------------�C��---------------------\n\n");
	if(head == NULL)
	{
	  printf("�L���\n");
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

//�������쵲�Ŷ� 
//���禡���Ӱ��D�Ohead�L�kfree�� 
void free_list()
{
	if(head == NULL)
	{
	  printf("�L���\n");	
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
	printf("�R�����\n");
	gets(del_data);					//�ؼЧR�������

	//1.�ؼи�Ʀb�Ĥ@��
	//2.�ؼи�Ʀb��
	//3.�ؼи�Ʀb����
	
	current=head;					//���Y�}�l
	printf("head���%d",head->data);
	if(head->data == del_data)			//�ؼЦb�Y
	{
	  ptr = head;					//�n�R�����ؼ�
	  head = current->next;				//�U�@�Ӭ�head
	  free(ptr);					//�R���ؼ�
	  current = head->next;				//����current
	}
} 

int main()
{

	int option;
	while(1)
	{
		printf("(1)�s�W��� (2)�R����� (3)��� (4)�M���Ҧ���� \n\n");
		option=getche();
		printf("\n");
		switch(option)
		{
			case'1':
				printf("��J���\n\n");
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
