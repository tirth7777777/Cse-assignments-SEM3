#include <global.h>

void cd(char *command)
{
	char s[100];
	int flag=1;
	for(int i=2;i<strlen(command);i++)
	{
		flag=0;
		s[i-2]=command[i];
	}
	if(flag==0)
	{
		s[strlen(command)-2]='\0';
		readcmd(s);
		behind(s);
		if(s[0]=='~')
		{
			char j[1000];
			for(int i=1;i<strlen(s);i++)
			{
				j[i-1]=s[i];
			}
			j[strlen(s)-1]='\0';
			strcpy(s,home);
			strcat(s,j);
			int i=chdir(s);
			if(i==-1)
			{
				perror("");
			}
		}
		else
		{	
			int i=chdir(s);
			if(i==-1)
			{
				perror("");
			}
		}
	}
	else
	{	
		int i=chdir(s);
		strcpy(s,home);
	}
	strcpy(command,s);
}