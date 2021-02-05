/***********************************************************
配合inetd使用
_exit 即可離開
***********************************************************/
#include<stdio.h>
#include<string.h>
int main()
{
    char exit[20] = "_exit";
    char txt[20];

    printf("What's wrong with you ?\n");
    printf("If you don't have any question. keyin _exit to left.\n");
     
    while(1)
    {
        printf(">>");

        fflush(stdout); //清空buffer並輸出

        scanf("%19s", txt);

        if(strcmp(exit, txt) == 0)break;

        printf("What is the %s ?\n", txt);
     }

     return 0;
}
