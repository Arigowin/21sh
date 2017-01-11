#include "shell.h"
#include "libft.h"

static states		get_state(states state, char **read_buff)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- GET STATE ------\n");

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

static int			token_standard(char **read_buff, char **data_tmp,
					int *bln, t_e_list **l_expr)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER STANDARD ------\n");

	if (**read_buff == DQUOTE)
		return (FALSE);
	if (**read_buff == '\\')
		token_backslash(read_buff, data_tmp);
	else if (**read_buff == '$')
		token_dollar(read_buff, data_tmp);
	else if (**read_buff == '~' && *bln == FALSE)
		token_tilde(read_buff, data_tmp, bln);
	else if (ft_strchr(SEP, **read_buff))
	{
		*bln = FALSE;
		token_sep(read_buff, data_tmp, l_expr);
	}
	else
		add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}


static int			token_quote(char curr_char, char **data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER QUOTE ------\n");

	if (curr_char != QUOTE)
		add_in_tbl(data_tmp, curr_char);
	return (TRUE);
}

static int			token_dquote(char **read_buff, char **data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER DQUOTE ------\n");

	if (**read_buff == DQUOTE)
		return (FALSE);
	if (**read_buff == '\\')
		token_backslash(read_buff, data_tmp);
	else if (**read_buff == '$')
		token_dollar(read_buff, data_tmp);
	else
		add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}

int 				finite_state_automaton(char **read_buff, t_e_list **l_expr,
					char **data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- FINITE STATE AUTOMATON ------\n");
	int					bln;
	states				state;

	bln = FALSE;
	state = STANDARD;
	while (*read_buff && **read_buff)
	{
		state = get_state(state, read_buff);
		if (state == STANDARD)
			token_standard(read_buff, data_tmp, &bln, l_expr);
		else if (state == IN_QUOTE)
			token_quote(**read_buff, data_tmp);
		else if (state == IN_DQUOTE)
			token_dquote(read_buff, data_tmp);
		(*read_buff)++;
	}
	return (TRUE);
}
