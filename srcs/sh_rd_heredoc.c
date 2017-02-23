#include "libft.h"
#include "shell.h"

static int			tree_trav_hrd(t_line *stline, t_node **tree,
					t_history **history)
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ TREE TRAV HRD ----------", 2);

	if (*tree && (*tree)->left)
	{
		if (heredoc_handler(stline, &((*tree)->left), history) == ERROR)
			return (ERROR);
	}
	if (*tree && (*tree)->right)
	{
		if (heredoc_handler(stline, &((*tree)->right), history) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}

int					check_end_heredoc(t_line *stline)
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ CHECK END HEREDOC ----------", 2);

	if (ft_strcmp(stline->hrd.deli->data, stline->hrd.ptr) == 0)
		return (BREAK);
	stline->hrd.ptr = &((stline->hrd.line)[stline->hrd.pos + 1]);
	return (CONTINUE);
}

char				*hrd_quote_dup(char *str, int len, int type)
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ HRD QUOTE DUP ----------", 2);

	char				*ret;
	char				*tmp;

	ret = ft_strnew(ft_strlen(str));
	while (str && *str)
	{
		if (*str == '\\' && type == 8)
		{
			str++;
			ft_strncat(ret, str, 1);
		}
		else if (*str == '$' && type == 8)
			token_dollar(&str, &ret);
		else
			ft_strncat(ret, str, 1);
		str++;
	}
	tmp = ft_strsub(ret, 0, ft_strlen(ret) - len);
	ft_strdel(&ret);
	return (tmp);
}

static int			fill_hrd_content(t_line *stline, t_node **tree) // static ac heredoc handler
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ FILL HRD CONTENT ----------", 2);

	int 				len;

	len = (ft_strlen(stline->hrd.line) -
			(ft_strlen(stline->hrd.deli->data) + 1));
	printf("(((%d)))\n", (*tree)->right->type);
	if (tree && (*tree) && (*tree)->right  && ((*tree)->right->type == HRD_QUOTE
	|| (*tree)->right->type == RED_ARG))
	{
		if ((*tree)->right->type == HRD_QUOTE && ((*tree)->right->right->data =
		ft_strsub(stline->hrd.line,	0, len + 1)) == NULL)
			return (sh_error(TRUE, 6, NULL, NULL));
		else if (((*tree)->right->right->data = hrd_quote_dup(stline->hrd.line,
		ft_strlen(stline->hrd.deli->data), (*tree)->right->type)) == NULL)
			return (sh_error(TRUE, 6, NULL, NULL));
	}
	else if(((*tree)->right->right->right->data = ft_strsub(stline->hrd.line, 0,
	len + 1)) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	return (TRUE);
}

int					heredoc_handler(t_line *stline, t_node **tree,
					t_history **history)
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ HEREDOC HANDLER ----------", 2);

	static int		bln = 0;

	if (stline->hrd.nb <= 0)
		return (FALSE);
	if ((*tree)->type == DLRED)
	{
		printf("TROLOLO heredoc\n");
		stline->hrd.deli = (((*tree)->right->type == RED_ARG
						|| (*tree)->right->type == HRD_QUOTE) ?
								(*tree)->right : (*tree)->right->right);
		stline->hrd.ptr = stline->hrd.line;
		if (bln)
			mini_prt_handler(&(stline->hrd.line), &(stline->hrd.pos), stline);
		else
			mini_prt_stline(stline);
		check_signal(4);
		if (fct_read(TRUE, stline, history) == ERROR)
			return (ERROR);
		check_signal(3);
		ft_putendl("");
		if ((stline->hrd.line)[0] == '\0')
			return (ERROR);
		if ((stline->hrd.deli->right = create_node(DLRED_DOC)) == NULL)
			return (sh_error(TRUE, 6, NULL, NULL));
		if (fill_hrd_content(stline, tree) == ERROR)
			return(ERROR);
		if (stline->hrd.line)
			ft_bzero(stline->hrd.line, ft_strlen(stline->hrd.line));
		stline->hrd.pos = 0;
		(stline->hrd.nb)--;
	}
	if ((tree_trav_hrd(stline, tree, history)) == ERROR)
		return (ERROR);
	return (TRUE);
}
