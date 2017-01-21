#include "shell.h"
#include "libft.h"

static char			**tree_to_tbl(t_node *tree, int nb_elem)
{
	if (DEBUG_CMD == 1)
		ft_putendl_fd ("----- TREE TO TBL -----", 2);

	int					i;
	char				**tbl;

	i = 0;
	if ((tbl = (char **)malloc(sizeof(char *) * (nb_elem + 1))) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (NULL);
	while (tree != NULL)
	{
		if ((tbl[i] = ft_strdup(tree->data)) == NULL)
			/* RET: error EXIT: true MSG: "malloc fail"
			** FREE: tbl */
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
		ft_putendl_fd ("----- FORMAT CMD -----", 2);

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

int					manage_cmd(int pipefd_tab[2][2], t_node *tree, t_global_fd **globalfd)
{
	if (DEBUG_CMD == 1)
		ft_putendl_fd ("----- CMD -----\n", 2);

	handle_fork(pipefd_tab, tree, globalfd);
	return (TRUE);
}
