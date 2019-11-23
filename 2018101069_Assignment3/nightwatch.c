#include <global.h>

void nightwatch(char *s)
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
		printf("CPU0\tCPU1\tCPU2\tCPU3\n");
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
			int file=open("/proc/interrupts",O_RDONLY);
			if(file==-1)
			{
				perror("");
			}
			char data[1000000];
			read(file,data,100000);
			char *token=strtok(data,"\n");
			char o[1000];
			int i=0;
			while(token!=NULL)
			{
				if(i==2)
				{
					strcpy(o,token);
				}
				token=strtok(NULL,"\n");
				i++;
			}
			token=strtok(o," ");
			i=0;

			while(token!=NULL)
			{
				if(i>0)
				{
					printf("%s\t",token);
				}
				i++;
				token=strtok(NULL," ");
			}
			printf("\n");
			sleep(j);
		}

	}
	else
	{
		printf("Invlaid flag\n");
	}
}