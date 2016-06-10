#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

t_node			*create_node(char *data, types type)
{
	t_node			*new;

	if ((new = (t_node *)malloc(sizeof(t_node))) == NULL)
		return (NULL);
	if ((new->data = ft_strdup(data)) == NULL)
		return (NULL);
	new->types = type;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

int				end_left(char *data, types type, t_node **node)
{
	t_node			*tmp;

	tmp = *node;
	if (*node = NULL)
		*node = create_node(data, type);
	else
	{
		while (tmp->left)
			tmp = tmp->left;
		tmp->left = create_node(data, type);
	}
	return (0);
}

int				end_right(char *data, types type, t_node **node)
{
	t_node			*tmp;

	tmp = *node;
	if (*node = NULL)
		*node = create_node(data, type);
	else
	{
		while (tmp->right)
			tmp = tmp->right;
		tmp->right = create_node(data, type);
	}
	return (0);
}

