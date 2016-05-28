#include "shell.h"
#include "libft.h""

t_arglst		*arglst_new(char *token, int type)
{
	t_arglst		*new;

	if ((new = (t_arglst *)malloc(sizeof(t_arglst))) == NULL)
		return (NULL);
	new->type = 0;
	new->token = NULL;
	new->next = NULL;
	if (token == NULL)
		return (NULL);
	new->content = ft_strdup(token);
	new->type = type;
	return (new);
}

int				arglst_pushbk(t_arglst **agrlst, char *token, int value)
{
	t_arglst		*tmp;

	tmp = *arglst;
	if (tmp == NULL)
	{
		*arglst = arglst_new(token, value);
		return (0);
	}
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = arglst_new(token, value);
	return (0);
}

t_arglst		*lexer(char *read_buff)
{
	char			*tmp;
	t_arglst		*ret;
	int				i;
	int				j;

	ft_bzero(tmp, ft_strlen(read_buff));
	ret = NULL;
	i = 0
	while (read_buff[i])
	{
		j = 0;
		while (SEP && read_buff[i] != SEP[j])
			j++;
		if (read_buff[i] == SEP[j] && (read_buff[i] != ' ' && read_buff[i] != '\t' && read_buff[i] != '\n' && read_buff[i] != '\0'))
	}
}
