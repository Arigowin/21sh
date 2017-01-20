#include <stdio.h>
#include <fcntl.h>
#include "shell.h"
#include "libft.h"



int					pipe_function(int pipefd_tab[2][2], t_node *tree,
		t_global_fd **globalfd)
{
	if (DEBUG_PIPE == 1)
		ft_putendl_fd("------- PIPE FUNCTION -------", 2);

	int					pfd[2];

	dprintf(2, "tree->data :(%s)\t", tree->data);
	if (pipefd_tab && pipefd_tab[1] && pipefd_tab[1][0] > 0)
	{
		pipefd_tab[0][0] = pipefd_tab[1][0];
		pipefd_tab[0][1] = pipefd_tab[1][1];
	}
	if (pipe(pfd) == ERROR)
		return (ERROR);
	pipefd_tab[1][0] = pfd[0];
	pipefd_tab[1][1] = pfd[1];
	if (tree->left->type == CMD)
	{dprintf(2, "tree->left :(%s)\n", tree->left->data);
		if ((tree_traversal(tree->left, globalfd, pipefd_tab)) == ERROR)
			return (ERROR);
	}
	if (tree->right->type == CMD)
	{
		pipefd_tab[0][0] = pipefd_tab[1][0];
		pipefd_tab[0][1] = pipefd_tab[1][1];
		pipefd_tab[1][0] = -2;
		pipefd_tab[1][1] = -2;
	}
	return (TRUE);
}
