#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

const char			*tbl_error1(int index)
{
	static const char	*err_tbl1[] = {/*0*/"21sh: cannot access termacp database",
						/*1*/"21sh: ioctl: cannot get window size",
						/*2*/"21sh: cannot open ",/*3*/ "21sh: cannot open fd",
						/*4*/"21sh: cannot performe pipe function",
						/*5*/"21sh: cannot performe fork function",
						/*6*/"21sh: memory allocation failed",
						/*7*/"21sh: cannot duplicate fd",
						/*8*/"21sh: bad file descriptor",
						/*9*/"21sh: unsetenv: too few arguments",
						/*10*/"21sh: setenv: too many arguments",
						/*11*/"21sh: cd: no OLDPWD variable set",
						/*12*/"21sh: no PATH variable set",
						/*13*/"21sh: cd: no HOME variable set",
						/*14*/"21sh: unsetenv", /*15*/"21sh: setenv",
	  					/*16 -> 16, 17, 18*/"21sh : cd: ",/*17->19, 20, 21, 22, 23, 25, 26, 29, 30*/ "21sh",
						/*18->26*/"21sh: parse error near",
						/*19->27, 28*/"21sh: exit "
						};
	int					ret_index;

	ret_index = index;
	ret_index = (index >= 16 && index <= 18 ? 16 : ret_index);
	ret_index = (index >= 26 ? 18 : ret_index);
	ret_index = ((index >= 19 && index <= 25) || index == 29  || index == 30 ? 17 : ret_index);
	ret_index = (index >= 27 && index <= 28 ? 19 : ret_index);
	return (err_tbl1[ret_index]);
}

const char			*tbl_error2(int index)
{
	static const char	*err_tbl2[] = {/*0->14*/": undefined variable",
						/*1->15*/": not a valid identifier",
						/*2->16, 23*/": not a directory",
						/*3->17, 21*/": no such file or directory",
						/*4->18, 19, 20*/": permission denied", /*5->22*/": invalid option",
						/*6->24*/": command not found",
						/*7->26*/": value not set", // : missing name for redirect",
						/*8->27*/": too many arguments",
						/*9->28*/": numeric argument required",
						/*10->29*/": bad file descriptor",
						/*11->30*/": ambiguous redirect"
						};
	int					ret_index;

	ret_index = 0;
	ret_index = (index == 15 ? 1 : ret_index);
	ret_index = (index == 16 || index == 23 ? 2 : ret_index);
	ret_index = (index == 17 || index == 21 ? 3 : ret_index);
	ret_index = (index >= 18 && index <= 20 ? 4 : ret_index);
	ret_index = (index == 22 ? 5 : ret_index);
	ret_index = (index == 24 ? 6 : ret_index);
	ret_index = (index == 25 ? 7 : ret_index);
	ret_index = (index == 27 ? 8 : ret_index);
	ret_index = (index == 28 ? 9 : ret_index);
	ret_index = (index == 29 ? 10 : ret_index);
	ret_index = (index == 30 ? 11 : ret_index);
	return (err_tbl2[ret_index]);
}

int					sh_error(int ret, int index, char *err, char *bi)
{
	if (ret == NO_PRINT)
		return (NO_PRINT);
	ft_putstr_fd(tbl_error1(index), 2);
	if (bi != NULL)
	{
		ft_putstr_fd(bi, 2);
		ft_putstr_fd(": ", 2);
	}
	if (err != NULL)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(err, 2);
	}
	if (index >= 14 && index <= 30 && index != 26)
		ft_putstr_fd(tbl_error2(index), 2);
	ft_putendl_fd("", 2);
	if (index <= 7 || index == 23 || index == 24)
		exit_pgm(EXIT_FAILURE);
	if (index == 11 || index == 14)
		return (TRUE);
	if (index == 19 || index == 22)
		return (-2);
	if (index == 26)
		return (NO_PRINT);
	if (index == 25)
		return (ERROR);
	else
		return (FALSE);
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
 *	- '21sh: bad file descriptor' + FREE lstfd + close? + FALSE
 *
 * CMD LINE ASSEMBLE
 * 	MESSAGES
 * 	- '21sh: cannnot performe pipe function' + exit FAILURE
 * 	- '21sh: cannnot duplicate fd: ' + int fd + exit FAILURE
 * 	- '21sh: no PATH variable set' + exit FAILURE
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
 * 	- 1 l_expr * + '21sh: memory allocation failed' + exit FAILURE
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
 * 	- '21sh: parse error near: ' + 1 char * + ': missing name for redirect' + exit FAILURE // fct parse error
 *	FREE
 * 	- 1 t_node * + '21sh: memory allocation failed' + exit FAILURE
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
 * 	- '21sh: memory allocation failed' + exit FAILURE + free saviors
 *
 *
 * EXIT (A FAIRE)!!!!!!!!
 *
 */
