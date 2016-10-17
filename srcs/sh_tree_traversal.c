#include "shell.h"
#include "libft.h"

int				tree_traversal(t_node *tree)
{
	int			saved_stdout;
	int			saved_stdin;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);

	if (DEBUG_TREE == 1)
		printf("------- TREE TRAVERSAL -------(%d)\n", tree->type);
	if (tree->type == PIPE)
	{
		pipe_function(tree, STDIN_FILENO);
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

	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);

	return (TRUE);
}
