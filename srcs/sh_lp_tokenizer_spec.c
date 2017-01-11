#include "shell.h"
#include "libft.h"

int					token_backslash(char **read_buff, char **data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER BACKSLASH ------\n");

	if (**read_buff == '\\' && (*(*read_buff + 1)) && (*(*read_buff + 1)) == '\n')
	{
		(*read_buff) += 1;
	}
	else
	{
		add_in_tbl(data_tmp, **read_buff);
		(*read_buff)++;
		add_in_tbl(data_tmp, **read_buff);
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
