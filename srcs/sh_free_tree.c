#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int					clear_node(t_node **node)
{
	if (DEBUG_PARSER == 1)
		printf("------- CLEAR NODE ------\n");

	if (node && (*node) && !(*node)->left && !(*node)->right)
	{
		ft_strdel(&((*node)->data));
		(*node)->type = NONE;
		free(*node);
		*node = NULL;
		return (TRUE);
	}
	return (FALSE);
}

int					clear_tree(t_node **tree)
{
	if (DEBUG_PARSER == 1)
		printf("------- CLEAR TREE ------\n");

	//	printf("clear tree -- [%s]\n", (*tree)->data);
	if ((*tree) && (*tree)->left)
		clear_tree(&((*tree)->left));
	if ((*tree) && (*tree)->right)
		clear_tree(&((*tree)->right));
	clear_node(tree);
	return (FALSE);
}
