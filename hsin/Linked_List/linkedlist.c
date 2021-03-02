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
    struct node *next;
};

/*
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
            pre = d ;
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

*//*
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

*/
int main(){
    
    struct node *tmp  = NULL;
    const char flr[2]=",";
    char data[105];
    char * dd;

    struct node *fst = NULL;
    struct node *pre = NULL;
    struct node *cur = NULL;
    
    while(1)
    {
        printf(">>");   
            
        //scanf("%103s",&data); //嘗試過[^\n]處理空白 但沒有成功 原因目前未知
        fgets(data,104,stdin);  
        if(strcmp( data, "__exit\n" ) == 0 )
        {   
            printf( "out\n" );  
            tmp=fst;    
            while(tmp != NULL)
            {   
                    free(tmp);
                    tmp = tmp -> next;
            }
            return 0;
        }   
        else if(strcmp( data, "showlist\n" ) == 0 )
        {
            if( fst == NULL)
            {
                printf("nothing\n");
            }
            else
            {   
                printf("showlist\n");
                tmp = fst;
                while( tmp != NULL)
                {
                    printf("%s", tmp -> data);
                    tmp = tmp -> next;
                }
            }   
        }
        else if(strncmp( data,"add,",4)==0)
        {   
            cur =(struct node *)malloc(sizeof(struct node )); 
            if(cur == NULL )
            {
                return 1;
            }
            
            if(fst == NULL )
            {
                fst = cur;
                strcpy( fst->data , strstr(data,flr)+1 );
                fst -> next = NULL;
            }
            else
            {
                strcpy( cur->data , strstr(data,flr)+1);
                if( fst -> next == NULL)
                {
                    fst -> next = cur;
                }
                else if( pre != NULL)
                {
                    pre -> next = cur;
                    cur -> next = NULL;     
                }   
                pre = cur;
            }
        }
        else if(strncmp( data, "del," , 4) == 0 )
        {   
           if( fst == NULL)
           {
                printf("not thing to del\n");
           }
           else
           {

                dd = strstr(data, flr)+1;
                cur = fst;
                pre = fst; 

                while( cur != NULL)
                {
                    if(strcmp( cur-> data , dd) == 0 )
                    {
                        break;      
                    }
                    cur = cur -> next;
                }

                if( cur == fst )
                {
                    fst = fst -> next;                    
                }
                else
                {
                    while( pre -> data != cur -> data )
                    {
                        if( pre -> next == NULL )
                        {
                            printf("nofind\n");
                            break;
                        }

                        if( pre -> next == cur)    
                        {
                            pre -> next = cur -> next; 
                            free(cur);
                            break;
                        }
                        cur = pre -> next;
                    }   
                }
            }
        }
        else
        {
            printf("something wrong\n");
        }   
    }

    return 0;
}
