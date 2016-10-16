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
	while (tmp->left)
	{
		tmp = tmp->left;
		i++;
	}

	ret = tree_to_tbl(tree, i);
	return (ret);
}

void		close_fd_red(t_intlst **lstfd, int saved_stdout, int saved_stdin)
{
	t_intlst	*tmp;

	while (*lstfd)
	{

		close((*lstfd)->data);
		tmp = *lstfd;
		*lstfd = (*lstfd)->next;
		free(tmp);
	}
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
}

int			manage_cmd(t_node *tree)
{
	if (DEBUG_CMD == 1)
		printf ("----- CMD -----(%s)\n", tree->data);
	t_intlst	*lstfd;
	char		**cmd;
	int			i;
	int			saved_stdout;
	int			saved_stdin;

	i = 0;
	lstfd = NULL;
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (tree->left != NULL)
		red(tree->left, &lstfd);

	if ((cmd = format_cmd(tree)) == NULL)
		return (FALSE);

	while (cmd[++i])
	{
		if (cmd[i][0] == '~')
			manage_tilde(&cmd[i]);
	}
	if (handle_builtin(cmd) != 0)
		return (FALSE);
	if (check_home(cmd) < 0)
		return (FALSE);
	father_n_son(cmd);
	close_fd_red(&lstfd, saved_stdout, saved_stdin);
	//free_tab(&cmd); // erreur de free
	return (TRUE);
}
