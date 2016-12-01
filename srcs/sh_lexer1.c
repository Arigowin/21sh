#include <unistd.h>
#include <stdlib.h>
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
	if (content == NULL)
		return (NULL);
	new->data = ft_strdup(content);
	return (new);
}
/*
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
*/

int						concat(char **dest, char *s1, char *s2)
{
	if (!(dest && *dest))
		return (-1);
	while (s1 && *s1)
	{
		add_in_tbl(dest, *s1);
		(*s1)++;
	}
	while (s2 && *s2)
	{
		add_in_tbl(dest, *s2);
		(*s2)++;
	}
	return (0);
}

int					lexer_dollar(char **read_buff, char **data_tmp)
{
	char 				*env_name;
	char 				*env_val;
	char 				*tmp;

	env_val = NULL;
	env_name = NULL;
	tmp = NULL;
	(**read_buff)++;
	if ((env_name = ft_strnew(ft_strlen(*read_buff))) == NULL)
		return (ERROR);
	while (ft_strchr(SEP, **read_buff))
	{
		add_in_tbl(&env_name, **read_buff);
		(**read_buff)++;
	}
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

	if ((*read_buff)[0] == '\\' && (*read_buff)[1] && (*read_buff)[1] == '\n')
		(**read_buff)++;
	(**read_buff)++;
	add_in_tbl(data_tmp, **read_buff);
	return (TRUE);
}

int					lexer_standard(char **read_buff, char **data_tmp,
					int *bln)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER STANDARD ------\n");

	if (**read_buff == DQUOTE)
		return (FALSE);
	if (**read_buff == '\\')
		lexer_backslash(read_buff, data_tmp);
	else if (**read_buff == '$')
		lexer_dollar(read_buff, data_tmp);
	else if (**read_buff == '~' && bln == FALSE)
		lexer_tilde(read_buff, data_tmp, bln);
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
	if (state == STANDARD && **read_buff == '\\')
		return (W_BACKSLASH);
	if ((state == IN_DQUOTE && **read_buff == '"') || (state == IN_QUOTE && **read_buff == '\''))
	{
		(*read_buff)++;
		return (STANDARD);
	}
	return (state);
}

int						lexer_1(char *read_buff, t_e_list **l_expr)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- IN LEXER 1 ------\n");
	int						bln;
	states					state;
	char 					*data_tmp;

	(void)l_expr;
	bln = FALSE;
	state = STANDARD;
	data_tmp = ft_strnew(ft_strlen(read_buff));
	while (read_buff && *read_buff)
	{
		state = get_state(state, &read_buff);
		if (state == STANDARD)
			lexer_standard(&read_buff, &data_tmp, &bln);
		else if (state == IN_QUOTE)
			lexer_quote(*read_buff, &data_tmp);
		else if (state == IN_DQUOTE)
			lexer_dquote(&read_buff, &data_tmp);
		read_buff++;
	}
	return (TRUE);
}
