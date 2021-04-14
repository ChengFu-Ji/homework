#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node
{
	char data[101];
	struct node *next;
};

struct node *head,*current,*prev,*ptr,*target;
void add(char *str)
{
	ptr = (struct node*) malloc(sizeof(struct node));
	strcpy(ptr->data,str);
	printf("add>>data:%s\n",ptr->data);
	ptr -> next = NULL;
	if(head != NULL)
	{
		current = head;
		while(current->next != NULL)
		{
			current = current -> next;
		}
		current->next = ptr;
	}
	else
		head = ptr;
}
void list()
{
	printf("\nShow List\n");
	if(head != NULL)
	{
	  current = head;
	  while(current != NULL)
	  {
		printf("data: %s\n",current->data);
		current = current->next;
	  }
	}
	else
		printf("error\n");
	
}

void del(char *str)
{
	printf("Del_Data\n\n");
	prev = current = head;
	while(current != NULL)
	{
		
		if(strcmp(str,current->data) == 0)
		{
			target = current;
			prev->next = current->next;
			printf("del: %s\n",target->data);
			printf("prev_data: %s\n",prev->data);
			free(target);
		}
		prev = current;
		current = current->next;
	}
	
}

void save()
{
	FILE *fp;
	current = head;
	fp = fopen("FileName.txt","w+");

	while(current != NULL)
	{
		fprintf(fp,"%s",current->data);
		current = current->next;
	}
	fclose(fp);
}

void load()
{
	FILE *fp;
	if((fp = fopen("FileName.txt","r")) == NULL)
	{
		printf("first data");
	}
}
		
int main()
{
	add("a");
	add("b");
	add("c");
	add("d");
	add("e");
	add("f");
	list();
	del("d");
	list();
	save();
	return 0;
}
