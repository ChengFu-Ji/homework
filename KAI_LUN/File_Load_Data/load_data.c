#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	
	FILE * fp;
	char * temp , *data , *file_name;
	int count = 0 , count_temp = 0 , flag_num = 1 , n = 1;

	temp = (char *)malloc(128);
	file_name = (char *)malloc(32);	

	printf("Current File :\n");
	system("ls");

	printf("Topic File : ");
	scanf("%s",file_name);
	
	fp = fopen(file_name,"r");
	fread(temp,128,1,fp);	
	fseek(fp,0,SEEK_SET);

	printf("Select Data : ");
	scanf("%d",&n);	

	while(1)
	{
		if(temp[count] == '\n')
		{	
			data = (char *)malloc(128);
			fread(data,count-count_temp,1,fp); //讀取時檔案指向的位址會移動
			fseek(fp,1,SEEK_CUR);
			count_temp = count + 1;
			
			if(flag_num == n)	
			{
				printf("data = %s\n",data);
				break;	
			}
			
			free(data);	
			flag_num = flag_num + 1;
		}

		if(temp[count] == '\0')
		{
			break;	
		}

		count++;	
	}

	fclose(fp);
	return 0;
}

