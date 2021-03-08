/**********************************
目前有 add 、 del 、 showlist、save、load功能，
輸入範例:add,1 、del,1 、showlist、save,filename、load,filename。
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

void save(char * file, struct node * data)
{   
    FILE * filesave;
    struct node * savedata;
    savedata = data; 
    filesave = fopen(file, "w");

    while(savedata != NULL)
    {
        fprintf(filesave, "%s", savedata -> data);
        savedata = savedata -> next;
    }
    fclose(filesave);
}

struct node * load(char * file, struct node * data)
{
    FILE * fileload; 
    struct node * loaddata;
    char filedatapuffer[101];

    fileload = fopen(file, "r");
   
    if( fileload == NULL)
    {
        printf("no search file.\n");
    }
    else
    { 
        while(fgets(filedatapuffer, 101, fileload) != NULL)
        {
            data=add(filedatapuffer, data);
        }

        fclose(fileload);
        printf("load file complete.\n");
    }

    return data; 
}

int main(){
    
    const char comma[2]=",";
    char data[105];
    char * inputdata;
    FILE * filename;
    struct node *fst = NULL;
    
    while(1)
    {
        printf(">>");   
            
        //scanf("%103s",&data); //嘗試過[^\n]處理空白 但沒有成功 原因目前未知
        fgets(data,104,stdin);  
        inputdata = (strstr(data, comma)+1);

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
        else if(strcmp( data, "showlist\n") == 0)
        {
            showlist(fst);
        }
        else if(strncmp( data, "add,", 4) == 0)
        {   
            fst = add( inputdata, fst);    
        }
        else if(strncmp( data, "del,", 4) == 0)
        {  
            fst = del(fst, inputdata);
        }
        else if(strncmp( data, "save,", 5) == 0)
        {
            *(strstr(inputdata, "\n"))='\0';
            save(inputdata, fst); 
            printf("save file complete.\n");
        }    
        else if(strncmp( data, "load,", 5) == 0)
        {
            *(strstr(inputdata, "\n"))='\0';
            fst=load(inputdata, fst);
        }
        else
        {
            printf("something wrong\n");
        }   
    }

    return 0;
}
