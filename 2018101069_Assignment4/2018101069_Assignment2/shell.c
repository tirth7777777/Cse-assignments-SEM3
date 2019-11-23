#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>
#include <string.h>
#define clear() printf("\033[H\033[J")
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <termios.h>
#include <signal.h>
//signal(SIGCHLD, background);
char *user;
char home[100];
void readcmd(char *cmd);
void behind(char *cmd);
void gethome()
{
	char d[PATH_MAX+1];
	int p=getpid();
	char o[2000];
	sprintf(o,"/proc/%d/exe",p);
	realpath(o,d);

	for(int i=0;i<strlen(d)-6;i++)
	{
		home[i]=d[i];
	}
	home[strlen(d)-6]='\0';
}

void username() {

	if ((user = getlogin()) == NULL)
		perror("getlogin() error");
}
char **getinput(char *s)
{
	char **cmd = malloc(10 * sizeof(char *));
	char *token=strtok(s," ");
	int ind=0;
	while(token!=NULL)
	{
		cmd[ind]=token;
		ind++;
		token=strtok(NULL," ");
	}
	cmd[ind]=NULL;
	return cmd;
}
struct bg
{
	int pid;
	char proc[1000];
	struct bg *next;
};
struct bg *root;
void delete(struct bg *d)
{
	if(d==root)
	{

		root=NULL;
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
			//printf("sfsfsafa\n");
			root=(struct bg *)malloc(sizeof(struct bg));
			root->next=(struct bg *)malloc(sizeof(struct bg));
			root->pid=pid;
			strcpy(root->proc,s);
			root->next->next=NULL;
			root->next->proc[0]=0;
			root->next->pid='\0';
			//printf("%d\n",root);
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
void forking(char *s,char *cmd)
{
	char **brr;
	char c[1000];
	strcpy(c,s);
	readcmd(c);
	behind(c);
	readcmd(s);
	behind(s);
	if(s[strlen(s)-1]=='&')
			c[strlen(c)-1]='\0';
	brr=getinput(c);
	//printf("%s\n%s\n%s\n",brr[0],brr[1],);
	readcmd(s);
	behind(s);
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		if(execvp(cmd,brr)==-1)
		{
			perror("Could not create process");
			exit(-1);
		}
		exit(0);
	}
	else
	{
		int d;
		if(s[strlen(s)-1]=='&')
			insert(pid,s);
		else
			waitpid(pid,&d,0);
	}
}

void concatdirectory(char *pwd)
{
	int j=0,i=0;
	while(i<strlen(pwd)&&i<strlen(home)&&home[i]==pwd[i])
	{
		i++;
	}
	if(i==strlen(home))
	{
		char s[100];
		i=strlen(home);
		s[j++]='~';
		while(i<strlen(pwd))
		{
			s[j++]=pwd[i++];
		}
		s[j]='\0';
		if(strlen(s)==0)
		{
			s[0]='~';
			s[1]='\0';
		}	
		strcpy(pwd,s);
	}	
}
void readcmd(char *cmd)
{
	int i=0;
	char s[100];
	while(cmd[i]==' ')
	{
		i++;
	}
	int j=0;
	while(cmd[i]!='\0')
	{
		s[j]=cmd[i];
		i++;
		j++;
	}
	s[j]='\0';
	strcpy(cmd,s);
}
void changemode(int dir)
{
	static struct termios oldt,newt;
	if(dir==1)
	{
		tcgetattr(STDIN_FILENO, &oldt);
		newt=oldt;
		newt.c_lflag&= ~(ICANON |ECHO);
		tcsetattr(STDIN_FILENO,TCSANOW,&newt);
	}
	else
	{
		tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
	}
}
int kbhit(void)
{
	struct timeval tv;
	fd_set rdfs;
	tv.tv_sec=0;
	tv.tv_usec=0;
	FD_ZERO(&rdfs);
	FD_SET (STDIN_FILENO, &rdfs);
	select(STDIN_FILENO+1,&rdfs,NULL,NULL,&tv);
	return FD_ISSET(STDIN_FILENO,&rdfs);
}
void behind(char *cmd)
{
	int i=strlen(cmd)-1;
	char s[100];
	while(cmd[i]==' ')
	{
		i--;
	}
	int j=0;
	while(j<=i)
	{
		s[j]=cmd[j];
		j++;
	}
	s[j]='\0';
	strcpy(cmd,s);
}
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
void permission(char *s,char *h)
{
	struct stat fileStat;
	stat(s,&fileStat);
	h[0]=( (S_ISDIR(fileStat.st_mode)) ? 'd' : '-');
	h[1]=( (fileStat.st_mode & S_IRUSR) ? 'r' : '-');
	h[2]=( (fileStat.st_mode & S_IWUSR) ? 'w' : '-');
	h[3]=( (fileStat.st_mode & S_IXUSR) ? 'x' : '-');
	h[4]=( (fileStat.st_mode & S_IRGRP) ? 'r' : '-');
	h[5]=( (fileStat.st_mode & S_IWGRP) ? 'w' : '-');
	h[6]=( (fileStat.st_mode & S_IXGRP) ? 'x' : '-');
	h[7]=( (fileStat.st_mode & S_IROTH) ? 'r' : '-');
	h[8]=( (fileStat.st_mode & S_IWOTH) ? 'w' : '-');
	h[9]=( (fileStat.st_mode & S_IXOTH) ? 'x' : '-');
	h[10]='\0';

}
int nlinks(char *s)
{
	struct stat fileStat;
	stat(s,&fileStat);
	return fileStat.st_nlink;
}
void uname(char *string,char *s)
{
	struct stat fileStat;
	stat(s,&fileStat);
	struct passwd *usr;
	usr=getpwuid(fileStat.st_uid);
	if(usr!=NULL)
	{
		strcpy(string,usr->pw_name);
	}
}
void groupname(char *string,char *s)
{
	struct stat fileStat;
	stat(s,&fileStat);
	struct group *usr;
	usr=getgrgid(fileStat.st_gid);
	if(usr!=NULL)
	{
		strcpy(string,usr->gr_name);
	}
}
int filesize(char *s)
{
	struct stat fileStat;
	stat(s,&fileStat);
	return fileStat.st_size;
}
void createtime(char *s,char *string)
{
	struct stat fileStat;
	stat(s,&fileStat);
	strcpy(string,ctime(&fileStat.st_mtime));
	string[strlen(string)-1]='\0';
}
void al(char *s,char *curr)
{
	DIR*p;
	struct dirent *directory;
	if(strlen(s)==0)
	{
		strcpy(s,curr);
	}
	p=opendir(s);
	if(p==NULL)
	{
		perror("Cannot find directory");
		exit(-1);
	}
	else
	{
		while(directory=readdir(p))
		{
			char string[100];
			permission(directory->d_name,string);
			printf("%s\t",string);
			printf("%d\t",nlinks(directory->d_name));
			uname(string,directory->d_name);
			printf("%s\t",string);
			groupname(string,directory->d_name);
			printf("%s\t",string);
			printf("%d\t",filesize(directory->d_name));
			createtime(directory->d_name,string);
			printf("%s\t",string);
			printf("%s",directory->d_name);
			printf("\n");
		}
	}
}
void lo(char *s,char *curr)
{
	DIR*p;
	struct dirent *directory;
	if(strlen(s)==0)
	{
		strcpy(s,curr);
	}
	p=opendir(s);
	if(p==NULL)
	{
		perror("Cannot find directory");
		exit(-1);
	}
	else
	{
		while(directory=readdir(p))
		{
			char string[100];
			//	printf("%s \tgsdfgsadsafdfsad\n",directory->d_name);
			if(directory->d_name[0]!='.')
			{
				permission(directory->d_name,string);
				printf("%s\t",string);
				printf("%d\t",nlinks(directory->d_name));
				uname(string,directory->d_name);
				printf("%s\t",string);
				groupname(string,directory->d_name);
				printf("%s\t",string);
				printf("%d\t",filesize(directory->d_name));
				createtime(directory->d_name,string);
				printf("%s\t",string);
				printf("%s",directory->d_name);

				printf("\n");
			}
		}
	}
}
void a(char *h,char *curr)
{

	DIR*p;
	struct dirent *directory;
	if(strlen(h)==0)
	{
		strcpy(h,curr);
	}
	p=opendir(h);
	if(p==NULL)
	{
		perror("Cannot find directory");
		exit(-1);
	}
	else
	{
		while(directory=readdir(p))
		{
			printf("%s\n",directory->d_name);
		}
	}
}
void l(char *h,char *curr)
{
	DIR*p;
	struct dirent *directory;
	if(strlen(h)==0)
	{
		strcpy(h,curr);
	}
	p=opendir(h);
	if(p==NULL)
	{
		perror("Cannot find directory");
		exit(-1);
	}
	else
	{
		while(directory=readdir(p))
		{
			if(directory->d_name[0]!='.')
				printf("%s\n",directory->d_name);
		}
	}
}
void ls(char *cmd)
{
	char curr[1000];
	getcwd(curr,1000);
	char s[1000];
	for(int i=2;i<strlen(cmd);i++)
	{
		s[i-2]=cmd[i];
	}
	s[strlen(cmd)-2]='\0';
	readcmd(s);
	behind(s);
	char f[1000];
	int j=0;
	int flag=0,flag2=0,flag3=0;
	int cnt=0,cnt2=0,cnt3=0;
	for(int i=0;i<strlen(s);i++)
	{
		if(s[i]=='-')
		{
			i++;
			while(i<strlen(s)&&s[i]=='a'||s[i]=='l'||s[i]==' '||s[i]=='-')
			{
				if(s[i]=='a')
				{flag++;}
				else if(s[i]=='l')
				{flag2++;}
				else if(s[i]==' ')
				{
					if(s[i-1]=='-')
					{
						cnt=1;
					}
				}
				else
				{
					flag3++;
					if(s[i-1]!=' ')
					{
						cnt=1;
					}
				}
				i++;
			}
		}
		f[j++]=s[i];
	}
	f[j]='\0';
	readcmd(f);
	behind(f);
	if(cnt==1)
	{
		printf("InvALID SYNTAX]\n");
	}
	else
	{
		if(flag==0&&flag2==0&&flag3<=2)
		{
			l(f,curr);	
		}
		else if(flag==1&&flag2==0&&flag3<=2)
		{
			a(f,curr);
		}
		else if(flag==0&&flag2==1&&flag3<=2)
		{
			lo(f,curr);	
		}
		else if(flag==1&&flag2==1&&flag3<=2)
		{
			al(f,curr);
			//printf("%s\n\n",f);
		}
		else
		{
			printf("Invalid Syntax\n");
		}
	}
}
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

void andbg(char *s)
{
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

void runshell()
{

	username();
	char hostname[1000];
	gethostname(hostname,1000);
	char s[1000];
	getcwd(s,1000);
	//printf("%s\n\n\n",s);
	concatdirectory(s);
	//printf("%s\n\n\n",s);
	printf("%s@%s:-%s/",user,hostname,s);
	char command[1000];
	command[0]='\0';
	scanf("%[^\n]",command);
	getchar();
	char *cmd=strtok(command,";");
	//printf("%d",strlen(command));
	if(strlen(command)!=0){
	while(cmd!=NULL)
	{
		readcmd(cmd);
		behind(cmd);
		char f[1000];
		strcpy(f,cmd);
		char *token=strtok(f," ");
		int z=0;
		while(token!=NULL)
		{


			if(z==0){
				z=1;
				if(strcmp(token,"echo")==0)
				{
					pid_t pid;
					pid=fork();
					if(pid==0)
					{
						if(cmd[strlen(cmd)-1]=='&')
						{
							cmd[strlen(cmd)-1]='\0';
							andbg(cmd);						
						}
						else
						echo(cmd);
							
						exit(0);
					}
					else
					{
						int d;
						if(cmd[strlen(cmd)-1]=='&')
							insert(pid,cmd);
						else
							waitpid(pid,&d,0);

					}
				}
				else if(strcmp(token,"ls")==0)
				{
					//printf("sdfasdfsd");
					pid_t pid;
					pid=fork();
					if(pid==0)
					{
						//printf("%c\n\n",cmd[strlen(cmd)-1]);
						
						if(cmd[strlen(cmd)-1]=='&')
						{
							cmd[strlen(cmd)-1]='\0';
							andbg(cmd);
						}
						else
						ls(cmd);
	
						exit(0);
					}
					else
					{
						int d;
						if(cmd[strlen(cmd)-1]=='&')
							insert(pid,cmd);
						else
							waitpid(pid,&d,0);

						//printf("%d\n",root);

					}
				}
				else if(strcmp(token,"pinfo")==0)
				{
					pid_t pid;
					pid=fork();
					if(pid==0)
					{
						if(cmd[strlen(cmd)-1]=='&')
						{
							pid_t pid;
							pid=fork();
							if(pid==0)
							{
								pinfo(cmd);
								exit(0);
							}
							else
							{}
						}
						else
						pinfo(cmd);

						exit(0);
					}
					else
					{
						int d;
						if(cmd[strlen(cmd)-1]=='&')
							insert(pid,cmd);
						else
							waitpid(pid,&d,0);

					}
				}

				else if(strcmp(token,"nightwatch")==0)
				{	
					pid_t pid;
					pid=fork();
					if(pid==0)
					{
						char s[1000];
						for(int i=10;i<strlen(cmd);i++)
						{
							s[i-10]=cmd[i];
						}
						s[strlen(cmd)-10]='\0';
						int p=strlen(cmd);
						if(cmd[p-1]=='y'&&cmd[p-2]=='t'&&cmd[p-3]=='r'&&cmd[p-4]=='i'&&cmd[p-5]=='d')
						{
							dirty(s);
							changemode(0);
						}
						else if(cmd[p-1]=='s'&&cmd[p-2]=='t'&&cmd[p-3]=='p'&&cmd[p-4]=='u'&&cmd[p-5]=='r'&&cmd[p-6]=='r'&&cmd[p-7]=='e'&&cmd[p-8]=='t'&&cmd[p-9]=='n'&&cmd[p-10]=='i')
						{
							nightwatch(s);
							changemode(0);						
						}
						else
						{

							printf("Invalid Syntax\n\n\n");
						}
						exit(0);
					}
					else
					{
						wait(NULL);
					}
				}
				else if(strcmp(token,"cd")==0)
				{
					//printf("%s\n\n",cmd);
					cd(cmd);
				}
				else if(strcmp(token,"pwd")==0)
				{
					char o[1000];
					getcwd(o,1000);
					printf("%s\n",o);
				}
				else
				{					
					forking(cmd,token);
				}
			}
			token=strtok(NULL," ");
		}
		int status;
	struct bg *temp=root;
	while(temp!=NULL&&temp->next!=NULL)
	{
		int s=waitpid(temp->pid,&status,WNOHANG);
		if(s==-1)
		{
			printf("Process %s with pid %d",temp->proc,temp->pid);
			psignal(WTERMSIG(status),"exited by\n");
			delete(temp);
		}
		else if(s>0)
		{
			printf("Process %s with pid %d exited normally\n",temp->proc,temp->pid);
			delete(temp);
		}
		if(temp->next!=NULL)
		{
			temp=temp->next;
		}
	}		
		cmd=strtok(NULL,";");
	}
	}
		
}
int main()
{
	gethome();
	while(1)
	{
		runshell();
	}
}
