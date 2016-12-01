#include "shell.h"
#include "libft.h"

static char		**tree_to_tbl(t_node *tree, int nb_elem)
{
	if (DEBUG_CMD == 1)
		printf ("----- TREE TO TBL -----\n");
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
	if (DEBUG_CMD == 1)
		printf ("----- FORMAT CMD -----\n");
	char	**ret;
	t_node	*tmp;
	int		i;

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

void		close_fd_red(t_intlst **lstfd, int saved_std[3])
{
	if (DEBUG_CMD == 1)
		printf ("----- CLOSE FD RED -----\n");
	t_intlst	*tmp;

	while (*lstfd)
	{
		close((*lstfd)->data);
		tmp = *lstfd;
		*lstfd = (*lstfd)->next;
		free(tmp);
	}
	if (saved_std[0] != -1 || saved_std[1] != -1 || saved_std[2] != -1)
	{
		dup2(saved_std[0], STDIN_FILENO);
		dup2(saved_std[1], STDOUT_FILENO);
		dup2(saved_std[2], STDERR_FILENO);
		close(saved_std[0]);
		close(saved_std[1]);
		close(saved_std[2]);
	}
}

int			manage_cmd(t_node *tree)
{
	if (DEBUG_CMD == 1)
		printf ("----- CMD -----(%s)\n", tree->data);
	t_intlst	*lstfd;
	char		**cmd;
	int			saved_std[3];
	int 		ret;

	saved_std[0] = -1;
	saved_std[1] = -1;
	saved_std[2] = -1;
	ret = 0;
	lstfd = NULL;
	if (tree->left != NULL)
	{
		saved_std[0] = dup(STDIN_FILENO);
		saved_std[1] = dup(STDOUT_FILENO);
		saved_std[2] = dup(STDERR_FILENO);
		if (red(tree->left, &lstfd) == ERROR)
		{
			close_fd_red(&lstfd, saved_std);
			return (ERROR);
		}
	}

	if ((cmd = format_cmd(tree)) == NULL)
		return (FALSE);

	if ((ret = handle_builtin(cmd)) == 1 || ret != 0)
		close_fd_red(&lstfd, saved_std);
	if (ret != 0)
		return (FALSE);
	if (check_home(cmd) < 0)
		return (FALSE);
	father_n_son(cmd);
	close_fd_red(&lstfd, saved_std);
	//free_tab(&cmd); // erreur de free
	return (TRUE);
}
