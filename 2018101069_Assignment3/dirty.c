#include <global.h>

void dirty(char *s)
{
	readcmd(s);
	behind(s);
	if(s[0]=='-'&&s[1]=='n')
	{
		char t[1000];
		for(int i=2;i<strlen(s);i++)
		{
			t[i-2]=s[i];
		}
		t[strlen(s)-2]='\0';
		readcmd(t);
		char r[2];
		r[0]=t[0];
		r[1]='\0';
		int j=atoi(r);
		while(1)
		{
			changemode(1);
			char ch;
			if(kbhit())
			{
				ch=getchar();
				if(ch=='q')
				{
					break;
				}
			}
			changemode(0);
			int file=open("/proc/meminfo",O_RDONLY);
			char data[1000000];
			read(file,data,100000);
			char *token=strtok(data,"\n");
			char o[1000];
			int i=0;
			while(token!=NULL)
			{
				if(i==16)
				{
					strcpy(o,token);
					break;
				}
				token=strtok(NULL,"\n");
				i++;
			}
			token=strtok(o," ");
			i=0;
			char r[1000];
			while(token!=NULL)
			{
				if(i==1)
					strcpy(r,token);
				i++;
				token=strtok(NULL," ");
			}
			printf("%s\n",r);
			sleep(j);
		}
	}
	else
	{
		printf("Invlaid flag\n");
	}
}