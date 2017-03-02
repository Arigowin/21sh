#include "shell.h"
#include "libft.h"

t_types				fill_red_type(char *data, int *nb_hrd)
{
	t_types				type;

	type = ft_strequ(data, ">") ? RRED : RED;
	type = ft_strequ(data, ">>") ? DRRED : type;
	type = ft_strequ(data, "<") ? LRED : type;
	type = ft_strequ(data, "<<") ? DLRED : type;
	type = ft_strequ(data, "<>") ? RWRED : type;
	*nb_hrd += (type == DLRED ? 1 : 0);
	return (type);
}

int					parser_ret_fct(int ret, t_node **tree, t_node **node,
					t_node **to_free)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- PARSER RET FCT ------\n", 2);

	if (tree && node && *node)
		*tree = *node;
	clear_node(to_free);
	return (ret);
}

int					move_in_list(t_e_list **l_expr)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- MOVE IN LIST ------\n", 2);

	if ((*l_expr) && (*l_expr)->next)
	{
		(*l_expr) = (*l_expr)->next;
		return (TRUE);
	}
	return (FALSE);
}