#include<stdio.h>
#include<stdlib.h>

struct link 
{
    int x;
    struct link *next; 
}typedef list;


list * create_node(int input,list *current_node);

int main()
{
    list *node1 = (list *)malloc(sizeof(list *));
    list *current;
    int input;
    int count = 0;
    
    node1->x = (int)NULL;
    node1->next = NULL;

    scanf("%d",&input);
    current = create_node(input,node1); 
  
    for(int i = 0 ; i < 3 ; i++)
    {
        current = create_node(i,current); 
        printf("current = %p \n",current);
    }

    return 0;
}

list * create_node(int input,list *current_node)
{
    list *new_node = (list *)malloc(sizeof(list *));
   
    current_node->next = new_node;
    current_node->next->x = input;
    new_node->next = NULL;
    
    printf("current_node  = %p  current_node -> next = %p \n",current_node,current_node->next);

    return new_node;
}
