#include "libft.h"
#include "shell.h"

int				fill_heredoc(t_line *stline)
{
	if (DEBUG_HEREDOC == 1)
		printf("------------ FILL HEREDOC ----------\n");
//
//	if (stline->pos_line > 1 && (stline->hrd.ptr) != NULL)
//		printf("adresse : %p, line : %c, nb : %d, ptr : [%c], [%s]\n", stline->hrd.ptr, stline->line[stline->pos_line - 1], stline->hrd.nb, *(stline->hrd.ptr), ft_strchr(IGN, stline->line[stline->pos_line - 1]));

	char				*tmp;

	tmp = NULL;
	if (stline->line[stline->pos_line - 1] && stline->line[stline->pos_line - 2]
	&& stline->line[stline->pos_line - 1] == '<'
	&& stline->line[stline->pos_line - 2] == '<')
	{
		(stline->hrd.nb)++;
		stline->hrd.ptr = &(stline->line[stline->pos_line - 1]);
	}
	else if (stline->hrd.ptr != NULL && *(stline->hrd.ptr) == '<'
	&& ft_strchr(IGN, stline->line[stline->pos_line - 1]) == NULL)
	{
		stline->hrd.ptr = &(stline->line[stline->pos_line - 1]);
	}
	else if (stline->hrd.ptr != NULL && *(stline->hrd.ptr) != '<'
	&& ft_strchr(SEP, stline->line[stline->pos_line - 1]) != NULL)
	{
		tmp = ft_strsub(stline->hrd.ptr, 0, ft_strlen(stline->hrd.ptr) - 1);
		ft_lstadd(&(stline->hrd.deli), ft_lstnew(tmp));
		ft_strdel(&tmp);
		stline->hrd.ptr = NULL;
	}
	return (TRUE);
}

int			return_heredoc(t_line *stline)
{
	(void)stline;
	return (TRUE);
}
