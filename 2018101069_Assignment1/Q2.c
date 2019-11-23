#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <string.h>


int reverse(char *old,char *new)
{
	int buff=1000000;
	int fp,f2;
	fp=open(old,O_RDONLY);
	f2=open(new,O_RDONLY);
	if(fp<0)
	{
		perror("r1");
		exit(0);
	}
	if(f2<0)
	{
		perror("r1");
		exit(0);
	}
	int k=lseek(fp,0,SEEK_END);
	int j=k/buff;
	long long int b,l,h;
	while(j--)
	{
		char c[1000000];
		char t[1000000];
		long long int i=lseek(fp,-1*buff,SEEK_CUR);
		read(fp,c,buff);
		lseek(fp,-1*buff,SEEK_CUR);
		int n = buff;
		for (int i = 0; i < n / 2; i++)
		{
			char a=c[i];
			char b=c[n-i-1];
			c[i]=b;
			c[n-i-1]=a;
		}
		read(f2,t,buff);
		if(strcmp(t,c)!=0)
		{
			return 0;
		}
	}
	char t[1000000];
	h=k%buff;
	int i=lseek(fp,-1*h,SEEK_CUR);
	char r[1000000];
	read(fp,r,h);
	lseek(fp,-1*h,SEEK_CUR);
	int n = h;
	for (int i = 0; i < n / 2; i++)
	{
		char a=r[i];
		char b=r[n-i-1];
		r[i]=b;
		r[n-i-1]=a;
	}
	read(f2,t,h);
	if (strcmp(t,r)!=0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
int main(int argc, char *argv[])
{
	if (argc!=4)
	{
		write(1,"Please print a valid file path for newfile,oldfile and directory\n",64);
		exit(0);
	}
	struct stat stats;
	struct stat stats2;
	struct stat stats3;
	int flag1=0,flag2=0,flag3=0;
	if (stat(argv[3],&stats3)==0)
	{
		stat(argv[3],&stats3);
		if( S_ISDIR(stats3.st_mode))
		{

			write(1,"Directory is created:Yes\n",strlen("Directory is created:Yes\n"));
		}
		else
		{
			flag3=1;
			write(1,"Directory is created:No\n",strlen("Directory is created:No\n"));
		}
	}
	else if (stat(argv[3],&stats3)!=0)
	{
		flag3=1;
		write(1,"Directory is created:No\n",strlen("Directory is created:No\n"));
	}
	if (reverse(argv[1],argv[2]))
	{
		write(1,"Whether file contents are reversed in newfile:Yes\n",strlen("Whether file contents are reversed in newfile:Yes\n"));
	}
	else
	{
		write(1,"Whether file contents are reversed in newfile:No\n",strlen("Whether file contents are reversed in newfile:No\n"));
	}
	if (stat(argv[1],&stats)==0)
	{
		stat(argv[1],&stats);
		if(stats.st_mode & S_IRUSR)
		{
			write(1,"User has Read permission on the newfile:Yes\n",strlen("User has Read permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"User has Read permission on the newfile:No\n",strlen("User has Read permission on the newfile:No\n"));
		}
		if(stats.st_mode & S_IWUSR)
		{
			write(1,"User has Write permission on the newfile:Yes\n",strlen("User has Write permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"User has Write permission on the newfile:No\n",strlen("User has Write permission on the newfile:No\n"));
		}
		if(stats.st_mode & S_IXUSR)
		{
			write(1,"User has Execute permission on the newfile:Yes\n",strlen("User has Execute permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"User has Execute permission on the newfile:No\n",strlen("User has Execute permission on the newfile:No\n"));
		}
		if(stats.st_mode & S_IRGRP)
		{
			write(1,"Group has Read permission on the newfile:Yes\n",strlen("Group has Read permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"Group has Read permission on the newfile:No\n",strlen("Group has Read permission on the newfile:No\n"));
		}
		if(stats.st_mode & S_IWGRP)
		{
			write(1,"Group has Write permission on the newfile:Yes\n",strlen("Group has Write permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"Group has Write permission on the newfile:No\n",strlen("Group has Write permission on the newfile:No\n"));
		}
		if(stats.st_mode & S_IXGRP)
		{
			write(1,"Group has Execute permission on the newfile:Yes\n",strlen("Group has Execute permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"Group has Execute permission on the newfile:No\n",strlen("Group has Execute permission on the newfile:No\n"));
		}
		if(stats.st_mode & S_IROTH)
		{
			write(1,"Other has Read permission on the newfile:Yes\n",strlen("Other has Read permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"Other has Read permission on the newfile:No\n",strlen("Other has Read permission on the newfile:No\n"));
		}
		if(stats.st_mode & S_IWOTH)
		{
			write(1,"Other has Write permission on the newfile:Yes\n",strlen("Other has Write permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"Other has Write permission on the newfile:No\n",strlen("Other has Write permission on the newfile:No\n"));
		}
		if(stats.st_mode & S_IXOTH)
		{
			write(1,"Other has Execute permission on the newfile:Yes\n",strlen("Other has Execute permission on the newfile:Yes\n"));
		}
		else
		{
			write(1,"Other has Execute permission on the newfile:No\n",strlen("Other has Execute permission on the newfile:No\n"));
		}
	}
	else
	{
		write(1,"\nNewfile does not exist\n",strlen("\nNewfile does not exist\n"));
	}
	if (stat(argv[2],&stats2)==0)
	{
		stat(argv[2],&stats2);
		if(stats2.st_mode & S_IRUSR)
		{
			write(1,"\nUser has Read permission on the oldfile:Yes\n",strlen("User has Read permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nUser has Read permission on the oldfile:No\n",strlen("\nUser has Read permission on the oldfile:No\n"));
		}
		if(stats2.st_mode & S_IWUSR)
		{
			write(1,"\nUser has Write permission on the oldfile:Yes\n",strlen("\nUser has Write permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nUser has Write permission on the oldfile:No\n",strlen("\nUser has Write permission on the oldfile:No\n"));
		}
		if(stats2.st_mode & S_IXUSR)
		{
			write(1,"\nUser has Execute permission on the oldfile:Yes\n",strlen("\nUser has Execute permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nUser has Execute permission on the oldfile:No\n",strlen("\nUser has Execute permission on the oldfile:No\n"));
		}
		if(stats2.st_mode & S_IRGRP)
		{
			write(1,"\nGroup has Read permission on the oldfile:Yes\n",strlen("\nGroup has Read permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nGroup has Read permission on the oldfile:No\n",strlen("\nGroup has Read permission on the oldfile:No\n"));
		}
		if(stats2.st_mode & S_IWGRP)
		{
			write(1,"\nGroup has Write permission on the oldfile:Yes\n",strlen("\nGroup has Write permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nGroup has Write permission on the oldfile:No\n",strlen("\nGroup has Write permission on the oldfile:No\n"));
		}
		if(stats2.st_mode & S_IXGRP)
		{
			write(1,"\nGroup has Execute permission on the oldfile:Yes\n",strlen("\nGroup has Execute permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nGroup has Execute permission on the oldfile:No\n",strlen("\nGroup has Execute permission on the oldfile:No\n"));
		}
		if(stats2.st_mode & S_IROTH)
		{
			write(1,"\nOther has Read permission on the oldfile:Yes\n",strlen("\nOther has Read permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nOther has Read permission on the oldfile:No\n",strlen("\nOther has Read permission on the oldfile:No\n"));
		}
		if(stats2.st_mode & S_IWOTH)
		{
			write(1,"\nOther has Write permission on the oldfile:Yes\n",strlen("\nOther has Write permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nOther has Write permission on the oldfile:No\n",strlen("\nOther has Write permission on the oldfile:No\n"));
		}
		if(stats2.st_mode & S_IXOTH)
		{
			write(1,"\nOther has Execute permission on the oldfile:Yes\n",strlen("\nOther has Execute permission on the oldfile:Yes\n"));
		}
		else
		{
			write(1,"\nOther has Execute permission on the oldfile:No\n",strlen("\nOther has Execute permission on the oldfile:No\n"));
		}
	}
	else
	{
		write(1,"\nOldfile does not exist\n",strlen("\nOldfile does not exist\n"));
	}
	if(stat(argv[3],&stats3)==0 & S_ISDIR(stats3.st_mode))
	{
		stat(argv[3],&stats3);
		if(stats3.st_mode & S_IRUSR)
		{
			write(1,"\nUser has Read permission on the directory:Yes\n",strlen("\nUser has Read permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nUser has Read permission on the directory:No\n",strlen("\nUser has Read permission on the directory:No\n"));
		}
		if(stats3.st_mode & S_IWUSR)
		{
			write(1,"\nUser has Write permission on the directory:Yes\n",strlen("\nUser has Write permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nUser has Write permission on the directory:No\n",strlen("\nUser has Write permission on the directory:No\n"));
		}
		if(stats3.st_mode & S_IXUSR)
		{
			write(1,"\nUser has Execute permission on the directory:Yes\n",strlen("\nUser has Execute permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nUser has Execute permission on the directory:No\n",strlen("\nUser has Execute permission on the directory:No\n"));
		}
		if(stats3.st_mode & S_IRGRP)
		{
			write(1,"\nGroup has Read permission on the directory:Yes\n",strlen("\nGroup has Read permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nGroup has Read permission on the directory:No\n",strlen("\nGroup has Read permission on the directory:No\n"));
		}
		if(stats3.st_mode & S_IWGRP)
		{
			write(1,"\nGroup has Write permission on the directory:Yes\n",strlen("\nGroup has Write permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nGroup has Write permission on the directory:No\n",strlen("\nGroup has Write permission on the directory:No\n"));
		}
		if(stats3.st_mode & S_IXGRP)
		{
			write(1,"\nGroup has Execute permission on the directory:Yes\n",strlen("\nGroup has Execute permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nGroup has Execute permission on the directory:No\n",strlen("\nGroup has Execute permission on the directory:No\n"));
		}
		if(stats3.st_mode & S_IROTH)
		{
			write(1,"\nOther has Read permission on the directory:Yes\n",strlen("\nOther has Read permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nOther has Read permission on the directory:No\n",strlen("\nOther has Read permission on the directory:No\n"));
		}
		if(stats3.st_mode & S_IWOTH)
		{
			write(1,"\nOther has Write permission on the directory:Yes\n",strlen("\nOther has Write permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nOther has Write permission on the directory:No\n",strlen("\nOther has Write permission on the directory:No\n"));
		}
		if(stats3.st_mode & S_IXOTH)
		{
			write(1,"\nOther has Execute permission on the directory:Yes\n",strlen("\nOther has Execute permission on the directory:Yes\n"));
		}
		else
		{
			write(1,"\nOther has Execute permission on the directory:No\n",strlen("\nOther has Execute permission on the directory:No\n"));
		}
	}
	else
	{
		write(1,"\nDirectory does not exist\n",strlen("\nDirectory does not exist\n"));
	}
	return 0;
}
