#include <stdio.h>
#include <stdlib.h>
#include "mysql/mysql.h"
#include "mysqllib.h"

int main()
{
        setvbuf(stdout, NULL, _IONBF, 0);
        char input[] = "";
        printf("\nWelcome Dict! Enter 0 Exit!\n\n");
        int sqlconcheck = sqlcon();
        
        if (sqlconcheck == 0)
        {
                while(1){
                        printf("請輸入國家編號：");
                        scanf("%s",&input);
                        if(input[0] == '0'){
                                break;
                        }
                        countryselect(input);
                        printf("\n");
                }
        }
        else
        {
                return 1;
        }  
        
        sqlclose();
        printf("Bye~\n\n");
        return 0;
}
