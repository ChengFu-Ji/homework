/**********************************
目前是沒有使用file寫入linkedlist的，
有 add 、 del 、 showlist 功能，
但 del和 輸入部分還有一些bug，
輸入範例:add,1 del,1 showlist

***********************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct node{
	char data[104];
	char* pdata;
	struct node *next;
};

	struct node *fst = NULL;
	struct node *pre = NULL;
	struct node *cur = NULL;

void createoradd(const char* data)
{	
	struct node *cur = (void *)data;
	
	if( fst == NULL )
	{
		fst = cur;
	}	
	else
	{
		pre -> next = cur;
	}
	cur -> next = NULL; 
	pre = cur;
		
	
}
void del(const char* no)
{
	struct node *n=(void *)no;
	struct node *dot=NULL;
	struct node *d=fst;
	
	if( fst == NULL)
	{
		printf("ERROR NOT THING CAN DELETE\n");		
	}

	if( strcmp(no,(void *)d) == 0)
	{		
		fst = fst -> next;
		free(d->data-4);			
	}
	else
	{
		while(strcmp(no,(void *)d -> next) != 0)
		{
			d = d -> next;
		}

		dot = d -> next;

		if( strcmp( no, pre -> data ) == 0 )
		{		
			dot-> next=NULL;
			pre = d	;
		}

		d-> next = dot -> next;
				
		free(dot-> data-4);	
	}
	
	free(cur);
}

void showlist()
{
	cur = fst;	
	
	if(cur == NULL)
	{
		printf("WITHOUT DATA\n");
	}
	while(cur != NULL)
	{
		printf("listshow:%s\n",cur);
		cur = cur->next;	
	}
}


int whatkind(const char* type ,const char* data)
{
	int a=0;
	if(strcmp("add",type) == 0 )
	{
		a=1;
		createoradd(data);
	}
	else if(strcmp("del",type)==0 )
	{
		a=2;
		del(data);
	}
	else
	{
		printf("Warning worng input!\n"); 
		a=4;
	}	

return a;
}


int main(){
	
	struct node *tmp  = NULL;
	const char flr[2]=",";
	char *b;	
	int No,lenoftype;

	while(1)
	{
		printf(">>");	
		tmp =(struct node *)malloc(sizeof(struct node )); 
		if(tmp == NULL )
		{
			return 1;
		}
			
		scanf("%103s",&(tmp -> data )); //嘗試過[^\n]處理空白 但沒有成功 原因目前未知
		
		if(strcmp( tmp -> data, "__exit" ) == 0 )
		{	
			printf( "out\n" );
			free(tmp);	
			tmp=fst;	
			while(tmp != NULL)
			{	
					free(tmp -> data-4);
					tmp = tmp -> next;
			}
			return 0;
		}
	
		if(strcmp( tmp -> data, "showlist" ) == 0 )
		{
			showlist();
			free(tmp);
		}
		else
		{	
			b=strtok(tmp -> data,flr);	
			if(	b!=NULL)
			{
				lenoftype = strlen(b)+1;
			}
	
			cur = tmp;
			tmp -> pdata = &(tmp ->data[lenoftype]); 	
			No=whatkind(tmp->data,tmp->pdata);
			if( No == 4 )
			{
				free(tmp);
			}
		}	
	}

return 0;
}