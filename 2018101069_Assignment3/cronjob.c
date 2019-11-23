#include <global.h>
//getting nth arg
char *ntharg(char *cmd, int n)
{
    char c[1000];
    strcpy(c, cmd);
    int cnt = 0;
    char *string = strtok(c, " ");
    while (string != NULL)
    {

        cnt++;
        string = strtok(NULL, " ");
        if (cnt == (n - 1))
        {
            // printf("%d\n\n\n\n",cnt);
            // printf("%s\n\n", string);
            readcmd(string);
            behind(string);
            return string;
        }
    }
    // return string;
} 
// executing cronjob by forking like bg process
void cronjob(char *cmd)
{
    if (nofargs(cmd) != 7)
    {
        printf("INvalid syntax");
    }
    else
    {
        // printf("%d\n\n", atoi(ntharg(cmd, 5)));
        char ff[1000];
        strcpy(ff, ntharg(cmd, 5));
        char tttt[1000];
        strcpy(tttt, ntharg(cmd, 7));
        int f = atoi(ff);
        int tt = atoi(tttt);
        char command[1000];
        strcpy(command, ntharg(cmd, 3));
        // printf("%d %d %s\n\n", f, tt, command);
        for (int i = 0; i < (tt / f); i++)
        {
            sleep(f);
            int pid;
            pid = fork();
            if (pid == 0)
            {
                execlp(command, command, NULL);
                exit(0);
            }
            else
            {
                // int d;
                // waitpid(pid, &d, WUNTRACED);
            }
        }
    }
}