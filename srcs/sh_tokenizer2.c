#include "libft.h"
#include "shell.h"

states				get_state(states state, char **read_buff)
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

int 				finite_state_atomaton(char **read_buff, t_e_list **l_expr,
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
