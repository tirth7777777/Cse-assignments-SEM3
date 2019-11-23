#include <global.h>
// convert into form of execvp 
char **getinput(char *s)
{
	char **cmd = malloc(10 * sizeof(char *));
	char *token = strtok(s, " ");
	int ind = 0;
	while (token != NULL)
	{
		cmd[ind++] = token;
		token = strtok(NULL, " ");
	}
	cmd[ind] = NULL;
	return cmd;
}
// check bg process status 
void checkprocessstatus()
{
	int status;
	struct bg *temp = root;
	while (temp != NULL && temp->next != NULL)
	{
		int s = waitpid(temp->pid, &status, WNOHANG);
		if (s > 0)
		{
			if (WIFSIGNALED(status))
			{
				printf("Process %s with pid %d", temp->proc, temp->pid);
				printf(" exited by %d\n", WTERMSIG(status));
				delete (temp);
			}
			else if (WIFEXITED(status))
			{
				printf("Process %s with pid %d exited normally\n", temp->proc, temp->pid);
				delete (temp);
			}
		}
		if (temp->next != NULL)
		{
			temp = temp->next;
		}
	}
}
int first(char **cmdl)
{
	int i = 0;
	while (strcmp(cmdl[i], ">") != 0 && strcmp(cmdl[i], "<") != 0 && strcmp(cmdl[i], ">>") != 0)
	{
		i++;
	}
	return i;
}
char *fileshare(char **cmdl, int in)
{
	for (int i = 1; i < in; i++)
	{
		struct stat file;
		if (stat(cmdl[i], &file) == 0)
		{
			return cmdl[i];
		}
	}
	return cmdl[in + 1];
}
char *outputfile(char **cmdl, int out)
{
	return cmdl[out + 1];
}
void copy(char *s, char *new, int t, int j)
{
	for (int i = j; i < t; i++)
	{
		new[i - j] = s[i];
	}
	new[t - j] = 0;
}
int count(char *cmd)
{
	int cnt = 0;
	for (int i = 0; i < strlen(cmd); i++)
	{
		if (cmd[i] == '>' || cmd[i] == '<')
		{
			if (cmd[i] == '>' && i < (strlen(cmd) - 1) && cmd[i] == '>')
			{
				i++;
			}
			cnt++;
		}
	}
	return cnt;
}
int arraycnt(char *g)
{
	char s[1000];
	// printf("%s",g);
	strcpy(s, g);
	//  printf("%s",s);
	char *token = strtok(s, " ");
	int ind = 0;
	while (token != NULL)
	{
		// cmd[ind]=token;
		ind++;
		token = strtok(NULL, " ");
	}
	return ind;
}
//  code for redirection
void redirect(char *cmd)
{
	int n = arraycnt(cmd);
	char **cmdl = getinput(cmd);
	int in = 0;
	int out = 0;
	int flag = 0;
	for (int i = n - 1; i >= 0 && (out == 0 || in == 0); i--)
	{
		if (strcmp(cmdl[i], ">") == 0)
		{
			if (out == 0)
			{
				out = i;
				flag = 1;
			}
		}
		else if (strcmp(cmdl[i], "<") == 0)
		{
			in = i;
		}
		else if (strcmp(cmdl[i], ">>") == 0)
		{
			if (out == 0)
			{
				out = i;
				flag = 2;
			}
		}
	}
	int h = first(cmdl);
	char s[1000];
	s[0] = '\0';
	for (int i = 0; i < h; i++)
	{
		strcat(s, cmdl[i]);
		strcat(s, " ");
	}
	if (out == 0)
	{
		char *tmp = fileshare(cmdl, in);
		int file = open(tmp, O_RDONLY);
		char **cm;
		// printf("%s %s\n", s, tmp);
		dup2(file, 0);
		close(file);
		cm = getinput(s);
		// printf("%s\n",cm[0]);
		execvp(cm[0], cm);
	}
	else if (in == 0)
	{
		char *tmp = outputfile(cmdl, out);
		int file2;
		if (flag == 2)
			file2 = open(tmp, O_WRONLY | O_APPEND);
		else
		{
			file2 = open(tmp, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		}

		char **cm;
		dup2(file2, 1);
		close(file2);
		// printf("%s\n", s);
		cm = getinput(s);
		execvp(cm[0], cm);
	}
	else
	{
		char *tmp = fileshare(cmdl, in);
		char *tmp2 = outputfile(cmdl, out);
		// printf("%s %s\n",tmp,tmp2);
		int file = open(tmp, O_RDONLY);
		int file2;
		if (flag == 2)
			file2 = open(tmp2, O_WRONLY | O_APPEND);
		else
		{
			file2 = open(tmp2, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		}
		char **cm;
		dup2(file, 0);
		close(file);
		dup2(file2, 1);
		close(file2);
		cm = getinput(s);
		execvp(cm[0], cm);
	}
}
//  handler for c and z
int controlz = 0, ctrlc = 0;
void ctrlzchandler()
{
	controlz = 1;
	if (foregroundpid == getpid())
	{
		// printf("Killed %d\n",foregroundpid);
		kill(foregroundpid, SIGSTOP);
	}
	return;
}
void ctrlcler()
{
	ctrlc=1;
}
// execute any function
void execute(char *s, char *cmd, char **brr)
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		if (count(s) >= 1)
		{
			redirect(s);
			exit(0);
		}
		else
		{
			if (execvp(cmd, brr) == -1)
			{
				perror("Could not create process");
				exit(-1);
			}
		}
	}
	else
	{
		int d;
		foregroundpid = pid;
		prevpid = pid;
		signal(SIGTSTP, ctrlzchandler);
		signal(SIGINT, ctrlcler);
		if (s[strlen(s) - 1] == '&')
			insert(pid, s);
		else
		{
			waitpid(pid, &d, WUNTRACED);
			if (controlz == 1)
			{
				insert(pid, s);
				controlz = 0;
				struct bg *temp = root;
				while ( temp != NULL&&temp->next!=NULL)
				{
					if (temp->pid!=foregroundpid)
					{
					kill(temp->pid, SIGCONT);
					}
					temp = temp->next;
				}
			}
			if(ctrlc==1)
			{
				ctrlc=0;			
				kill(foregroundpid, 9);
			}
		}
	}
}
// forking of process
void forking(char *s, char *cmd)
{
	char **brr;
	char c[1000];
	strcpy(c, s);
	readcmd(c);
	behind(c);
	readcmd(s);
	behind(s);
	if (s[strlen(s) - 1] == '&')
		c[strlen(c) - 1] = '\0';
	brr = getinput(c);
	readcmd(s);
	behind(s);
	if (checkpiped(s))
	{
		piper(s);
	}
	else
	{
		execute(s, cmd, brr);
	}
}