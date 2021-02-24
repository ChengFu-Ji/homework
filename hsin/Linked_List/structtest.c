#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node{
	
	char data[10];
	struct list * next;

}typedef node;

int main()
{
	node * a = (node*)malloc(sizeof(node));	

	printf("a->data,len:%d\n",sizeof( a -> data ));
	scanf("%[^\n]",a -> data);
	printf("data:%s\n",a -> data);

	return 0;
}
