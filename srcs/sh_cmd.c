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
		sh_error(TRUE, 6, NULL, NULL);
	while (tree != NULL)
	{
		if ((tbl[i] = ft_strdup_ignchar(tree->data, '\\')) == NULL)
			sh_error(TRUE, 6, NULL, NULL);
		tree = tree->right;
		i++;
	}
	tbl[i] = NULL;
	return (tbl);
}

static char			**format_cmd(t_node *tree) //static ac manage cmd
{
	if (DEBUG_CMD == 1)
		ft_putendl_fd ("----- FORMAT CMD -----", 2);

	char				**ret;
	t_node				*tmp;
	int					i;

	i = 0;
	ret = NULL;
	tmp = tree;
	if (tree && tree->left && tree->left->right
	&& ft_strcmp(tree->left->right->data, "-1") == 0)
		return (ret);
	while (tmp->right)
	{
		tmp = tmp->right;
		i++;
	}
	ret = tree_to_tbl(tree, i + 1);
	return (ret);
}

static int			nopipe_cmd(int pipefd_tab[2][2], t_node *tree,
		t_lst_fd **lstfd, char **cmd)
{
	if (DEBUG_CMD == 1)
		ft_putendl_fd ("----- MANAGE CMD WITHOUT PIPE -----", 2);

	int					ret;
	int					fd;

	ret = -1;
	fd = (lstfd && *lstfd ? (*lstfd)->fd : -2);
	if (tree && tree->left && ((*lstfd &&
	(((ret = redirect(tree->left, *lstfd)) <= 0) || (*lstfd)->fd == -2))
	|| (tree->left->type == DLRED && redirect(tree->left, NULL) == ERROR)))
	{
		//	sh_error(8, NULL, NULL);// -> pb sur redirection gauche, 2 erreurs s'affichent
		if (ret == ERROR)
		{
			reset_std_fd();
			close_lstfd(lstfd);
			del_lstfd(lstfd);
		}
		else if (ret == ERROR|| ret == FALSE)
			return (ret);
	}
	ret = check_builtin(fd, cmd, pipefd_tab, NULL);
	if (ret != FALSE)
		free_tab(&cmd);
	return (ret);
}

int					manage_cmd(int pipefd_tab[2][2], t_node *tree,
		t_lst_fd **lstfd)
{
	if (DEBUG_CMD == 1)
		ft_putendl_fd ("----- CMD -----", 2);

	char				**cmd;
	int					ret;

	if ((cmd = format_cmd(tree)) == NULL)
		return (ERROR);
	if (pipefd_tab[0][0] < 0 && pipefd_tab[1][0] < 0)
	{
		ret = nopipe_cmd(pipefd_tab, tree, lstfd, cmd);
		if (ret != FALSE)
			return (ret);
	}
	handle_fork(pipefd_tab, tree, lstfd, cmd);
	if (cmd)
		free_tab(&cmd);
	return (TRUE);
}
