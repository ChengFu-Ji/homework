/**********************************
目前有 add 、 del 、 showlist、save、load功能，
輸入範例:add,1 、del,1 、showlist、save,filename、load,filename。
跳離請打exit
***********************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct node{
    char data[104];
    struct node *next;
};

struct node *add(char *data, struct node *first)
{
    struct node *first_in_add = first;
    struct node *current_in_add = NULL;
    struct node *previous_in_add = NULL;
    
    previous_in_add = first_in_add;

    if(previous_in_add != NULL)
    {
        while(previous_in_add -> next != NULL)
        {
            previous_in_add = previous_in_add -> next;
        }
    }

    current_in_add =(struct node *)malloc(sizeof(struct node)); 
          
    if(current_in_add == NULL )
    {
        return NULL;
    }

    if(first_in_add == NULL )
    {
        first_in_add = current_in_add;
        strcpy( first_in_add -> data , data);
        first_in_add -> next = NULL;
    }
    else
    {
        strcpy( current_in_add -> data, data);
        
        if( first_in_add -> next == NULL)
        {
            first_in_add -> next = current_in_add;
        }
        else if(previous_in_add != NULL)
        {
            previous_in_add -> next = current_in_add;
        }
        current_in_add -> next = NULL;
        
    }
    return first_in_add;    
}

struct node *del(struct node *first, char *data)
{ 
    struct node *first_in_del = first;
    struct node *current_in_del = NULL; 
    struct node *previous_in_del = NULL;

    if( first_in_del == NULL)
    {
        printf("not thing to del\n");
    }
    else
    {
        current_in_del = first_in_del;
        previous_in_del = first_in_del; 

        while( current_in_del != NULL)
        {
            if(strcmp( current_in_del -> data, data) == 0 )break;

                    current_in_del = current_in_del -> next;
        }

        if( current_in_del == first_in_del && current_in_del != NULL )
        {
            first_in_del = first_in_del -> next;                    
            free(current_in_del);
        }
        else
        {
            while( previous_in_del -> data != current_in_del -> data )
            {  
                if( previous_in_del -> next == NULL )
                {
                    printf("data no find.\n");
                    break;
                }
                    
                if( previous_in_del -> next == current_in_del )    
                {
                    previous_in_del -> next = current_in_del -> next; 
                    free(current_in_del);
                    break;
                }
                previous_in_del = previous_in_del -> next;

             }   
         }
    }
    return first_in_del;
}

void showlist(struct node *first)
{
    struct node * current_in_showlist;
    current_in_showlist = first;  
    
    if(current_in_showlist == NULL)
    {
        printf("WITHOUT DATA\n");
    }
    else
    {
        printf("showlist:\n");
        while(current_in_showlist != NULL)
        {
            printf("%s", current_in_showlist -> data);
            current_in_showlist = current_in_showlist -> next;    
        }
    }
}

void save(char *file, struct node *data)
{   
    FILE * file_save;
    struct node * save_data;
    save_data = data; 
    file_save = fopen(file, "w");

    while(save_data != NULL)
    {
        fprintf(file_save, "%s", save_data -> data);
        save_data = save_data -> next;
    }
    fclose(file_save);
}

struct node *load(char * file, struct node * data)
{
    FILE *file_load; 
    struct node *load_data;
    char file_data_puffer[101];

    file_load = fopen(file, "r");
   
    if( file_load == NULL)
    {
        printf("no search file.\n");
    }
    else
    { 
        while(fgets(file_data_puffer, 101, file_load) != NULL)
        {
            data=add(file_data_puffer, data);
        }

        fclose(file_load);
        printf("load file complete.\n");
    }

    return data; 
}

int main(){
    
    const char comma[2]=",";
    char input_data_buffer[105];
    char *input_data;
    FILE *filename;
    struct node *first = NULL;
    
    printf("welcome! we have file control with 'save' and 'load' ex: save,filename.filename Extension same as load.\nAnd build linkedlist we need 'add' and 'del' ex: add,data same as del.\nIf you want to show what thing in linkedlist.You can use 'showlist'.  \nAlso leave the progream please keyin 'exit'.\n");
    while(1)
    {
        printf(">>");   
            
        //scanf("%103s",&data); //嘗試過[^\n]處理空白 但沒有成功 原因目前未知
        fgets(input_data_buffer, 104, stdin);  
        input_data = (strstr(input_data_buffer, comma)+1);

        if(strcmp( input_data_buffer, "exit\n" ) == 0 )
        {   
            printf( "out\n" );  
            
            while(first != NULL)
            {   
                free(first);
                first = first -> next;
            }

            return 0;
        }   
        else if(strcmp( input_data_buffer, "showlist\n") == 0)
        {
            showlist(first);
        }
        else if(strncmp( input_data_buffer, "add,", 4) == 0)
        {   
            first = add( input_data, first);    
        }
        else if(strncmp( input_data_buffer, "del,", 4) == 0)
        {  
            first = del(first, input_data);
        }
        else if(strncmp( input_data_buffer, "save,", 5) == 0)
        {
            *(strstr(input_data, "\n")) = '\0';
            save(input_data, first); 
            printf("save file complete.\n");
        }    
        else if(strncmp( input_data_buffer, "load,", 5) == 0)
        {
            *(strstr(input_data, "\n"))='\0';
            first=load(input_data, first);
        }
        else
        {
            printf("something wrong\n");
        }   
    }

    return 0;
}
