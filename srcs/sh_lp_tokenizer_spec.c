#include "shell.h"
#include "libft.h"

int					token_backslash(char **read_buff, char **data_tmp)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- TOKEN BACKSLASH ------", 2);

	if (**read_buff == '\\' && (*(*read_buff + 1)) && (*(*read_buff + 1)) == '\n')
	{
		(*read_buff) += 1;
	}
	else
	{
//		add_in_tbl(data_tmp, **read_buff);
		(*read_buff)++;
		add_in_tbl(data_tmp, **read_buff);
	}
	return (TRUE);
}

int					token_dollar(char **read_buff, char **data_tmp)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- TOKEN DOLLAR ------", 2);

	char 				*env_name;
	char 				*env_val;
	char 				*tmp;

	env_name = NULL;
	env_val = NULL;
	tmp = NULL;
	if ((env_name = ft_strnew(ft_strlen((*read_buff)++))) == NULL)
		return (ERROR);
		/* MSG ret: ERROR exit: FALSE msg: "malloc fail"
		 * free: read_buff + data_tmp */
	while (ft_strchr(SEP, **read_buff) == NULL && **read_buff != QUOTE && **read_buff != DQUOTE)
	{
		add_in_tbl(&env_name, **read_buff);
		(*read_buff)++;
	}
	if ((env_val = get_env(env_name)) == NULL)
	{
		ft_strdel(&env_name);
		return (FALSE);
	}
	ft_strdel(&env_name);
	if (*data_tmp && (tmp = ft_strdup(*data_tmp)) == NULL)
	{
		ft_strdel(&env_val);
		return (ERROR);
	}
	/* MSG ret: ERROR exit: FALSE msg: "malloc fail"
		 * free: read_buff + data_tmp + env_name */
	ft_strdel(data_tmp); // c'est bien ici le free du data_tmp ?
	if ((*data_tmp = ft_strnew(ft_strlen(tmp) + ft_strlen(env_val)
					+ ft_strlen((*read_buff)--))) == NULL)
	{
		ft_strdel(&tmp);
		ft_strdel(&env_val);
		return (ERROR);
	}
	/* MSG ret: ERROR exit: FALSE msg: "malloc fail"
		 * free: read_buff + data_tmp + env_name */
	concat(data_tmp, tmp, env_val);
	ft_strdel(&env_val);
	ft_strdel(&tmp);
	return (TRUE);
}

int					token_tilde(char **read_buff, char **data_tmp, int *bln)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- TOKEN TILDE ------", 2);

	char 				*env_val;
	char 				*tmp;

	tmp = NULL;
	if ((env_val = get_env("HOME")) == NULL || (*(*read_buff + 1) &&
	ft_strchr(SEP, *(*read_buff + 1)) == NULL && *(*read_buff + 1) != '/'))
		return (FALSE);
	if (*data_tmp && (tmp = ft_strdup(*data_tmp)) == NULL)
	{
		ft_strdel(&env_val);
		return (ERROR);
	}
	/* MSG ret: ERROR exit: FALSE msg: "malloc fail" */
	ft_strdel(data_tmp);
	if ((*data_tmp = ft_strnew(ft_strlen(tmp) + ft_strlen(env_val)
					+ ft_strlen(*read_buff))) == NULL)
	{
		ft_strdel(&env_val);
		ft_strdel(&tmp);
		return (ERROR);
	}
	/* MSG ret: ERROR exit: FALSE msg: "malloc fail" */
	concat(data_tmp, tmp, env_val);
	*bln = TRUE;
	ft_strdel(&env_val);
	ft_strdel(&tmp);
	return (TRUE);
}
