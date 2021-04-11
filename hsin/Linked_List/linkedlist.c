/**********************************
目前有 add 、 del 、 showlist、save、load、 show_n_line功能，
輸入範例:add,1 、del,1 、showlist、save,filename、load,filename、show,filename,row，
跳離請打exit。
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

struct node *cleanlist(struct node *first)
{ 
    struct node *first_in_clean = first;
    struct node *current_in_clean = NULL; 

    if( first_in_clean == NULL)
    {
        printf("not thing to clear\n");
    }
    else if( first_in_clean != NULL)
    {
        current_in_clean = first_in_clean;
        
        while( current_in_clean != NULL)
        { 
            current_in_clean = current_in_clean -> next;
            free( first_in_clean );
            first_in_clean = current_in_clean;
        }
        printf("mission complete.\n");
    }
    return first_in_clean;
}

void showlist(struct node *first)
{
    struct node *current_in_showlist;
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
    FILE *file_save;
    FILE *file_index;

    struct node *save_data;
    save_data = data;

    int len = strlen(file) + 8;
    char file_index_name[len];
    strcpy(file_index_name,".index_");
    strcat(file_index_name, file);

    file_save = fopen(file, "w"); 
    file_index = fopen(file_index_name, "w");        //製造index檔案，他是個隱藏檔。
    int count = 0;
    int lencount = 0;

    while(save_data != NULL)
    {
        if(lencount == 0)
        {
           lencount++; 
        }
        else
        {
            lencount = lencount + strlen(save_data -> data);
        }
        fprintf(file_index, "%d", lencount);
        fprintf(file_save, "%s", save_data -> data);         
        save_data = save_data -> next;
        count++;
    }
    fprintf(file_index, "%d", count);
    printf("count:%d\n",count);
    fclose(file_save);
    fclose(file_index);
}

struct node *load(char *file, struct node *data)
{
    FILE *file_load; 
    struct node *load_data;
    char file_data_buffer[101];

    file_load = fopen(file, "r");
   
    if( file_load == NULL)
    {
        printf("no search file.\n");
    }
    else
    { 
        while(fgets(file_data_buffer, 101, file_load) != NULL)
        {
            data=add(file_data_buffer, data);
        }

        fclose(file_load);
        printf("load file complete.\n");
    }
    return data; 
}

void show_n_line(char *filename, int n_line)
{
    FILE *file_load; 
    FILE *file_index;

    char file_data_buffer[101];

    int len = strlen(filename) + 8;
    char file_index_name[len];
    strcpy(file_index_name,".index_");
    strcat(file_index_name, filename);

    file_load = fopen(filename, "r");
    file_index = fopen(file_index_name, "r");

    int count_rows = 1;
    int count_max = 0;
    char max_tmp[2];
    char count_rown[2];

    if( file_load == NULL)
    {
        printf("no search file.\n");
    }
    else
    { 
        if(file_index != NULL)                      //方法一，有index檔案時則使用index檔案的紀錄直接位移指標取值。
        {
            fseek(file_index, -1, SEEK_END);
            fgets(max_tmp, 2, file_index);        
            count_max = atoi(max_tmp);

            if( count_max == 0)
            {
                printf("index file error.\n");
            }
            else if( count_max < n_line)
            {
                printf("your request is too big.\n");
            }
            else if( count_max >= n_line && count_max > 0)
            {
                fseek(file_index, n_line-1, SEEK_SET); 
                fgets(count_rown, 2, file_index);

                fseek(file_load, atoi(count_rown)-1, SEEK_SET); 
                fgets(file_data_buffer, 2, file_load);
                printf("%s\n",file_data_buffer);
            }
            else
            {
                printf("no search the line,please check the file and try again.\n");
            }       

            fclose(file_index);
        }
        else
        {
            printf("no search index file but we can search file data too!\n");
            while(count_rows <= n_line && fgets(file_data_buffer, 101, file_load) != NULL)      //方法二(原方法)，一個一個累加取得資料。
            {
                count_rows ++;
            }  

            if(count_rows - 1 == n_line && n_line != 0)
            { 
                printf("%s",file_data_buffer);
                printf("show n line down.\n");
            }
            else
            {
                printf("no search the line,please check the file and try again.\n");
            }       
        }

        fclose(file_load); 
    }
}

int main(){
    
    const char comma[2]=",";
    char input_data_buffer[105];
    char *input_data;
    struct node *first = NULL;
    
    printf("welcome! we have file control with 'save' and 'load' ex: save,filename.filename Extension same as load.\nAnd build linkedlist we need 'add' and 'del' ex: add,data same as del.\nIf you want to show what thing in linkedlist.You can use 'showlist'. \nOr show the data of the line in the file. ex: show,filename,row. \nAlso leave the progream please keyin 'exit'.\n");
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
        else if(strncmp( input_data_buffer, "cleanlist", 5) ==0)
        {
            first = cleanlist(first);
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
        else if(strncmp( input_data_buffer, "show,", 5) == 0)
        {
            int number;
            char *buffer_of_data;
            char *filename;
            int count_comma = 0;
                 
            *(strstr(input_data, "\n"))='\0';
            buffer_of_data = strtok(input_data, comma);

            while( count_comma != 2)
            {
                count_comma ++;

                if(count_comma == 1)
                {    
                    filename = buffer_of_data; 
                }
                else if(count_comma == 2)
                {
                    number = atoi(buffer_of_data);
                }

                buffer_of_data = strtok(NULL,comma);
            }
            show_n_line(filename, number);
        }
        else
        {
            printf("something wrong\n");
        }   
    }

    return 0;
}
