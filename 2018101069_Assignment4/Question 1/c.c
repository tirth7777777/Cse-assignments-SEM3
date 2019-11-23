#include <stdio.h>
#include <stdlib.h>
int main()
{
	printf("10000\n");
	for(int i=0;i<10000;i++)
	{
		printf("%d\n",rand()%10000);
	}
}
