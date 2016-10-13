#include "shell.h"
#include "libft.h"

static char		**tree_to_tbl(t_node *tree, int nb_elem)
{
	int			i;
	char		**tbl;

	i = 0;
	if ((tbl = (char **)malloc(sizeof(char *) * (nb_elem + 1))) == NULL)
		return (NULL);
	while (tree != NULL)
	{
		if ((tbl[i] = ft_strdup(tree->data)) == NULL)
			return (NULL);
		tree = tree->right;
		i++;
	}
	tbl[i] = NULL;
	return (tbl);
}

char		**format_cmd(t_node *tree)
{
	char	**ret;
	t_node	*tmp;
	int		i;

	i = 0;
	tmp = tree;
	while (tmp->left)
	{
		tmp = tmp->left;
		i++;
	}

	ret = tree_to_tbl(tree, i);
	return (ret);
}
