#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void file_n_len(FILE *target_file , char *file_name);
void seek_stack(int *respond , int n , int *data_len); 

int main()
{
	
	FILE *fp1;	
	int n , seek_temp = 0 , file_check_count = 0 , data_len = 0 , flag = 0;
	char *data , *file_name;	

	file_name = (char *)malloc(128);
	memset(file_name,'\0',128);

	//檔名輸入
	printf("File_name : ");
	scanf("%[^\n]",file_name);

	//確認檔案存在
	if(fopen(file_name,"r"))
	{
		printf("Success\n");	
	}
	else
	{
		printf("Error : File is not true\n");	
		return 1;
	}

	//輸入第n筆資料
	printf("n : ");
	scanf("%d",&n);

	file_n_len(fp1,file_name);
	seek_stack(&seek_temp , n , &data_len);	

	//showN 執行
	fp1 = fopen(file_name,"r+");
	fseek(fp1,seek_temp + n - 1,SEEK_SET);
	
	printf("%ld\n",ftell(fp1));

	data =(char *)malloc(data_len+1);	

	fread(data,data_len,1,fp1);
	printf("%s\n",data);

	fclose(fp1);	
	free(file_name);

	return 0;
}

//記錄每筆資料長度到index
void file_n_len(FILE *target_file , char *file_name)
{
	target_file = fopen(file_name,"r+");
	fseek(target_file,0,SEEK_END);
	FILE *index_file = fopen("index","w+");
	char *data_temp;
	int file_size = ftell(target_file) , count = 0 , prev_count = 0;	
	
	data_temp = (char *)malloc(file_size+1);
	fseek(target_file,0,SEEK_SET);
	fread(data_temp,file_size,1,target_file);

	while(1)
	{
		if(*(data_temp + count) == '\n')	
		{
			//處理第一筆資料
			if(prev_count == 0)	
			{
				fprintf(index_file , "%d\n" , count - prev_count);
			}
			else
			{
				fprintf(index_file , "%d\n" , count - prev_count - 1);
			}	

			prev_count = count;
		}
		
		if(count == file_size)	
		{	
			break;	
		}
		count++;
	}

	fclose(target_file);	
	fclose(index_file);
}

void seek_stack(int *respond , int n , int *data_len)
{
	FILE *index_file = fopen("index","r+");
	fseek(index_file,0,SEEK_END);	
	
	int file_size = ftell(index_file) , count = 0 , passed_index = 0;
	char *temp;
	
	fseek(index_file,0,SEEK_SET);	

	while(1)
	{
		temp =(char *)malloc(file_size);	
		memset(temp,'\0',file_size);	

		fscanf(index_file,"%s",temp);

		if(count == file_size || count == (n - 1) )
		{	
			break;
		}

		*respond += atoi(temp);
		count++;
		
		free(temp);
	}

	*data_len = atoi(temp);

	free(temp);
	fclose(index_file);
}

