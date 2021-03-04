/**********************************
目前是沒有使用file寫入linkedlist的，
有 add 、 del 、 showlist 功能，
輸入範例:add,1 、del,1 、showlist。
***********************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct node{
    char data[104];
    struct node *next;
};

struct node * add(char * data, struct node * fst)
{
    struct node * fstinadd = fst;
    struct node * preinadd = NULL;
    struct node * curinadd = NULL;
   
    preinadd = fstinadd;

    if(preinadd != NULL)
    {
        while(preinadd -> next != NULL)
        {
            preinadd = preinadd -> next;
        }
    }
   
    curinadd =(struct node *)malloc(sizeof(struct node )); 
            
    if(curinadd == NULL )
    {
        return NULL;
    }

    if(fstinadd == NULL )
    {
        fstinadd = curinadd;
        strcpy( fstinadd -> data , data);
        fstinadd -> next = NULL;
    }
    else
    {
        strcpy( curinadd -> data , data);
        
        if( fstinadd -> next == NULL)
        {
            fstinadd -> next = curinadd;
        }
        else if( preinadd != NULL)
        {
            preinadd -> next = curinadd;
            curinadd -> next = NULL;     
        }   
        preinadd = curinadd;
    }
    return fstinadd;    
}

struct node * del(struct node * fst , char *data)
{ 
    struct node * fstindel = fst;
    struct node * curindel = NULL; 
    struct node * preindel = NULL;

    if( fstindel == NULL)
    {
        printf("not thing to del\n");
    }
    else
    {
        curindel = fstindel;
        preindel = fstindel; 

        while( curindel != NULL)
        {
            if(strcmp( curindel -> data, data) == 0 )break;

                    curindel = curindel -> next;
        }

        if( curindel == fstindel && curindel !=NULL )
        {
            fstindel = fstindel -> next;                    
            free(curindel);
        }
        else
        {
            while( preindel -> data != curindel -> data )
            {  
                if( preindel -> next == NULL )
                {
                    printf("nofind\n");
                    break;
                }
                    
                if( preindel -> next == curindel )    
                {
                    preindel -> next = curindel -> next; 
                    free(curindel);
                    break;
                }
                preindel = preindel -> next;

             }   
         }
    }
    return fstindel;
}

void showlist(struct node * cur)
{
    struct node * curinshow;
    curinshow = cur;  
    
    if(curinshow == NULL)
    {
        printf("WITHOUT DATA\n");
    }
    else
    {
        printf("showlist:\n");
        while(curinshow != NULL)
        {
            printf("%s",curinshow -> data);
            curinshow = curinshow -> next;    
        }
    }
}

int main(){
    
    const char flr[2]=",";
    char data[105];
    char * inputdata;

    struct node *fst = NULL;
    
    while(1)
    {
        printf(">>");   
            
        //scanf("%103s",&data); //嘗試過[^\n]處理空白 但沒有成功 原因目前未知
        fgets(data,104,stdin);  
        inputdata = (strstr(data, flr)+1);

        if(strcmp( data, "__exit\n" ) == 0 )
        {   
            printf( "out\n" );  
            
            while(fst != NULL)
            {   
                free(fst);
                fst = fst -> next;
            }
            return 0;
        }   
        else if(strcmp( data, "showlist\n" ) == 0 )
        {
            showlist(fst);
        }
        else if(strncmp( data,"add,", 4) == 0)
        {   
            fst = add( inputdata, fst);    
        }
        else if(strncmp( data, "del," , 4) == 0 )
        {  
           fst = del(fst, inputdata);
        }
        else
        {
            printf("something wrong\n");
        }   
    }

    return 0;
}
