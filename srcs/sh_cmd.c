#include "shell.h"
#include "libft.h"

static char			**tree_to_tbl(t_node *tree, int nb_elem)
{
	if (DEBUG_CMD == 1)
		printf ("----- TREE TO TBL -----\n");

	int					i;
	char				**tbl;

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

char				**format_cmd(t_node *tree)
{
	if (DEBUG_CMD == 1)
		printf ("----- FORMAT CMD -----\n");

	char				**ret;
	t_node				*tmp;
	int					i;

	i = 0;
	ret = NULL;
	tmp = tree;
	while (tmp->right)
	{
		tmp = tmp->right;
		i++;
	}
	ret = tree_to_tbl(tree, i + 1);
	return (ret);
}

int					manage_cmd(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_CMD == 1)
		printf ("----- CMD -----(%s)\n", tree->data);

	handle_fork(-1, -1, tree, lstfd);
	return (TRUE);
}
