#include <global.h> 
//check if there is a pipe
int  checkpiped(char *command)
{
    int cnt=0;
    for(int i=0;i<strlen(command);i++)
    {
        if (command[i]=='|')
        {
            cnt++;
        }
    }
    return cnt;
}
//implementing pipe using pipe command
void piper(char *command)
{
    int cnt=checkpiped(command);
    char save[1000];
    strcpy(save,command);
	char *cmd=strtok(command,"|");
    int a[2], pid,in,out;
    in = dup(0);
    out = dup(1);   
    for (int i = 0; i <=cnt; i++)
    {
            if (i==0)
            {
                if (pipe(a) < 0) 
                perror("");
                out=dup(1);
                dup2(a[1],1);
                
            }
            else if (i==(cnt))
            {
                close(a[1]);
                in=dup(0);
                dup2(a[0], 0);
                close(a[0]);
            }
            else
            {
                close(a[1]);
                in=dup(0);                
                dup2(a[0], 0);
                close(a[0]);
                if (pipe(a) < 0) 
                perror("");
                out=dup(1);
                dup2(a[1],1);
            }
            pid=fork();
            if (pid==0)
            {
                char **brr;
                char sd[1000];
                readcmd(cmd);
                behind(cmd);
                strcpy(sd,cmd);
                brr=getinput(sd);
                execute(cmd,brr[0],brr);
                exit(0);                
            }
            else
            {
                int status;
                waitpid(pid, &status, WUNTRACED);
                dup2(in, 0);
                dup2(out, 1);
            }
	        cmd=strtok(NULL,"|");
        // }
    }   
}