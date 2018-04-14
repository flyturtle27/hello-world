#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<iostream>
using namespace std;


int main(int argc , char* argv[])
{
    FILE *file,*file2 ;

    file = fopen("map.txt", "r");
	file2 = fopen("result.txt", "r");

	char str1[50];
	char str2[50];
	int r=0;
	for(int i=0;i<1000;++i)
	{
		fscanf(file,"%s",str1);
		fscanf(file2,"%s",str2);
		if(strcmp(str1,str2)!=0)
			r++;
	}

	printf("错误识别数:%d\n",r);
	printf("正确率:%lf\n",1.0-((double)r)/1000);
    fclose(file) ;
	fclose(file2);
    return 0 ;
}
