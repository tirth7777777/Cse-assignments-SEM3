#include <global.h>
void delete(struct bg *d)
{
	if(d==root)
	{

		root=root->next;
		// root=NULL;
		return;
	}
	else
	{
		strcpy(d->proc,d->next->proc);
		d->pid=d->next->pid;
		d->next=d->next->next;
	}
}
void insert(int pid,char *s)
{
		struct bg *temp=root;
		if(root==NULL)
		{
			root=(struct bg *)malloc(sizeof(struct bg));
			root->next=(struct bg *)malloc(sizeof(struct bg));
			root->pid=pid;
			strcpy(root->proc,s);
			root->next->next=NULL;
			root->next->proc[0]=0;
			root->next->pid='\0';
		}
		else
		{
			while(temp->next!=NULL)
			{
				temp=temp->next;
			}
			temp->next=(struct bg *)malloc(sizeof(struct bg));
			strcpy(temp->proc,s);
			temp->pid=pid;
			temp->next->next=NULL;
			temp->next->pid=0;
			temp->next->proc[0]='\0';
		}	
}
void andbg(char *s)
{
	int r=0;
	char **brr;
	brr=getinput(s);
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		if(execvp(s,brr)==-1)
		{
			perror("Could not create process");
			exit(-1);
		}
		exit(0);
	}
	else
	{
		
	}
	
}
