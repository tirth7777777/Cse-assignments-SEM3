#include <global.h>

void echo(char *cmd)
{
	char s[100];
	for(int i=4;i<strlen(cmd);i++)
	{
		s[i-4]=cmd[i];
	}
	s[strlen(cmd)-4]='\0';
	strcpy(cmd,s);
	readcmd(cmd);
	behind(cmd);
	printf("%s\n",cmd);
}