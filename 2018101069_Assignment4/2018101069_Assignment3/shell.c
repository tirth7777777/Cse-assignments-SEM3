#include <global.h>
//Getting home name
void gethome()
{
	char d[PATH_MAX + 1];
	int p = getpid();
	char o[2000];
	sprintf(o, "/proc/%d/exe", p);
	realpath(o, d);

	for (int i = 0; i < strlen(d) - 6; i++)
	{
		home[i] = d[i];
	}
	home[strlen(d) - 6] = '\0';
}
// getting user name
void username()
{

	if ((user = getlogin()) == NULL)
		perror("getlogin() error");
}
// changing name of directory
void concatdirectory(char *pwd)
{
	int j = 0, i = 0;
	while (i < strlen(pwd) && i < strlen(home) && home[i] == pwd[i])
	{
		i++;
	}
	if (i == strlen(home))
	{
		char s[100];
		i = strlen(home);
		s[j++] = '~';
		while (i < strlen(pwd))
		{
			s[j++] = pwd[i++];
		}
		s[j] = '\0';
		if (strlen(s) == 0)
		{
			s[0] = '~';
			s[1] = '\0';
		}
		strcpy(pwd, s);
	}
}
// trimming
void readcmd(char *cmd)
{
	int i = 0;
	char s[100];
	while (cmd[i] == ' ')
	{
		i++;
	}
	int j = 0;
	while (cmd[i] != '\0')
	{
		s[j] = cmd[i];
		i++;
		j++;
	}
	s[j] = '\0';
	strcpy(cmd, s);
}
// trimming
void behind(char *cmd)
{
	int i = strlen(cmd) - 1;
	char s[100];
	while (cmd[i] == ' ')
	{
		i--;
	}
	int j = 0;
	while (j <= i)
	{
		s[j] = cmd[j];
		j++;
	}
	s[j] = '\0';
	strcpy(cmd, s);
}
// no of args
int nofargs(char *cmd)
{
	char c[1000];
	strcpy(c, cmd);
	int cnt = 0;
	char *token = strtok(c, " ");
	while (token != NULL)
	{
		token = strtok(NULL, " ");
		cnt++;
	}
	return cnt;
}
//not used function for first arg
char *firstarg(char *cmd)
{
	char c[1000];
	strcpy(c, cmd);
	// printf("%s\n\n\n", c);
	int cnt = 0;
	char *string = strtok(c, " ");
	while (string != NULL)
	{

		cnt++;
		string = strtok(NULL, " ");
		if (cnt == 1)
		{
			return string;
		}
	}
}
//not used funtion for sec arg
char *secarg(char *cmd)
{
	char c[1000];
	strcpy(c, cmd);
	int cnt = 0;
	char *string = strtok(c, " ");
	while (string != NULL)
	{

		cnt++;
		string = strtok(NULL, " ");
		if (cnt == 2)
		{
			return string;
		}
	}
}
// main function to run shell;
void runshell()
{
	username();
	char hostname[1000];
	gethostname(hostname, 1000);
	char s[1000];
	getcwd(s, 1000);
	concatdirectory(s);
	printf("%s@%s:-%s/", user, hostname, s);
	char command[1000];
	char a;
	int i = 0;
	for (int j = 0; j < 1000; j++)
	{
		command[j]='\0';
	}
	while (1)
	{
		a = getchar();
		if (a == EOF)
		{
			// command[i] = '\0';
			// i = 0;
			// break;
			printf("\n");
			return;
		}
		else if (a == '\n')
		{
			command[i] = '\0';
			i = 0;
			break;
		}
		else
		{
			command[i++] = a;
		}
	}
	readcmd(command);
	behind(command);
	int h = 0;
	for (int j = 0; j < strlen(command) - 2; j++)
	{
		if (command[j] == '\033' && command[j + 1] == '[' && command[j + 2] == 'A')
		{
			h++;
			j=j+2;
		}
	}
	// printf("%d n ups \n", h);
	if (3 * h != strlen(command)&&h>0)
	{
		printf("Invalid command\n");
		return;
	}
	else if(3*h ==strlen(command)&&h>0)
	{
		char string[1000];
		strcpy(string,up(h));
		printf("%s@%s:-%s/%s\n", user, hostname, s,string);		
		strcpy(command,string);
	}
	char hist[1000];
	strcpy(hist, command);
	history(hist);
	char *cmd = strtok(command, ";");
	if (strlen(command) != 0)
	{
		while (cmd != NULL)
		{
			readcmd(cmd);
			behind(cmd);
			char f[1000];
			strcpy(f, cmd);
			char *token = strtok(f, " ");
			int z = 0;
			int t = nofargs(cmd);
			while (token != NULL)
			{
				if (z == 0)
				{

					z = 1;
					//setenv code
					if (strcmp(token, "setenv") == 0)
					{
						pid_t pid;
						pid = fork();
						if (pid == 0)
						{
							if (t != 3)
							{
								printf("INvalid syntax");
							}
							else
							{
								char var[1000];
								strcpy(var, ntharg(cmd, 2));
								char val[1000];
								strcpy(val, ntharg(cmd, 3));
								if (getenv(var) == NULL)
								{
									putenv(var);
									setenv(var, val, 1);
								}
								else
								{
									setenv(var, val, 1);
								}
								char *check = getenv(var);
								printf("%s\n", check);
							}
							exit(0);
						}
						else
						{
							int d;

							waitpid(pid, &d, WUNTRACED);
						}
					}
					//unsetenv code
					else if (strcmp(token, "unsetenv") == 0)
					{
						pid_t pid;
						pid = fork();
						char var[1000];
						strcpy(var, ntharg(cmd, 2));
						if (pid == 0)
						{
							if (t != 2)
							{
								printf("INvalid syntax");
							}
							else
							{
								unsetenv(var);
							}
							exit(0);
						}
						else
						{
							int d;

							waitpid(pid, &d, WUNTRACED);
						}
					}
					// jobs function code
					else if (strcmp(token, "jobs") == 0)
					{
						pid_t pid;
						pid = fork();
						if (pid == 0)
						{
							jobs();
							exit(0);
						}
						else
						{
							int d;
							waitpid(pid, &d, WUNTRACED);
						}
					}
					// kjobs code
					else if (strcmp(token, "kjob") == 0)
					{
						pid_t pid;
						pid = fork();
						if (pid == 0)
						{
							if (t != 3)
							{
								printf("Invalid no of arguments\n");
							}
							else
							{
								char var[1000];
								strcpy(var, ntharg(cmd, 2));
								int fir = atoi(var);
								// printf("%d\n",fir);
								char val[1000];
								strcpy(val, ntharg(cmd, 3));
								int sec = atoi(val);
								int cnt = 1;
								struct bg *temp = root;
								while (cnt < fir && temp != NULL)
								{

									if (temp->next == NULL)
									{
										temp = temp->next;
									}
									else
									{
										break;
									}
									cnt++;
								}
								int pid;
								int status;
								if (cnt == fir)
								{
									kill(temp->pid, sec);
								}
								else
								{
									printf("NO such job number exist\n");
								}
							}
							exit(0);
						}
						else
						{
							int d;

							waitpid(pid, &d, WUNTRACED);
						}
					}
					// code to bring a process to foreground
					else if (strcmp(token, "fg") == 0)
					{
						int status;
						char fi[1000];
						strcpy(fi, ntharg(cmd, 2));
						int fir = atoi(fi);
						int cnt = 1;
						if (t != 2)
						{
							printf("Invalid no of arguments\n");
						}
						struct bg *temp = root;
						while (cnt < fir && temp != NULL)
						{
							if (temp->next != NULL)
							{
								temp = temp->next;
							}
							else
							{
								break;
							}
							cnt++;
							// printf("%d\n",cnt);
						}
						if (cnt == fir)
						{
							kill(temp->pid, SIGCONT);
							waitpid(temp->pid, &status, WUNTRACED);
						}
						else
						{
							printf("NO such job number exist\n");
						}
					}
					// bg function code
					else if (strcmp(token, "bg") == 0)
					{
						pid_t pid;
						pid = fork();
						if (pid == 0)
						{
							char fi[1000];
							strcpy(fi, ntharg(cmd, 2));
							int fir = atoi(fi);
							int cnt = 0;
							if (t != 2)
							{
								printf("Invalid no of arguments\n");
							}
							struct bg *temp = root;
							while (cnt < fir && temp != NULL)
							{
								temp = temp->next;
								cnt++;
							}
							if (cnt == fir)
							{
								kill(temp->pid, SIGCONT);
							}
							exit(0);
						}
						else
						{
							int d;
							waitpid(pid, &d, WUNTRACED);
						}
					}
					//overkill function code
					else if (strcmp(token, "overkill") == 0)
					{
						pid_t pid;
						pid = fork();
						if (pid == 0)
						{
							if (t != 1)
							{
								printf("Invalid no of arguments\n");
							}
							struct bg *temp = root;
							while (temp != NULL && temp->next != NULL)
							{
								kill(temp->pid, 9);
								temp = temp->next;
							}
							exit(0);
						}
						else
						{
							int d;
							waitpid(pid, &d, WUNTRACED);
						}
					}
					else if (strcmp(token, "quit") == 0)
					{
						exit(0);
					}
					//calling cronjob function
					else if (strcmp(token, "cronjob") == 0)
					{
						if (fork() == 0)
						{
							cronjob(cmd);
							exit(0);
						}
						else
						{
						}
					}
					//executing any command using exevp
					else
					{
						forking(cmd, token);
					}
				}
				token = strtok(NULL, " ");
			}
			checkprocessstatus();
			cmd = strtok(NULL, ";");
		}
	}
	return;
}
//ctrl c handler
void ctrlchanler()
{
	signal(SIGINT, ctrlchanler);
}
//ctrlz handler
void ctrlhandler()
{
	signal(SIGTSTP, ctrlhandler);
}
int main()
{
	createfile();
	signal(SIGINT, ctrlchanler);
	signal(SIGTSTP, ctrlhandler);
	gethome();
	while (1)
	{
		prevpid = -1;
		foregroundpid=-1;
		runshell();
	}
}
