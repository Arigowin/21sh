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

char				*hrd_quote_dup(char *str, int len)
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ HRD QUOTE DUP ----------", 2);

	char				*ret;
	char				*tmp;

	ret = ft_strnew(ft_strlen(str));
	while (str && *str)
	{
		if (*str == '\\')
			str++;
		else if (*str == '$')
			token_dollar(&str, &ret);
		else
			ft_strncat(ret, str, 1);
			str++;
	}
	tmp = ft_strsub(ret, 0, ft_strlen(ret) - len);
	return (tmp);
}

static int			fill_hrd_content(t_line *stline, t_node **tree) // static ac heredoc handler
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ FILL HRD CONTENT ----------", 2);

	int 				len;

	len = (ft_strlen(stline->hrd.line) -
			(ft_strlen(stline->hrd.deli->data) + 1));
	if (tree && (*tree) && (*tree)->right  && ((*tree)->right->type == HRD_QUOTE || (*tree)->right->type == RED_ARG))
	{
		if ((*tree)->right->type == HRD_QUOTE)
		{
			if (((*tree)->right->right->data = ft_strsub(stline->hrd.line,
			0, len + 1)) == NULL)
			/* RET: error EXIT: true MSG: "malloc fail" */
				return (ERROR);
		}
		else
		{
			if (((*tree)->right->right->data = hrd_quote_dup(stline->hrd.line, ft_strlen(stline->hrd.deli->data))) == NULL)
			/* RET: error EXIT: true MSG: "malloc fail" */
				return (ERROR);
		}
	}
	else
	{
		if(((*tree)->right->right->right->data = ft_strsub(stline->hrd.line, 0,
		len + 1)) == NULL)
			/* RET: error EXIT: true MSG: "malloc fail" */
			return (ERROR);
	}
	return (TRUE);
}

int					heredoc_handler(t_line *stline, t_node **tree,
					t_history **history)
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ HEREDOC HANDLER ----------", 2);

	if (stline->hrd.nb <= 0)
		return (FALSE);
	if ((*tree)->type == DLRED)
	{
		stline->hrd.deli = (((*tree)->right->type == RED_ARG || (*tree)->right->type == HRD_QUOTE) ? (*tree)->right : (*tree)->right->right);
		stline->hrd.ptr = stline->hrd.line;
		mini_prt_handler(&(stline->hrd.line), &(stline->hrd.pos), stline);
		check_signal(4);
		if (fct_read(TRUE, stline, history) == ERROR)
			return (ERROR);
		check_signal(1);
		ft_putendl("");
		if ((stline->hrd.deli->right = create_node(DLRED_DOC)) == NULL)
			/* RET: error EXIT: true MSG: "malloc fail" */
			return (ERROR);
		if (fill_hrd_content(stline, tree) == ERROR)
			/* useless return */
			return(ERROR);
		if (stline->hrd.line)
			ft_bzero(stline->hrd.line, ft_strlen(stline->hrd.line));
		stline->hrd.pos = 0;
		(stline->hrd.nb)--;
	}
	if ((tree_trav_hrd(stline, tree, history)) == ERROR)
		/* RET: error EXIT: false MSG: "" */
		return (ERROR);
	return (TRUE);
}
