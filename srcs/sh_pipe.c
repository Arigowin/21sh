#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include "shell.h"
#include "libft.h"

int					pipe_function(int fd_in, int fd_out, t_node *tree,
					t_lst_fd **lstfd)
{
	if (DEBUG_PIPE == 1)
		ft_putendl_fd("------- PIPE FUNCTION -------", 2);

	int					pfd[2];
	int					fd;

	if (tree->right == NULL || tree->right->type == CMD_ARG)
	{
		ft_putendl_fd("TOTO1", 2);
	ft_putendl_fd("fd_in", 2);
	ft_putnbr_fd(fd_in, 2);
	ft_putendl_fd("", 2);
	ft_putendl_fd("fd_out", 2);
	ft_putnbr_fd(fd_out, 2);
	ft_putendl_fd("", 2);
		if (fd_in != STDIN_FILENO)
		{
			if (dup2(fd_in, STDIN_FILENO) == ERROR)
				return (ERROR);
		}
		if (fd_out != STDOUT_FILENO)
		{
			if ((fd_out = open(savior_tty(NULL, FALSE), O_RDWR)) == -1)
				return (FALSE);
			if (dup2(fd_out, STDOUT_FILENO) == -1)
				return (FALSE);
			if (fd_out > STDERR_FILENO)
				close(fd_out);
		}
		if (handle_fork(fd_in, fd_out, tree, lstfd) == ERROR)
			return (ERROR);
	}
	else
	{
		ft_putendl_fd("TOTO3", 2);
		if (pipe(pfd) == ERROR)
			return (ERROR);
	ft_putendl_fd("pfd1", 2);
	ft_putnbr_fd(pfd[1], 2);
	ft_putendl_fd("", 2);
	ft_putendl_fd("pfd0", 2);
	ft_putnbr_fd(pfd[0], 2);
	ft_putendl_fd("", 2);
	ft_putendl_fd("fd_in", 2);
	ft_putnbr_fd(fd_in, 2);
	ft_putendl_fd("", 2);
	ft_putendl_fd("fd_out", 2);
	ft_putnbr_fd(fd_out, 2);
	ft_putendl_fd("", 2);

		fd_out = pfd[1];
		if (handle_fork(fd_in, fd_out, tree->left, lstfd) == ERROR)
			return (ERROR);

		close(pfd[1]);
		if ((fd = open(savior_tty(NULL, FALSE), O_RDWR)) == -1)
			return (FALSE);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (FALSE);
		if (fd > STDERR_FILENO)
			close(fd);
		fd_in = pfd[0];

		if (pipe_function(fd_in, fd_out, tree->right, lstfd) == ERROR)
			return (ERROR);
	}
	close(pfd[0]);
	close(pfd[1]);
	return (TRUE);
}
