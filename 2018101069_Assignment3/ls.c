#include <global.h>
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