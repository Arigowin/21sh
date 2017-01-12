#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

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

	int						stat_loc;

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

int					son(int fd_in, int fd_out, char **cmd)
{
	if (DEBUG == 1)
		printf("------- SON ------\n");

	//	printf("WHY????????\n");
	//	if (check_builtin(tree, lstfd, cmd) == TRUE)
	//		return (TRUE);
	check_signal(2);
	if (fd_in != -1 && fd_out != -1)
	{
		if (fd_in != STDIN_FILENO)
		{
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		if (fd_out != STDOUT_FILENO)
		{
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
	}
	check_fct(cmd);
	// appeler la fonction d'erreur
	ft_putstr_fd("21sh: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);
	return (FALSE);
}

int					handle_fork(int fd_in, int fd_out, t_node *tree, t_lst_fd **lstfd)
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
		son(fd_in, fd_out, cmd);
	else
		father();
	init_term();
	return (TRUE);
}
