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
 * OPTION :
 *	MESSAGE
 *	- '21sh: ' + ': ' + ': invalid option' + 2 char * + return ERROR (a vérifier)
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
 *	- '21sh: bad file descriptor' + FREE lstfd + close?
 *
 * CMD LINE ASSEMBLE
 * 	MESSAGES
 * 	- '21sh: cannnot performe pipe function' + exit FAILURE
 * 	- '21sh: cannnot duplicate fd: ' + int fd + exit FAILURE
 * 	- '21sh: cd: no PATH variable set' + exit FAILURE
 * 	- '21sh: memory allocation failed'
 * 	- '21sh: ' + ': permission denied' + 1 char * + return -2 (important!!!!)
 * 	FREE
 * 	- 2 char ** + return FALSE + message 'command not found' ici?
 *
 * FATHER N SON
 * 	MESSAGES
 * 	- '21sh: ' + ': command not found' + 1 char * + exit FAILURE
 * 	- '21sh: cannot perform fork function' + exit FAILURE
 *
 * FCT READ
 *	MESSAGES
 *	- '21sh: cannot open fd' + exit FAILURE
 * 	- '21sh: cannnot duplicate fd: ' + int fd + exit FAILURE
 *	FREE
 *	- 1 t_e_list + return RET
 *	- 1 t_node (tree//savior) + return RET
 *	- 1 t_lstfd (lstfd//savior) + return RET
 *
 * FILE HISTORY
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE
 *	FREE
 * 	- 1 char * + '21sh: memory allocation failed' + exit FAILURE
 * 	- 2 char * + exit FAILURE
 * 	- 1 char * + exit FAILURE
 * 	- 2 char * + '21sh : cannot open ' + 1 char *(path -> on le passe parmis les 2 précédents) + exit FAILURE
 * 	- 1 char * + '21sh : cannot open ' + 1 char *(path -> on le passe parmis les 1 précédents) + exit FAILURE
 * 	- 1 t_history (in savior) + return TRUE
 *
 * FIRST STEPS
 * 	FREE
 * 	- 2 char * + return TRUE
 * 	- 1 char * + return TRUE
 *
 * INIT
 * 	FREE
 * 	- 1 char ** + '21sh: memory allocation failed' + exit FAILURE
 * 	- '21sh: memory allocation failed' + exit FAILURE
 * 	- '21sh: ioctl: cannot get window size' + exit FAILURE
 * 	- 1 char * + '21sh: memory allocation failed' + exit FAILURE
 *
 * LEXER
 * 	FREE
 * 	- 1 l_expr * (savior)  + '21sh: memory allocation failed' + exit FAILURE
 * 	- 1 char * + '21sh: memory allocation failed' + exit FAILURE
 * 	- 1 char * + return TRUE
 *
 * PARSER    // parse error = EXIT!!!!!!!!
 * 	MESSAGES
 * 	- '21sh: parse error near: ' + 1 char * + exit FAILURE // fct parse error
 *	FREE
 * 	- 1 t_node * + '21sh: memory allocation failed' + exit FAILURE
 * 	- 1 t_node * + return TRUE
 * 	- '21sh: parse error near: ' + 1 char * (msg) + 1 t_node * (free)+ exit FAILURE // fct parse error
 *
 * PARSER ADDITIONAL ITEMS    // parse error = EXIT!!!!!!!!
 * 	MESSAGES
 * 	- '21sh: parse error near: ' + 1 char * + exit FAILURE // fct parse error
 *	FREE
 * 	- 1 t_node * + '21sh: memory allocation failed' + exit FAILURE
 * 	- '21sh: parse error near: ' + 1 char * + ': missing name for redirect' + exit FAILURE // fct parse error
 *
 * TOKENIZER
 * 	FREE
 * 	- '21sh: memory allocation failed' + exit FAILURE
 *
 * TOKENIZER SPEC
 * 	FREE
 * 	- '21sh: memory allocation failed' + exit FAILURE
 * 	- 1 char * + return FALSE (a vérif ce qu'on fait dans get_env, si dup -> exit failure)
 * 	- 1 char * + '21sh: memory allocation failed' + exit FAILURE
 * 	- 2 char * + '21sh: memory allocation failed' + exit FAILURE
 * 	- 2 char * + return TRUE
 *
 * MAIN
 * 	FREE
 * 	- 1 char * + exit SUCCESS
 * 	- 1 char * + exit FAILURE
 * 	- 2 char * + exit SUCCESS ? (return 0 in main)
 *
 * PIPE
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE
 *
 * PIPE HANDLER
 * 	MESSAGES
 * 	- '21sh: cannnot duplicate fd: ' + exit FAILURE
 *
 * HEREDOC
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE
 *
 * MANAGE LSTFD
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE
 *
 * HIGHLIGHT
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE
 *
 * HISTORY
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE
 *
 * INSERT IN HISTORY
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE + free saviors
 *
 * TERMCAP
 * 	MESSAGES
 * 	- '21sh: Could not access the termcap database' + exit FAILURE
 *
 * TELIST HANDLER
 * 	FREE
 * 	- 1 char * + return NULL _> ds fct erreur ?
 *
 * TREE FUNCTION
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE
 *
 * TREE TRAVERSAL
 * 	MESSAGES
 * 	- '21sh: memory allocation failed' + exit FAILURE
 * 	- '21sh: ' + ': no such file or directory'+ 1 char * + return FALSE
 * 	- '21sh: ' + ': permission denied'+ 1 char * + return FALSE
 * 	FREE
 * 	- '21sh: memory allocation failed' + exit FAILURE + free saviors
 *
 *
 * EXIT (A FAIRE)!!!!!!!!
 *
 */
