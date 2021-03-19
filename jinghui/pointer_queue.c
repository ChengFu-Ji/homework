/**��VLinkList**/ 
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
	char change[101];						 //�]gets�u����char���A 
	ptr = (struct node*)malloc(sizeof(struct node)); 
	fgets(change, sizeof(change),stdin);			 //stdin���зǿ�J 
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
	char del_data[101];
	printf("�R�����\n");
	fgets(del_data,sizeof(del_data),stdin);		//�ؼЧR�������

	//1.�ؼи�Ʀb�Ĥ@��
	//2.�ؼи�Ʀb��
	//3.�ؼи�Ʀb����
	
	prev=head;					//���Y�}�l
	current = prev->next;
	if(strcmp(del_data,current->data) == 0)		//strcmp����r��۲Ŧ^��0
	{
		prev->next = current->next;		//�e�@�ӫ��V�R����ƪ��U�@��
		free(ptr);
		printf("�w�R�����%d\n",del_data);
	}
	else
		printf("�䤣����\n");	

}

int save()
{
	FILE *fp;
	prev = head;
	current = prev->next;
	fp = fopen("FileName.txt","w+");			//�}�ҭnŪ���,w+���طs�ťդ��,Ū���M�g�J

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
		printf("(1)�s�W��� (2)�R����� (3)��� (4)�M���Ҧ���� \n\n");
		option=getche();			//����conio.h 
		printf("\n");
		switch(option)
		{
			case'1':
				printf("��J���(�̦h�u���J100�줸)\n\n");
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
