#include <global.h>
void pinfo(char *s)
{
	char a[100];
	for(int i=5;i<strlen(s);i++)
	{
		a[i-5]=s[i];
	}
	a[strlen(s)-5]='\0';
	int p;
	if(strlen(a)==0)
	{
		p=getpid();
	}
	else
	{
		readcmd(a);
		behind(a);
		p=atoi(a);
	}
	char curr[100];
	getcwd(curr,100);
	char o[100];
	sprintf(o,"/proc/%d/",p);
	chdir(o);
	char data[10000];
	printf("pid -- %d\n",p);
	int file=open("stat", O_RDONLY);
	if(file==-1)
	{
		perror("");
	}
	int y=read(file,data,1000);
	char u[100];
	sprintf(u,"/proc/%d/exe",p);
	char data1[1000];
	char data2[1000];
	int j=0;
	char* token =strtok(data," ");
	while(token!=NULL)
	{
		if(j==22)
		{
			strcpy(data2,token);
			break;
		}
		if(j==2)
		{
			strcpy(data1,token);
		}
		token=strtok(NULL, " ");
		j++;
	}
	printf("Process Status -- %s\n",data1);
	printf("memory -- %s\t{Virtual Memory}\n",data2);
	char d[PATH_MAX+1];
	char *Y=realpath(u,d);
	printf("Executable Path -- %s\n",d);
	close(file);
	chdir(curr);
}