/***************************************************
簡易輸入測試程式
當輸入為"_exit"時結束迴圈，即結束程式
****************i***********************************/
#include<stdio.h>
#include<string.h>
int main()
{
    char exit[20]="_exit";
    char txt[20];

    while(1)
	{
        scanf("%19s",txt);
        if(strcmp(exit,txt)==0)break;
        printf("what the %s\n",txt);
    }

return 0;
}
