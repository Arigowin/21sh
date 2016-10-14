#include "shell.h"
#include "libft.h"

int				tree_traversal(t_node *tree)
{
	if (tree->type == PIPE)
	{
		pipe_function(tree->left, STDIN_FILENO);
	}
	if (tree->type == SEMI)
	{
		tree_traversal(tree->left);
		tree_traversal(tree->right);
	}
	if (tree->type == CMD)
	{
		manage_cmd(tree);
	}

	return (TRUE);
}
