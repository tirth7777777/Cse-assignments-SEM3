#include <global.h>
// cfreating hist file
void createfile()
{
    histfile = open("history", O_RDWR | O_CREAT, 0777);
    // perror("");
}
// adding to file
void history(char *cmd)
{
    sprintf(cmd, "%s\n", cmd);
    lseek(histfile, 0, SEEK_END);
    write(histfile, cmd, strlen(cmd));
}
//  getting command to execute
char *up(int n)
{
    int cnt = 0;
    char c[100000];
    lseek(histfile, 0, SEEK_SET);
    read(histfile, c, 100000);
    char *y = strtok(c, "\n");
    while (y != NULL)
    {
        cnt++;
        y = strtok(NULL, "\n");
    }
    // printf("%d nooflines\n\n", cnt);
    int ef = 0;
    lseek(histfile, 0, SEEK_SET);
    read(histfile, c, 100000);
    y = strtok(c, "\n");
    n = cnt - n;
    while (ef < (n) && y != NULL)
    {
        ef++;
        y = strtok(NULL, "\n");
    }
    // printf("%d ef\n", ef);
    // printf("%s\n\n\n", y);
    return y;
    // int fd = 0;
}