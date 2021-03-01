#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node{
	
	char data[101];
	struct list * next;

}typedef node;

void test(char *a)
{
	printf("test:%s\n",a);
}


int main()
{
	char input[105];
	char *i;
	i=input;
	char po[2] = ",";
	node *fst = NULL;
	node *cur = NULL;
	node *dre = NULL; 

	for(int q=0;q<10;q++)
	{
		//scanf("%[^\n]",i); scanf到第二次時會不斷的收取空白
		scanf("%s",i);
		char *d= strtok(i,po);
		if(strcmp(d,"test")==0)
		{		
			//node * a = (node*)malloc(sizeof(node));	
			test(strtok(NULL,po));
		}
		else if(strcmp(d,"__exit")==0)
		{
			break;	
		}
		//i=0;
	
	//strcpy(a -> data, strtok(NULL,po));
	//printf("data:%s\n",a -> data);

	}

	return 0;
}
