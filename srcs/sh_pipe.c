#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"
#include <sys/wait.h>

int					pipe_function(t_node *tree, int in_fd, t_lst_fd **lstfd)
{
	if (DEBUG_PIPE == 1)
		printf("------- PIPE FUNCTION -------(%s)-(%d)\n", tree->data, in_fd);
	int					pfd[2];
	int					pid;
	int					stat_lock;

	if (tree->right == NULL || tree->right->type == CMD_ARG)
	{
		if (in_fd != STDIN_FILENO)
		{
			if (dup2(in_fd, STDIN_FILENO) != -1)
				close(in_fd);
			else
				return (FALSE);
		}
		manage_cmd(tree, lstfd);
		return (FALSE);
	}
	else
	{
		if (pipe(pfd) == -1 || (pid = fork()) == -1)
			return (FALSE);
		if (pid > 0)
			wait(&stat_lock);
		if (pid == 0)
		{
			close(pfd[0]);
			if (dup2(in_fd, STDIN_FILENO) == -1)
				return (FALSE);
			if (dup2(pfd[1], STDOUT_FILENO) == -1)
				return (FALSE);
			else
			{
				father_n_son_for_pipe(format_cmd(tree->left));
				return (FALSE);
			}
		}
		close(pfd[1]);
		close(in_fd);
		pipe_function(tree->right, pfd[0], lstfd);
	}
	return (TRUE);
}
