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
	printf("�s�W�@�����->10");
	head = (struct node*)malloc(sizeof(struct node));	//�Ĥ@�Ӹ`�I�ݥ����t�O����Ŷ�
	head->data = 10;					//head��U�@����ơA�õ��U�@����ƭ�
	current = head;						//��e���Vhead
	head->next=NULL;					//�U�@����Ƶ���null
	printf("�s�W�@�����->20");
	node2 = (struct node*)malloc(sizeof(struct node));	//�A�s�W�@�Ӹ`�I�A�ä��t�O����Ŷ� 
	node2->data = 20;					//��� 
	head->next = node2;					
	node2->next = NULL;
	printf("�s�W�@�����->30");
	node3 = (struct node*)malloc(sizeof(struct node));
	node3->data=30;
	node2->next=node3;
	node3->next = NULL;
	printf("�L�X���");
	if(current !=NULL)
	{
	  printf("%d",current->data);
	  current=current->next;
	}
	printf("�R�����->20");
	current=head;
	while((current != NULL) &&(current->data != 20))	//��{�b������null�Τ�����ؼи��
	{
	  prev=current;						//�Nprev���Vcurrent����m
	  current=current->next;				//current���ʨ�U�@�`�I
	}
	if(current != NULL)
	{
	  prev->next=current->next;				//prev���Vcurrent���U�@��m
	  free(current);					//����current�Ŷ�
	}
	print("�L�X���");
	current=head;
	if(current !=NULL)
	{
	  printf("%d",current->data);
	  current=current->next;
	}	
	return 0;	
}
