#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "libft.h"
#include "shell.h"

t_e_list				*expr_new(char *content)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- EXPR NEW ------\n");
	t_e_list		*new;

	if ((new = (t_e_list *)malloc(sizeof(t_e_list))) == NULL)
		return (NULL);
	new->data = NULL;
	new->type = NONE;
	new->next = NULL;
	if ((new->data = ft_strdup(content)) == NULL)
		return (NULL);
	return (new);
}

int 					expr_pushbk(t_e_list **l_expr, char *data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- EXPR PUSHBK ------\n");

	t_e_list				*tmp;

	tmp = NULL;
	if (data_tmp == NULL && *data_tmp == '\0')
		return (FALSE);
	if (!(l_expr && *l_expr))
	{
		if ((*l_expr = expr_new(data_tmp)) == NULL)
			return (ERROR);
		return (TRUE);
	}
	tmp = *l_expr;
	while (tmp->next)
		tmp = tmp->next;
	if ((tmp->next = expr_new(data_tmp)) == NULL)
		return (ERROR);
	return (TRUE);
}

int						concat(char **dest, char *s1, char *s2)
{
	if (!(dest && *dest))
		return (-1);
	while (s1 && *s1)
	{
		add_in_tbl(dest, *s1);
		s1++;
	}
	while (s2 && *s2)
	{
		add_in_tbl(dest, *s2);
		s2++;
	}
	return (0);
}

static int			pushbck_cdt(char **read_buff, char **data_tmp)
{
	return (**data_tmp
			&& (!(ft_strchr(WAKA, (*data_tmp)[ft_strlen(*data_tmp) - 1])
					&& **read_buff == '&'))
			&& (!((*data_tmp)[ft_strlen((*data_tmp)	- 1)] == '&'
					&& ft_strchr(WAKA, **read_buff)))
			&& (!(ft_isstrnum(*data_tmp)
					&& ft_strchr(WAKA, **read_buff)
					&& ft_strlen(*data_tmp) <= 10
					&& ft_atoi_long(*data_tmp) <= INT_MAX)));
}

int 				manage_sep(char **read_buff, char **data_tmp,
		t_e_list **l_expr)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- MANAGE SEP ------\n");

	if (pushbck_cdt(read_buff, data_tmp))
	{
		expr_pushbk(l_expr, *data_tmp);
		ft_bzero(*data_tmp, ft_strlen(*data_tmp));
	}

	if (ft_strchr(SPECIAL, **read_buff))
	{
		add_in_tbl(data_tmp, **read_buff);
		if (ft_strchr(WAKA, **read_buff) && (*(*read_buff + 1))
				&& (*(*read_buff + 1)) == (**read_buff))
		{
			(*read_buff)++;
			add_in_tbl(data_tmp, **read_buff);
		}
		if ((**read_buff == '&' && ft_strchr(WAKA, (*(*read_buff + 1))))
		|| (ft_strchr(WAKA, **read_buff) && (*(*read_buff + 1)) == '&'))
			return (TRUE);
		expr_pushbk(l_expr, *data_tmp);
		ft_bzero(*data_tmp, ft_strlen(*data_tmp));
	}
	return (TRUE);
}

int					lexer_dollar(char **read_buff, char **data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER DOLLAR ------\n");

	char 				*env_name;
	char 				*env_val;
	char 				*tmp;

	env_val = NULL;
	tmp = NULL;
	(*read_buff)++;
	if ((env_name = ft_strnew(ft_strlen(*read_buff))) == NULL)
		return (ERROR);
	while (ft_strchr(SEP, **read_buff) == NULL)
	{
		add_in_tbl(&env_name, **read_buff);
		(*read_buff)++;
	}
	(*read_buff)--;
	if ((env_val = get_env(env_name)) == NULL)
		return (FALSE);
	if (*data_tmp && (tmp = ft_strdup(*data_tmp)) == NULL)
		return (ERROR);
	ft_strdel(data_tmp);
	if ((*data_tmp = ft_strnew(ft_strlen(tmp) + ft_strlen(env_val)
					+ ft_strlen(*read_buff))) == NULL)
		return (ERROR);
	concat(data_tmp, tmp, env_val);
	return (TRUE);
}

int					lexer_tilde(char **read_buff, char **data_tmp, int *bln)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER TILDE ------\n");

	char 				*env_val;
	char 				*tmp;

	env_val = NULL;
	tmp = NULL;
	if ((env_val = get_env("HOME")) == NULL)
		return (FALSE);
	if (*data_tmp && (tmp = ft_strdup(*data_tmp)) == NULL)
		return (ERROR);
	ft_strdel(data_tmp);
	if ((*data_tmp = ft_strnew(ft_strlen(tmp) + ft_strlen(env_val)
					+ ft_strlen(*read_buff))) == NULL)
		return (ERROR);
	concat(data_tmp, tmp, env_val);
	*bln = TRUE;
	return (TRUE);
}

int					lexer_backslash(char **read_buff, char **data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER BACKSLASH ------\n");

	if (**read_buff == '\\' && (*(*read_buff + 1)) && (*(*read_buff + 1)) == '\n')
	{
		(*read_buff) += 2;
	}
	else
	{
		add_in_tbl(data_tmp, **read_buff);
		(*read_buff)++;
		add_in_tbl(data_tmp, **read_buff);
	}
	return (TRUE);
}

int					lexer_standard(char **read_buff, char **data_tmp,
		int *bln, t_e_list **l_expr)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER STANDARD ------\n");

	if (**read_buff == DQUOTE)
		return (FALSE);
	if (**read_buff == '\\')
		lexer_backslash(read_buff, data_tmp);
	else if (**read_buff == '$')
		lexer_dollar(read_buff, data_tmp);
	else if (**read_buff == '~' && *bln == FALSE)
		lexer_tilde(read_buff, data_tmp, bln);
	else if (ft_strchr(SEP, **read_buff))
	{
		*bln = FALSE;
		manage_sep(read_buff, data_tmp, l_expr);
	}
	else
		add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}


int					lexer_quote(char curr_char, char **data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER QUOTE ------\n");

	if (curr_char != QUOTE)
		add_in_tbl(data_tmp, curr_char);
	return (TRUE);
}


int					lexer_dquote(char **read_buff, char **data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER DQUOTE ------\n");

	if (**read_buff == DQUOTE)
		return (FALSE);
	if (**read_buff == '\\')
		lexer_backslash(read_buff, data_tmp);
	else if (**read_buff == '$')
		lexer_dollar(read_buff, data_tmp);
	else
		add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}

states				get_state(states state, char **read_buff)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- GET STATE ------\n");
	if (state == STANDARD && **read_buff == '"')
		return (IN_DQUOTE);
	if (state == STANDARD && **read_buff == '\'')
		return (IN_QUOTE);
	if ((state == IN_DQUOTE && **read_buff == '"') || (state == IN_QUOTE && **read_buff == '\''))
	{
		(*read_buff)++;
		return (STANDARD);
	}
	return (state);
}

int 					finite_state_atomaton(char **read_buff, t_e_list **l_expr
						,char **data_tmp)
{
//	if (DEBUG_LEXER_PARSER == 1)
		printf("------- FINITE STATE AUTOMATON ------\n");
	int						bln;
	states					state;

	bln = FALSE;
	state = STANDARD;
	while (*read_buff && **read_buff)
	{
		state = get_state(state, read_buff);
		if (state == STANDARD)
			lexer_standard(read_buff, data_tmp, &bln, l_expr);
		else if (state == IN_QUOTE)
			lexer_quote(**read_buff, data_tmp);
		else if (state == IN_DQUOTE)
			lexer_dquote(read_buff, data_tmp);
		(*read_buff)++;
	}
	return (TRUE);
}

int						lexer_1(char *read_buff, t_e_list **l_expr)
{
//	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER 1 ------\n");
	char 					*data_tmp;

	(void)l_expr;
	data_tmp = ft_strnew(ft_strlen(read_buff));
	finite_state_atomaton(&read_buff, l_expr, &data_tmp);
	if (*data_tmp)
	{
		expr_pushbk(l_expr, data_tmp);
		ft_bzero(data_tmp, ft_strlen(data_tmp));
	}

	if (DEBUG_LEXER_PARSER == 1)
	{
		t_e_list *tmp = *l_expr;
		while (tmp)
		{
			printf("[%s] -> ", tmp->data);
			tmp = tmp->next;
		}
		printf("\n");
	}

	return (TRUE);
}
