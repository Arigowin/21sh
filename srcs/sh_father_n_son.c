#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

#include <errno.h>
#include <string.h>

// virer tree et lstfd ???
int					check_builtin(char **cmd, int fd_in, int fd_out)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK BUILTIN ------", 2);

	int					ret;

	ret = -1;
	if (is_builtin(cmd) != -1)
	{
		if (fd_in != -1 && fd_out != -1)
		{
	ft_putendl_fd("fd_in", 2);
	ft_putnbr_fd(fd_in, 2);
	ft_putendl_fd("", 2);
	ft_putendl_fd("fd_out", 2);
	ft_putnbr_fd(fd_out, 2);
	ft_putendl_fd("", 2);
			if (fd_out != STDOUT_FILENO)
			{
				ft_putendl_fd("TATA2", 2);
				if (dup2(fd_out, STDOUT_FILENO) == ERROR)
					ft_putendl_fd("ERROR4", 2);
				close(fd_out);
			}
			if (fd_in != STDIN_FILENO)
			{
				ft_putendl_fd("TATA1", 2);
				if (dup2(fd_in, STDIN_FILENO) == ERROR)
					ft_putendl_fd("ERROR3", 2);
				close(fd_in);
			}
		}
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
		ft_putendl_fd("------- FATHER ------", 2);

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
		ft_putendl_fd("------- SON ------", 2);

	//	ft_putendl_fd("WHY????????\n");
	//	if (check_builtin(tree, lstfd, cmd) == TRUE)
	//		return (TRUE);
	check_signal(2);
	if (fd_in != -1 && fd_out != -1)
	{
	ft_putendl_fd("fd_in", 2);
	ft_putnbr_fd(fd_in, 2);
	ft_putendl_fd("", 2);
	ft_putendl_fd("fd_out", 2);
	ft_putnbr_fd(fd_out, 2);
	ft_putendl_fd("", 2);

		if (fd_out != STDOUT_FILENO)
		{
			ft_putendl_fd("TITI2", 2);
			if (dup2(fd_out, STDOUT_FILENO) == ERROR)
				ft_putendl_fd("ERROR2", 2);
	//		perror(strerror(errno));
			close(fd_out);
		}
		if (fd_in != STDIN_FILENO)
		{
			ft_putendl_fd("TITI1", 2);
			if (dup2(fd_in, STDIN_FILENO) == ERROR)
				ft_putendl_fd("ERROR1", 2);
			close(fd_in);
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
		ft_putendl_fd("------- HANDLE FORK ------", 2);

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
	if ((ret = check_builtin(cmd, fd_in, fd_out)) == TRUE)
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
