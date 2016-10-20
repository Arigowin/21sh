#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

char					*search_with_backslash(char *str, char c)
{
	int				i;

	i = 0;
	if (str)
	{
		while (str[i] != '\0')
		{
			if (str[i] == c && str[i - 1] && str[i - 1] != '\\')
				return (str + i);
			i++;
		}
		if (str[i] == c)
			return (str + i);
	}
	return (NULL);
}

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
	if (content == NULL)
		return (NULL);
	new->data = ft_strdup(content);
	return (new);
}

static void				expr_pushbk(t_e_list **l_expr, char content[])
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- EXPR PUSHBK ------\n");
	t_e_list		*tmp;
	char			*content_tmp;
	int				i;

	i = -1;
	tmp = *l_expr;
	content_tmp = NULL;
	if (content[0] == '\0')
		return ;
	if (content[0] == content[ft_strlen(content) - 1] && (content [0] == QUOTE || content[0] == DQUOTE))
	{
		content_tmp = ft_strsub(content, 1, ft_strlen(content) - 2);
		ft_bzero(content, 1024);
		while (content_tmp[++i] != '\0')
			content[i] = content_tmp[i];
		free(content_tmp);
	}
	if (tmp == NULL)
	{
		*l_expr = expr_new(content);
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = expr_new(content);
}

static int				in_lexer_1(char (*tmp)[], char *r_buff, int *i,
		t_e_list **l_exp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- IN LEXER 1 ------\n");
	int				j;

	j = 1;
	(*tmp)[0] = r_buff[(*i)];
	if (r_buff[(*i) + j] && r_buff[(*i) + j] == r_buff[(*i)]
			&& r_buff[(*i)] != '\0')
	{
		(*tmp)[j] = r_buff[(*i) + j];
		(*i)++;
	}
	expr_pushbk(l_exp, *tmp);
	ft_bzero(*tmp, j + 1);
	return (0);
}

int						lexer_1(char *read_buff, t_e_list **l_expr)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER 1 ------\n");
	char			tmp[1024];
	int				i;
	int				k;
	int				quote;
	char			boolean;

	ft_bzero(tmp, 1024);
	i = -1;
	k = 0;
	quote = 0;
	while (read_buff[++i])
	{
		boolean = FALSE;
		if (read_buff[i] == QUOTE || read_buff[i] == DQUOTE)
		{
			quote = (quote == read_buff[i] ? 0 : read_buff[i]);
		}
		if (quote == 0 && search_with_backslash(SEP, read_buff[i]))
		{

			if (search_with_backslash(WAKA, read_buff[i]) && read_buff[i - 1]
					&& ft_isdigit(read_buff[i - 1]) && (!read_buff[i - 2]
						|| (read_buff[i - 2] && search_with_backslash(SEP, read_buff[i - 2]))))
			{
				tmp[k++] = read_buff[i];
				if (read_buff[i + 1] && read_buff[i + 1] == read_buff[i])
				{
					tmp[k++] = read_buff[i + 1];
					i++;
				}
				boolean = TRUE;
			}
			expr_pushbk(l_expr, tmp);
			ft_bzero(tmp, 1024);
			k = 0;
			if (!boolean && !search_with_backslash(IGN, read_buff[i]))
				in_lexer_1(&tmp, read_buff, &i, l_expr);
		}
		else if (quote == 0 && search_with_backslash(SEP, read_buff[i]) == NULL)
			tmp[k++] = read_buff[i];
		else if (quote != 0)
			tmp[k++] = read_buff[i];
	}
	if (ft_strlen(tmp))
		expr_pushbk(l_expr, tmp);
	return (0);
}
