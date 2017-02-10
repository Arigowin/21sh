#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

//out = 0 => on continue - out = 1 => on quitte
int					sh_error(int ret_code, char *msg, int out)
{
	t_duo				*env;

	env = savior(NULL, FALSE);
	if (ret_code == TRUE)
		return (TRUE);
	else if (ret_code == FALSE)
	{
		if (msg != NULL)
			ft_putendl(msg);
		if (out == 1)
			bi_exit(NULL, &env);
		return(FALSE);
	}
	else if (ret_code == ERROR)
	{
		if (msg != NULL)
			ft_putendl(msg);
		if (out == 1)
			bi_exit(NULL, &env);
		return (ERROR);
	}
	return (TRUE);
}

/*
 * CD :
 * 	MESSAGES
 * 	- '21sh: cd: ' + ': not a directory' + 1 char * + return FALSE
 * 	- '21sh: cd: ' + ': no such file or directory' + char * + return FALSE
 * 	- '21sh: cd: ' + ': permission denied' + 1 char * + return FALSE
 * 	- '21sh: cd: no HOME variable set' + return FALSE
 * 	- '21sh: cd: no OLDPWD variable set'
 * 	FREE
 * 	- 1 char * + return TRUE
 * 	- 1 char * + return RET
 * 	- 2 char * + FALSE
 *
 * ENV :
 *	FREE
 * 	- 1 char * + return TRUE
 * 	- 1 char ** + return TRUE
 *
 * SETENV :
 * 	MESSAGES
 * 	- '21sh: setenv: ' + ': not a valid identifier' + 1 char * + return FALSE
 * 	- '21sh: setenv: too many arguments' + return FALSE
 *
 * UNSETENV :
 * 	MESSAGES
 * 	- '21sh: unsetenv: too few arguments' + return FALSE
 * 	- '21sh: unsetenv: ' + ': undefined variable' + 1 char * + return TRUE
 *
 * CMD
 * 	MESSAGES
 *	- '21sh:
 * 	FREE
 * 	- '21sh:
 *
 * EXIT (A FAIRE)!!!!!!!!
