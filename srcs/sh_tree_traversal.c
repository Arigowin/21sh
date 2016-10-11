#include "shell.h"
#include "libft.h"

int				tree_traversal(t_node *tree)
{
	if (tree && tree->left != NULL)
	{
		printf("------ IF -------\n");
		tree_traversal(tree->left);
		printf("========= IF depile ========\n");
		
		if (tree->type == CMD)
		{
			printf("CMD : [%s]\n", tree->data);
			// cmd(tree);
		}
		else if (tree->type == PIPE)
		{
			printf("PIPE\n");
			// pipe(tree);
		}
		else if (tree->type == SEMI)
		{
			printf("SEMI\n");
			// semi(tree);
		}
		else if (tree->type == RRED || tree->type == LRED || tree->type == DRRED || tree->type == DLRED)
		{
			printf("RED : [%s]\n", tree->data);
			red(tree);
		}
		return (TRUE);
	}
	else
	{
		printf("------- ELSE ------\n");
		// peut plus descendre a gauche
		if (tree->type == CMD)
		{
			printf("CMD : [%s]\n", tree->data);
			// cmd(tree);
		}
		else if (tree->type == RRED || tree->type == LRED || tree->type == DRRED || tree->type == DLRED)
		{
			printf("RED : [%s]\n", tree->data);
			red(tree);
		}
		return (TRUE);
	}
	//	if (tree && tree->right != NULL)
	//	{
	//		tree_traversal(tree->right);
	//	}
	return (FALSE);
}
