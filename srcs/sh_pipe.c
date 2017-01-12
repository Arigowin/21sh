#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int					pipe_function(int fd_in, int fd_out, t_node *tree,
					t_lst_fd **lstfd)
{
	if (DEBUG_PIPE == 1)
		printf("------- PIPE FUNCTION -------\n");

	int					pfd[2];

	if (tree->right == NULL || tree->right->type == CMD_ARG)
	{
		if (fd_in != STDIN_FILENO)
		{
			if (dup2(fd_in, STDIN_FILENO) == ERROR)
				return (ERROR);
		}
		if (handle_fork(fd_in, fd_out, tree, lstfd) == ERROR)
			return (ERROR);
	}
	else
	{
		if (pipe(pfd) == ERROR)
			return (ERROR);
		fd_out = pfd[1];
		if (handle_fork(fd_in, fd_out, tree->left, lstfd) == ERROR)
			return (ERROR);
		close(pfd[1]);
		fd_in = pfd[0];
		if (pipe_function(fd_in, fd_out, tree->right, lstfd) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}
