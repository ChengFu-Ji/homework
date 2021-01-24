/**��C�[�J�M�R��**/ 
#include<stdio.h> 
#include<stdlib.h> 


struct node //�إ߸`�I 
{
	int data[20];
	struct node *next;
};

struct node *ptr,*head,*current,*prev;
 

void add()
{
	//�إ߲Ĥ@�� node //
	head = (struct node*)malloc(sizeof(struct node));
	gets(head->data);	//��J 
	head->next = NULL;	//�U�@�`�I��null 
	
	if(ptr == NULL)
	{
		ptr = head; //���w���ŭȮɫ��Vhead 
		return;
	}
	else 
	{
		current = ptr;
		while(current->next != NULL) 
		{
			current = current->next; 
		}
		current->next = head;
		return;
	}		
}


void list()
{
	int count = 0;

	if(head->data == NULL)
	{
		printf("�L���\n\n");
	}
	else
	{
		printf("-------------�C��---------------------\n\n");
		current = current->next;
		while(current != NULL)
		{
			printf("%d\n",current->data);
			count++;
			current = current->next;
		}
		printf("---------------------------------\n");
	}
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
	printf("�R�����\n");
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
			printf("%d�w�Q�R��\n",del_data);
		}
		else
		{
			current = current->next;
			printf("�䤣��Ӹ��\n");
		}	
	}
	getch();
} 

int main()
{
	int option;
	while(1)
	{
		printf("(1)�s�W��� (2)�R����� (3)��� \n\n");
		option=getche();
		printf("\n");
		switch(option)
		{
			case'1':
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
