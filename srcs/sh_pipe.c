#include <stdio.h>
#include <fcntl.h>
#include "shell.h"
#include "libft.h"

int					pfd_handler(int pipefd_tab[2][2])
{
	if (DEBUG == 1)
		ft_putendl_fd("------- PFD HANDLER ------", 2);

	if (pipefd_tab && pipefd_tab[0][0] < 0 && pipefd_tab[1][0] >= 0)
	{
		close(pipefd_tab[1][0]);
		if (dup2(pipefd_tab[1][1], STDOUT_FILENO) == ERROR)
			return (sh_error(FALSE, 7, NULL, NULL));
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] >= 0)
	{
		close(pipefd_tab[0][1]);
		if (dup2(pipefd_tab[0][0], STDIN_FILENO) == ERROR)
			return (sh_error(FALSE, 7, NULL, NULL));
		close(pipefd_tab[1][0]);
		if (dup2(pipefd_tab[1][1], STDOUT_FILENO) == ERROR)
			return (sh_error(FALSE, 7, NULL, NULL));
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] < 0)
	{
		close(pipefd_tab[0][1]);
		if (dup2(pipefd_tab[0][0], STDIN_FILENO) == ERROR)
			return (sh_error(FALSE, 7, NULL, NULL));
	}
	return (TRUE);
}

int					pfd_close(int pipefd_tab[2][2])
{
	if (pipefd_tab && pipefd_tab[0][0] >= 0)
	{
		close(pipefd_tab[0][0]);
		close(pipefd_tab[0][1]);
	}
	return (TRUE);
}

int					pipe_function(int pipefd_tab[2][2], t_node *tree,
					t_lst_fd **lstfd)
{
	if (DEBUG_PIPE == 1)
		ft_putendl_fd("------- PIPE FUNCTION -------", 2);

	int					pfd[2];

	if (pipefd_tab && pipefd_tab[1] && pipefd_tab[1][0] > 0)
	{
		pipefd_tab[0][0] = pipefd_tab[1][0];
		pipefd_tab[0][1] = pipefd_tab[1][1];
	}
	if (pipe(pfd) == ERROR)
		return (sh_error(FALSE, 4, NULL, NULL));
	pipefd_tab[1][0] = pfd[0];
	pipefd_tab[1][1] = pfd[1];
	if (tree->left->type == CMD)
		if ((tree_traversal(tree->left, lstfd, pipefd_tab)) == ERROR)
			return (ERROR);
	if (tree->right->type == CMD)
	{
		pipefd_tab[0][0] = pipefd_tab[1][0];
		pipefd_tab[0][1] = pipefd_tab[1][1];
		pipefd_tab[1][0] = -2;
		pipefd_tab[1][1] = -2;
	}
	return (TRUE);
}
