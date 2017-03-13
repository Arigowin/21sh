#include "shell.h"
#include "libft.h"

int					check_end_pipe(char **str, int *pos)
{
	if (DEBUG_KEY == 1)
		ft_putendl_fd("------- CHECK END PIPE ------", 2);

	int					i;
	int					j;

	i = *pos - 1;
	while (str && *str && (*str)[i] && i > 0)
	{
		if ((*str)[i] != ' ' && (*str)[i] != '\t' && (*str)[i] != '\n'
		&& (*str)[i] != '|')
			return (FALSE);
		if ((*str)[i] == '|' && (*str)[i - 1] && (*str)[i - 1] == '|')
		{
			j = 1;
			i = ft_strlen(*str);
			while ((*str)[i - 1] && (*str)[--i] == '|' && i > 0
			&& (*str)[i] == (*str)[i - 1])
				j++;
			if (j % 2 != 0)
				return (TRUE);
			return (FALSE);
		}
		if ((*str)[i] == '|' && (*str)[i - 1] && (*str)[i - 1] != '\\')
			return (TRUE);
		i--;
	}
	return (FALSE);
}

//int					check_nb_quote(char c, int back, int *quote, int to_fill)
//{
//	if (c && (*quote == QUOTE || (c != '\\' || back % 2 == 0)))
//		*quote = (*quote != 0 ? 0 : to_fill);
//	if (c == '\0')
//		*quote = (*quote != 0 ? 0 : to_fill);
//	return (TRUE);
//}

int					quote_is_close(char **str)
{
	int					i;
	int					quote;
	int					bkslash;

	i = -1;
	quote = 0;
	bkslash = 0;
	while (str && *str && (*str)[++i])
	{
		if (quote != DQUOTE && (*str)[i] && (*str)[i] == QUOTE
		&& (quote == QUOTE || bkslash % 2 == 0))
			quote = (quote != 0 ? 0 : QUOTE);
		if (quote != QUOTE && (*str)[i] && (*str)[i] == DQUOTE
		&& bkslash % 2 == 0)
			quote = (quote != 0 ? 0 : DQUOTE);
		if ((*str)[i] == '\\')
			bkslash++;
		else if (bkslash > 0 && (*str)[i] && ((*str)[i] != '\\'
		|| ((*str)[i] != QUOTE || (*str)[i] != DQUOTE)))
			bkslash = 0;
	}
	return (quote);
}

int					handle_quote(int key, char **str, int *pos, t_line *stline)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- HANDLE QUOTE ------", 2);

	if (*pos > 0 && (*str)[(*pos) - 1] && (*str)[(*pos) - 1] == '\\')
		return (FALSE);
	if (key == QUOTE || key == DQUOTE)
	{
		if (stline->quote == key)
			stline->quote = 0;
		else if (stline->quote == 0)
			stline->quote = key;
	}
	return (TRUE);
}
