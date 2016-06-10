#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

static t_e_list			*expr_new(char *content)
{
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
	t_e_list		*tmp;

	tmp = *l_expr;
	if (content[0] == '\0')
		return ;
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
	(*tmp)[0] = r_buff[(*i)];
	if (r_buff[(*i) + 1] && r_buff[(*i) + 1] == r_buff[(*i)]
			&& r_buff[(*i)] != ';')
	{
		(*tmp)[1] = r_buff[(*i) + 1];
		(*i)++;
	}
	expr_pushbk(l_exp, *tmp);
	ft_bzero(*tmp, 2);
	return (0);
}

int						lexer_1(char *read_buff, t_e_list **l_expr)
{
	char			tmp[1024];
	int				i;
	int				k;

	ft_bzero(tmp, 1024);
	i = -1;
	k = 0;
	while (read_buff[++i])
	{
		if (ft_strchr(SEP, read_buff[i]))
		{
			expr_pushbk(l_expr, tmp);
			ft_bzero(tmp, 1024);
			k = 0;
			if (ft_strchr(IGN, read_buff[i]) == NULL)
				in_lexer_1(&tmp, read_buff, &i, l_expr);
		}
		else if (ft_strchr(SEP, read_buff[i]) == NULL)
			tmp[k++] = read_buff[i];
	}
	if (ft_strlen(tmp))
		expr_pushbk(l_expr, tmp);
	return (0);
}
