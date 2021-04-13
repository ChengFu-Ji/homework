#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int data_n(FILE *fp,char *file_name);
void file_n_len(char *file_name,int *array);

int main(int argc , char * argv[])
{
	FILE *fp1;	
	int n , seek_temp = 0 , file_check_count = 0;
	char *data , *input_file_name;
	int *test_len[argc-1] , which_file;
	
	input_file_name = (char *)malloc(32);

	printf("Now have file :\n ");
	//將多個檔案中各個資料長度寫入array
	for(int i = 1 ;i < argc ; i++)
	{
		test_len[i-1] = (int *)malloc(data_n(fp1,argv[i]));
		
		//寫入每筆資料的長度
		file_n_len(argv[i],test_len[i-1]);
		printf("\t%s\n",argv[i]);	
	}
	
	printf("Input the file name : \n");
	//輸入檔案名稱
	scanf("%[^\n]",input_file_name);	
	for(int i = 1 ; i < argc ; i++)
	{
		if(strcmp(input_file_name , argv[i]) == 0)	
		{
			which_file  = i;	
		}	
		else
		{
			file_check_count++;	
		}
	}	
	if(file_check_count == (argc-1) || *input_file_name == '\n' )
	{
		printf("=============================================\n");
		printf("The file entered by the user does not exist\n");
		printf("=============================================\n");
		return 1;	
	}
	
	printf("Need n with %s : ",argv[which_file]);
	
	//輸出第n筆資料
	scanf("%d",&n);
	fflush(stdin);	
	
	if(n < 0 || n > data_n(fp1,argv[which_file]))	
	{
		printf("==========================\n");
		printf("User input n out of range\n");	
		printf("==========================\n");
		return 1;
	}

	//移多少位置	
	for(int i = 0 ; i < n-1 ; i++)
	{
		seek_temp += *(test_len[which_file-1] + i);	
	}
	
	seek_temp += (n-1);
	
	if(seek_temp < 0)
	{
		printf("==================\n");
		printf("n is not a number\n");	
		printf("==================\n");
		return 1;	
	}
	else
	{
		printf("Current Seek = %d\n", seek_temp);	
	}
	
	//指定要讀的檔案
	fp1 = fopen(argv[which_file],"r+");
	fseek(fp1,seek_temp,SEEK_SET);
	data = (char *)malloc(*(test_len[which_file-1] + n) + 1);	
	
	fread(data,*(test_len[which_file-1] + n-1),1,fp1);
	printf("%s\n",data);	
	
	return 0;
}

//知道幾筆資料
int data_n(FILE *fp,char *file_name)
{
	fp = fopen(file_name,"r");	
	fseek(fp,0,SEEK_END);	

	int count = 0 , file_size = ftell(fp) , data_count = 0;
	char *data_temp;
		
	data_temp = (char *)malloc(file_size+1);
	fseek(fp,0,SEEK_SET);
	
	
	while(1)
	{
		fread(data_temp,1,1,fp);	
		
		if(*data_temp == '\n')
		{
			data_count++;	
		}

		if(count == file_size)
		{
			break;
		}	
		
		count++;	
	}
	fclose(fp);

	return data_count-1;
}

//記錄每筆資料長度到array
void file_n_len(char *file_name,int *array)
{
	FILE *fp = fopen(file_name,"r+");
	fseek(fp,0,SEEK_END);

	char *data_temp;
	int file_size = ftell(fp) , count = 0 , prev_count = 0 , array_count = 0;	
	
	data_temp = (char *)malloc(file_size+1);
	fseek(fp,0,SEEK_SET);
	fread(data_temp,file_size,1,fp);

	while(1)
	{
		if(*(data_temp + count) == '\n')	
		{
			//處理第一筆資料
			if(prev_count == 0)	
			{
				*(array + array_count) = count - prev_count;	
			}	
			else
			{
				*(array + array_count) = count - prev_count - 1;	
			}	

			prev_count = count;
			array_count += 1;
		}
		
		if(count == file_size)	
		{	
			break;	
		}
		count++;
	}
	fclose(fp);		
}
