#include "shell.h"
#include "libft.h"

int				tree_traversal(t_node *tree)
{
	if (tree && tree->left != NULL)
	{
		tree_traversal(tree->left);
		
		if (tree->type == CMD)
		{
			// cmd(tree);
		}
		else if (tree->type == PIPE)
		{
			// pipe(tree);
		}
		else if (tree->type == SEMI)
		{
			// semi(tree);
		}
		return (TRUE);
	}
	else
	{
		// peut plus descendre a gauche
		if (tree->type == CMD)
		{
			// cmd(tree);
		}
		else if (tree->type == RED)
		{
			// red(tree);
		}
		return (TRUE);
	}
	//	if (tree && tree->right != NULL)
	//	{
	//		tree_traversal(tree->right);
	//	}
	return (FALSE);
}
