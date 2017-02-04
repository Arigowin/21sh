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
	if (tree && tree->left && tree->left->right && ft_strcmp(tree->left->right->data, "-1") == 0)
	{
		//		dprintf(2, "data (((((((%s)))))))\n", tree->left->right->data);
		return (ret);
	}
	while (tmp->right)
	{
		tmp = tmp->right;
		i++;
	}
	ret = tree_to_tbl(tree, i + 1);
	return (ret);
}

int					manage_cmd_without_pipe(int pipefd_tab[2][2], t_node *tree,
		t_lst_fd **lstfd, char **cmd)
{
	if (DEBUG_CMD == 1)
		ft_putendl_fd ("----- MANAGE CMD WITHOUT PIPE -----", 2);

	int					ret;
	int					fd;

	ret = -1;
	fd = (lstfd && *lstfd ? (*lstfd)->fd : -2);
	if (tree && tree->left && *lstfd && (ret = redirect(tree->left, *lstfd)))
	{
		if (ret == ERROR || (*lstfd)->fd == -1)
		{
			/* RET: error EXIT: false MSG: "i don't know" */
			reset_std_fd();
			close_lstfd(lstfd);
			del_lstfd(lstfd);
			return (ERROR);
		}
		if (ret == FALSE)
			return (FALSE);
	}
	else if (tree && tree->left && tree->left->type == DLRED
			&& redirect(tree->left, NULL) == ERROR)
	{
		/* RET: error EXIT: false MSG: "i don't know" */
		reset_std_fd();
		close_lstfd(lstfd);
		del_lstfd(lstfd);
		return (ERROR);
	}
	ret = check_builtin(fd, cmd, pipefd_tab, NULL);
	if (ret != FALSE)
		free_tab(&cmd); // FREE_MALLOC_OK
	return (ret);
}

int					manage_cmd(int pipefd_tab[2][2], t_node *tree,
		t_lst_fd **lstfd)
{
	if (DEBUG_CMD == 1)
		ft_putendl_fd ("----- CMD -----", 2);

	char				**cmd;
	int					ret;

	cmd = NULL;
	if ((cmd = format_cmd(tree)) == NULL)
		return (ERROR);
	if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0)
	{
		ret = manage_cmd_without_pipe(pipefd_tab, tree, lstfd, cmd);
		if (ret != FALSE)
			return (ret);
	}
	handle_fork(pipefd_tab, tree, lstfd, cmd);
	return (TRUE);
}
