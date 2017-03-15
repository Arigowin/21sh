#include "shell.h"
#include "libft.h"

static t_states		get_state(t_states state, char **read_buff)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- GET STATE ------", 2);

	if (state == STANDARD && **read_buff == '"')
		return (IN_DQUOTE);
	if (state == STANDARD && **read_buff == '\'')
		return (IN_QUOTE);
	if ((state == IN_DQUOTE && **read_buff == '"')
		|| (state == IN_QUOTE && **read_buff == '\''))
	{
		(*read_buff)++;
		return (STANDARD);
	}
	return (state);
}

static int			state_standard(int *int_tbl[2], char **read_buff,
					char **data_tmp, t_e_list **l_expr)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- STATE STANDARD ------", 2);

	if (**read_buff == DQUOTE || **read_buff == QUOTE)
		return (FALSE);
	if (**read_buff == '\\')
		token_backslash(STANDARD, read_buff, data_tmp);
	else if (**read_buff == '$')
		token_dollar(read_buff, data_tmp);
	else if (**read_buff && **read_buff == '~' && (*(int_tbl[0]) == TRUE
	|| (*(int_tbl[0]) == FALSE
	&& token_tilde(read_buff, data_tmp, int_tbl[0]) == FALSE)))
		add_in_tbl(data_tmp, **read_buff);
	else if (**read_buff && ft_strchr(SEP, **read_buff))
	{
		*(int_tbl[0]) = FALSE;
		token_sep(int_tbl[1], read_buff, data_tmp, l_expr);
	}
	else if (**read_buff)
		add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}

static int			state_quote(int *hrd, char *read_buff, char **data_tmp)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- STATE QUOTE ------", 2);

	*hrd = (*hrd > 1 ? *hrd : QUOTE);
	if (*read_buff)
	{
		if (*read_buff == QUOTE)
		{
			if (*(read_buff + 1) == QUOTE)
				add_in_tbl(data_tmp, 26);
			return (FALSE);
		}
		add_in_tbl(data_tmp, *read_buff);
	}
	return (TRUE);
}

static int			state_dquote(int *hrd, char **read_buff, char **data_tmp)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- STATE DQUOTE ------", 2);

	*hrd = (*hrd > 1 ? *hrd : DQUOTE);
	if (**read_buff == DQUOTE)
	{
		if (*(*read_buff + 1) == DQUOTE)
			add_in_tbl(data_tmp, 26);
		return (FALSE);
	}
	if (**read_buff == '\\')
		token_backslash(IN_DQUOTE, read_buff, data_tmp);
	else if (**read_buff == '$')
		token_dollar(read_buff, data_tmp);
	else
		add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}

int					finite_state_automaton(int *hrd, char **read_buff,
					t_e_list **l_expr, char **data_tmp)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- FINITE STATE AUTOMATON ------", 2);

	int					bln;
	int					*int_tbl[2];
	t_states			state;

	bln = FALSE;
	state = STANDARD;
	while (data_tmp && read_buff && *read_buff && **read_buff)
	{
		int_tbl[0] = &bln;
		int_tbl[1] = hrd;
		state = get_state(state, read_buff);
		if (!(**read_buff))
			break ;
		if (state == STANDARD)
			state_standard(int_tbl, read_buff, data_tmp, l_expr);
		else if (state == IN_QUOTE)
			state_quote(hrd, *read_buff, data_tmp);
		else if (state == IN_DQUOTE)
			state_dquote(hrd, read_buff, data_tmp);
		if (*(*read_buff))
			(*read_buff)++;
	}
	return (TRUE);
}
