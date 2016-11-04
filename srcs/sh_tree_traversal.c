#include "shell.h"
#include "libft.h"

int				tree_traversal(t_node *tree)
{
	if (DEBUG_TREE == 1)
		printf("------- TREE TRAVERSAL -------(%d)\n", tree->type);
	int			saved_std[3];

	if (tree->type == PIPE)
	{
		saved_std[0] = dup(STDIN_FILENO);
		saved_std[1] = dup(STDOUT_FILENO);
		saved_std[2] = dup(STDERR_FILENO);
		pipe_function(tree, STDIN_FILENO);
		dup2(saved_std[0], STDIN_FILENO);
		dup2(saved_std[1], STDOUT_FILENO);
		dup2(saved_std[2], STDOUT_FILENO);
		close(saved_std[0]);
		close(saved_std[1]);
		close(saved_std[2]);
	}
	if (tree->type == SEMI)
	{
		if (tree->left)
			tree_traversal(tree->left);
		else
			return (FALSE);
		if (tree->right)
			tree_traversal(tree->right);
		else
			return (FALSE);
	}
	if (tree->type == CMD)
	{
		manage_cmd(tree);
	}

	return (TRUE);
}
