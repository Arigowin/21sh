#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include "libft.h"

t_node				*create_node(types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- CREATE NODE ------", 2);
	t_node				*new;

	if ((new = (t_node *)malloc(sizeof(t_node))) == NULL)
		return (NULL);
	new->data = NULL;
	new->type = type;
	new->left = NULL;
	new->right = NULL;
	//	ft_putendl_fd("end create node\n");
	return (new);
}

int					clear_node(t_node **node)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CLEAR NODE ------", 2);

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
		ft_putendl_fd("------- CLEAR TREE ------", 2);

	//	ft_putendl_fd("clear tree -- [%s]\n", (*tree)->data);
	if ((*tree) && (*tree)->left)
		clear_tree(&((*tree)->left));
	if ((*tree) && (*tree)->right)
		clear_tree(&((*tree)->right));
	clear_node(tree);
	return (FALSE);
}
