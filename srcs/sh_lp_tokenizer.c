#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "shell.h"
#include "libft.h"

static int			pushbck_cdt(char **read_buff, char **data_tmp)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- PUSHBCK_CDT ------", 2);

	return (**data_tmp
		&& (!(ft_strchr(WAKA, (*data_tmp)[ft_strlen(*data_tmp) - 1])
				&& **read_buff == '&'))
		&& (!((*data_tmp)[ft_strlen(*data_tmp)	- 1] == '&'
				&& ft_strchr(WAKA, **read_buff)))
		&& (!(ft_isstrnum(*data_tmp)
				&& ft_strchr(WAKA, **read_buff)
				&& ft_strlen(*data_tmp) <= 10
				&& ft_atoi_long(*data_tmp) <= INT_MAX)));
}

int					concat(char **dest, char *s1, char *s2)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- CONCAT ------", 2);

	if (!(dest && *dest))
		return (ERROR);
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
	return (TRUE);
}

int 				token_sep(int *hrd, char **read_buff, char **data_tmp,
					t_e_list **l_expr)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- TOKEN SEP ------", 2);

	if (pushbck_cdt(read_buff, data_tmp))
	{
		expr_pushbk(l_expr, *data_tmp, *hrd);
		*hrd = (*hrd >= 1 && ft_strcmp("<<", *data_tmp) != 0 ? 0 : *hrd);
		ft_bzero(*data_tmp, ft_strlen(*data_tmp));
	}
	if (read_buff && *read_buff && **read_buff && ft_strchr(SPECIAL, **read_buff) != NULL)
	{
		add_in_tbl(data_tmp, **read_buff);
		if (ft_strchr(LWAKA, **read_buff) && (*(*read_buff + 1)) && ((**read_buff == '<' && *(*read_buff + 1) == '>') || (*(*read_buff + 1)) == (**read_buff)))
			//add_in_tbl(data_tmp, *(++(*read_buff)));
		{
			(*read_buff)++;
			add_in_tbl(data_tmp, **read_buff);
		}
		if ((**read_buff == '&' && ft_strchr(WAKA, (*(*read_buff + 1))))
		|| (ft_strchr(WAKA, **read_buff) && (*(*read_buff + 1)) == '&'))
			return (TRUE);
		expr_pushbk(l_expr, *data_tmp, *hrd);
		if (*hrd >= 1 && ft_strcmp("<<", *data_tmp) != 0)
			*hrd = 0;
		if (ft_strcmp("<<", *data_tmp) == 0)
			*hrd = 1;
		ft_bzero(*data_tmp, ft_strlen(*data_tmp));
	}
	return (TRUE);
}

int					tokenizer(int *hrd, char *read_buff, t_e_list **l_expr)
{
	if (DEBUG_TOKEN == 1)
		ft_putendl_fd("------- TOKENIZER ------", 2);
	char 				*data_tmp;

	if ((data_tmp = ft_strnew(ft_strlen(read_buff))) == NULL)
		return (sh_error(TRUE, 6, NULL, NULL));
	finite_state_automaton(hrd, &read_buff, l_expr, &data_tmp);
	if (data_tmp)
	{
		if (ft_strcmp("<<", data_tmp) == 0)
			*hrd = 1;
		expr_pushbk(l_expr, data_tmp, *hrd);
		ft_bzero(data_tmp, ft_strlen(data_tmp));
		if (*hrd >= 1 && ft_strcmp("<<", data_tmp) != 0)
			*hrd = 0;
	}
	ft_strdel(&data_tmp);

	// ANTIBUG !!!!!!!!!
	if (DEBUG_TOKEN == 1)
	{
		t_e_list *tmp = *l_expr;
		while (tmp)
		{
			printf("t[(%s)(%d)] -> ", tmp->data, *hrd);
			tmp = tmp->next;
		}
		printf ("\n");
	}
	// fin  ANTIBUG !!!!!!!!!
	return (TRUE);
}
