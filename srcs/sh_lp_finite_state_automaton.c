#include "shell.h"
#include "libft.h"

static states		get_state(states state, char **read_buff)
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

static int			state_standard(int *hrd, char **read_buff, char **data_tmp,
								   int *bln, t_e_list **l_expr)
{
	if (DEBUG_TOKEN == 0)
		ft_putendl_fd("------- STATE STANDARD ------", 2);

	if (**read_buff == DQUOTE)
		return (FALSE);
	if (**read_buff == '\\')
		token_backslash(STANDARD, read_buff, data_tmp);
	else if (**read_buff == '$')
		token_dollar(read_buff, data_tmp);
	else if (**read_buff && **read_buff == '~' && *bln == FALSE)
	{
		if (token_tilde(read_buff, data_tmp, bln) == FALSE)
			add_in_tbl(data_tmp, **read_buff);
	}
	else if (**read_buff && ft_strchr(SEP, **read_buff))
	{
		*bln = FALSE;
		token_sep(hrd, read_buff, data_tmp, l_expr);
	}
	else
		add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}


static int			state_quote(int *hrd, char curr_char, char **data_tmp)
{
	if (*hrd == 1)
		*hrd = 2;
	if (DEBUG_TOKEN == 0)
		ft_putendl_fd("------- STATE QUOTE ------", 2);

	if (curr_char && curr_char != QUOTE)
		add_in_tbl(data_tmp, curr_char);
	return (TRUE);
}

static int			state_dquote(int *hrd, char **read_buff, char **data_tmp)
{
	if (DEBUG_TOKEN == 0)
		ft_putendl_fd("------- STATE DQUOTE ------", 2);

	if (*hrd == 1)
		*hrd = 2;
	if (**read_buff == DQUOTE)
		return (FALSE);
	if (**read_buff == '\\')
		token_backslash(IN_DQUOTE, read_buff, data_tmp);
	else if (**read_buff == '$')
		token_dollar(read_buff, data_tmp);
	else
		add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}

int 				finite_state_automaton(int *hrd, char **read_buff, t_e_list **l_expr,
										   char **data_tmp)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- FINITE STATE AUTOMATON ------", 2);
	int					bln;
	states				state;

	bln = FALSE;
	state = STANDARD;
	if (data_tmp && read_buff && *read_buff)
	{
		while (read_buff && *read_buff && **read_buff)
		{
			state = get_state(state, read_buff);
			if (state == STANDARD)
				state_standard(hrd, read_buff, data_tmp, &bln, l_expr);
			else if (state == IN_QUOTE)
				state_quote(hrd, **read_buff, data_tmp);
			else if (state == IN_DQUOTE)
				state_dquote(hrd, read_buff, data_tmp);
			(*read_buff)++;
		}
	}
	return (TRUE);
}
