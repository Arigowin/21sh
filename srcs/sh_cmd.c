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

int			manage_cmd(t_node *tree, t_duo **env)
{
	t_intlst	*lstfd;
	char		**cmd;
	int			i;

	i = 0;
	if (tree->left != NULL)
		red(tree, &lstfd);

	if ((cmd = format_cmd(tree)) == NULL)
		return (FALSE);

    while (cmd[++i])
    {
        if (cmd[i][0] == '~')
            manage_tilde(env, &cmd[i]);
    }
    if (handle_builtin(cmd, env) != 0)
        return (FALSE);
    if (check_home(cmd) < 0)
        return (FALSE);
    father_n_son(cmd, env);
    free_tab(&cmd);
	return (TRUE);
}
