#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"


// regrouper ac father_and_son_for_pipe en rajoutant un boolean dans le prototype
// + demander explication pour for_pipe
int					father_n_son(char **cmd)
{
	if (DEBUG == 1)
		printf("------- FATHER N SON ------\n");

	pid_t				father;
	int					stat_loc;

	if ((father = fork()) < 0)
		return (ERROR);
	if (father > 0)
	{
		check_signal(3);
		wait(&stat_loc);
	}
	if (father == 0)
	{
		check_signal(2);
		check_fct(cmd);
		// appeler la fonction d'erreur
		ft_putstr_fd("21sh: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(EXIT_FAILURE);
	}
	return (TRUE);
}

// inclure cette fct dans handle_fork ???? c'est exactement ce qui se passe dans son, sauf pour l'appel a signal (qui doit manquer ici...)
int					father_n_son_for_pipe(char **cmd)
{
	if (DEBUG == 1)
		printf("------- FATHER N SON FOR PIPE ------\n");

	check_fct(cmd);
	ft_putstr_fd("21sh: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);

	return (0);
}

// virer tree et lstfd ???
int					check_builtin(t_node *tree, t_lst_fd **lstfd, char **cmd)
{
	if (DEBUG == 1)
		printf("------- CHECK BUILTIN ------\n");

	int					ret;

	(void)tree;
	(void)lstfd;
	ret = -1;
	if (is_builtin(cmd) != -1)
	{
		if ((ret = handle_builtin(cmd)) == ERROR)
		{
			//close_lstfd(lstfd);
			return (ERROR);
		}
		return (TRUE);
	}
	return (FALSE);
}

int					father(void)
{
	if (DEBUG == 1)
		printf("------- FATHER ------\n");

	int					stat_loc;

	stat_loc = 0;
	check_signal(3);
	while (waitpid(-1, &stat_loc, WNOHANG) >= 0)
		;
	if (WIFSIGNALED(stat_loc))
		return (-2); // revoir le code de retour
	if (WIFEXITED(stat_loc) != TRUE)
		return (ERROR);
	return (WEXITSTATUS(stat_loc));
}

int					son(char **cmd) //t_node *tree, t_lst_fd **lstfd, char **cmd)
{
	if (DEBUG == 1)
		printf("------- SON ------\n");

	//	printf("WHY????????\n");
	//	if (check_builtin(tree, lstfd, cmd) == TRUE)
	//		return (TRUE);
	check_signal(2);
	check_fct(cmd);
	// appeler la fonction d'erreur
	ft_putstr_fd("21sh: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);
	return (FALSE);
}

int					handle_fork(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG == 1)
		printf("------- HANDLE FORK ------\n");

	pid_t				fpid;
	char				**cmd;
	int					ret;

	fpid = -1;
	cmd = NULL;
	ret = 0;
	if ((cmd = format_cmd(tree)) == NULL)
		return (ERROR);
	if (tree->left != NULL)
	{
		if (redirect(tree->left, lstfd) == ERROR)
		{
			printf("vFUGEPIUFGEVHFVEHPFVEFPEZVFEZHFVPEZYFUVHZ\n");
			//			close_fd_red(&lstfd, saved_std);
			return (ERROR);
		}
	}
	if ((ret = check_builtin(tree, lstfd, cmd)) == TRUE)
		return (TRUE);
	if (ret == ERROR)
		return (ERROR);
	if ((fpid = fork()) < 0)
		return (ERROR);
	reset_term();
	if (fpid == 0)
		son(cmd);
	//	son(tree, lstfd, cmd);
	else
		father();
	init_term();
	return (TRUE);
}
