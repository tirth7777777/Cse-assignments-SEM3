#include <global.h>
// code for jobs
void jobs()
{
	int cnt=0;
	struct bg *temp=root;
	while(temp!=NULL&&temp->next!=NULL)
	{
		int pid =temp->pid;
		char name[10000];
		// printf("%s\n",temp->proc);
		strcpy(name,temp->proc);
		char path[1000];
		sprintf(path,"/proc/%d/status",pid);
		int file=open(path,O_RDONLY);
		// perror("");
		char hih[10000];
		read(file,hih,1000);
		char *token=strtok(hih,"\n");
		token=strtok(NULL,"\n");
		token=strtok(NULL,"\n");
		char *tok=strtok(token," ");
		if(tok[strlen(tok)-1]=='S')
		{
			printf("[ %d ] Running %s [ %d ]\n",cnt+1,name,pid);
		}
		else if (tok[strlen(tok)-1],'T')
		{
			printf("[ %d ] Stopped %s [ %d ]\n",cnt+1,name,pid);
		}
		cnt++;
		temp=temp->next;
	}
}
