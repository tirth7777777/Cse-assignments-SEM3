#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <string.h>
char c[1000001];
int main(int argc, char **argv)
{
	if (argc!=2)
	{
		write(1,"Please print a valid file path\n",32);
		exit(0);
	}
	int fp,f2;
	char g[1000000];
	int buff=1000000;
	fp = open(argv[1], O_RDONLY);
	if(fp<0)
	{
		perror("r1");
		exit(0);
	}
	mkdir("Assignment",0700);
	char d[10000]="Assignment/";
	strcat(d,argv[1]);
	creat(d,0700);
	f2 = open(d,O_RDWR);
	if(f2<0)
	{
		perror("r1");
		exit(0);
	}
	int k=lseek(fp,0,SEEK_END);
	// printf("%d\n",k);
	int j=k/buff;
	long long int b,l,h;
	while(j--)
	{
		char t[1000000];
		long long int i=lseek(fp,-1*buff,SEEK_CUR);
		b=(100-(i*100)/k);
		// printf("%lld\n",b);
		sprintf(t,"\r%lld",b);
		write(1,t,strlen(t));
		read(fp,c,buff);
		// printf("%s in while loop \n", c);
		lseek(fp,-1*buff,SEEK_CUR);
		int n = buff;
    		for (int i = 0; i < n / 2; i++)
    		{
        		char a=c[i];
        		char b=c[n-i-1];
        		c[i]=b;
        		c[n-i-1]=a;
   	 		}
		write(f2,c,buff);
	}
	char t[1000];
	h=k%buff;
	int i=lseek(fp,-1*h,SEEK_CUR);
	b=(100-(i*100)/k);
	// pintf("%d\n",b);

	sprintf(t,"\r%lld",b);
	write(1,t,strlen(t));
	char r[1000001];
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
	write(f2,r,h);
	close(fp);
	close(f2);
}
