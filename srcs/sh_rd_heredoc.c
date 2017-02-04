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

static int			fill_hrd_content(t_line *stline, t_node **tree) // static ac heredoc handler
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ FILL HRD CONTENT ----------", 2);

	int 				len;

	len = (ft_strlen(stline->hrd.line) -
			(ft_strlen(stline->hrd.deli->data) + 1));
	if (tree && (*tree) && (*tree)->right && (*tree)->right->type == RED_ARG)
	{
		if (((*tree)->right->right->data = ft_strsub(stline->hrd.line, 0,
		len + 1)) == NULL)
			/* RET: error EXIT: true MSG: "malloc fail" */
			return (ERROR);
	}
	else// if (tree && (*tree) && (*tree)->right && (*tree)->right->right && (*tree)->right->type == RED_ARG)
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
		stline->hrd.deli = ((*tree)->right->type == RED_ARG ? (*tree)->right
													: (*tree)->right->right);
		stline->hrd.ptr = stline->hrd.line;
		mini_prt_handler(&(stline->hrd.line), &(stline->hrd.pos), stline);
		if (fct_read(TRUE, stline, history) == ERROR)
			return (ERROR);
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
