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
char *user;
char home[100];
pid_t prevpid;
pid_t foregroundpid;
struct bg *root;
int histfile;
void jobs();
struct bg
{
	int pid;
	char proc[1000];
	struct bg *next;
};
void readcmd(char *cmd);
void behind(char *cmd);
void gethome();
void username();
char **getinput(char *s);
void  delete(struct bg *d);
void insert(int pid,char *s);
void forking(char *s,char *cmd);
void concatdirectory(char *pwd);
void changemode(int dir);
int kbhit(void);
void nightwatch(char *s);
void cd(char *command);
void echo(char *cmd);
void pinfo(char *s);
void permission(char *s,char *h);
int nlinks(char *s);
void uname(char *string,char *s);
void groupname(char *string,char *s);
int filesize(char *s);
void createtime(char *s,char *string);
void al(char *s,char *curr);
void lo(char *s,char *curr);
void a(char *h,char *curr);
void l(char *h,char *curr);
void ls(char *cmd);
void dirty(char *s);
void andbg(char *s);
void checkprocessstatus();
void runshell();
void execute(char *s,char *cmd,char **brr);
int  checkpiped(char *command);
void piper(char *command);
int nofargs(char *cmd);
char *ntharg(char *cmd,int n);
void cronjob(char *cmd);
void history(char *cmd);
void createfile();
char  *up(int n);