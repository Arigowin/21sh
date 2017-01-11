#include <limits.h>
#include "shell.h"

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

int					concat(char **dest, char *s1, char *s2)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- EXPR PUSHBK ------\n");

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

int 				token_sep(char **read_buff, char **data_tmp,
					t_e_list **l_expr)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- TOKEN SEP ------\n");

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

int					token_dollar(char **read_buff, char **data_tmp)
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

int					token_tilde(char **read_buff, char **data_tmp, int *bln)
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