#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

t_node			*create_node(types type)
{
	if (DEBUG_TREE == 1)
		printf("------- CREATE NODE ------\n");
	t_node			*new;

	if ((new = (t_node *)malloc(sizeof(t_node))) == NULL)
		return (NULL);
	new->data = NULL;
	new->type = type;
	new->left = NULL;
	new->right = NULL;
//	printf("end create node\n");
	return (new);
}

//int				add_node(char *data, types type, t_node **node, int side)
//{
//	t_node			*tmp;
//
//	tmp = *node;
//	if (*node == NULL)
//		*node = create_node(data, type);
//	if (side == 0)
//	{
//		while (tmp->right)
//			tmp = tmp->right;
//		tmp->right = create_node(data, type);
//	}
//	else if (side == 1)
//	{
//		while (tmp->left)
//			tmp = tmp->left;
//		tmp->left = create_node(data, type);
//	}
//	return (0);
//}
