#include "shell.h"
#include "libft.h"
#include <term.h>

static int		hide_highlight(t_line *stline)
{
	char		*tmp;
	int			i;

	tmp = ft_strdup(stline->line);
	while ((stline->curs_x - 3) < (int)ft_strlen(stline->line))
	{
		move(RIGHT, stline);
	}
	while ((stline->curs_x) > 3)
	{
		backspace(stline);
	}
	i = 0;
	while (tmp[i])
	{
		insert(stline, tmp[i], ++(stline->curs_x) - 4);
		i++;
	}
	return (TRUE);
}

static int		highlight(t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- HIGHLIGHT ------\n");
	if (stline->cpy_start == FALSE)
	{
		if ((stline->curs_x - 3) < (int)ft_strlen(stline->line))
		{
			// start highlight
			tputs(tgetstr("mr", NULL), 1, my_outc);

			ft_putchar((stline->line)[stline->curs_x - 3]);
			stline->copy[0] = (stline->line)[stline->curs_x - 3];
			stline->curs_x++;
			move(LEFT, stline);
			stline->cpy_pos = 1;
			stline->cpy_start = TRUE;

			return (TRUE);
		}
	}
	else
	{
		tputs(tgetstr("me", NULL), 1, my_outc);
		free(stline->copy);
		stline->copy = NULL;
		stline->cpy_pos = 0;
		stline->cpy_start = FALSE;
		hide_highlight(stline);
		return (TRUE);
	}
	return (FALSE);
}

static int		copy(t_line *stline)
{
	tputs(tgetstr("me", NULL), 1, my_outc);
	stline->cpy_start = FALSE;
	hide_highlight(stline);
	return (TRUE);
}

static int		paste(t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- PASTE ------\n");
	int		i;

	i = 0;
	while (stline->copy[i])
	{
		insert(stline, stline->copy[i], ++(stline->curs_x) - 4);
		i++;
	}
	tputs(tgetstr("me", NULL), 1, my_outc);
	return (TRUE);
}

int				copy_paste(int key, t_line *stline)
{
	if (DEBUG_COPY_PASTE == 1)
		printf("------- COPY_PASTE ------\n");

	if (stline->copy == NULL)
	{
		stline->copy = ft_strnew(ft_strlen(stline->line));
		stline->cpy_pos = 0;
		stline->cpy_start = FALSE;
	}

	if (key == HIGHLIGHT)
		highlight(stline);
	else if (key == PASTE && stline->copy && stline->cpy_start == FALSE)
	{
		paste(stline);
	}
	else if (key == COPY && stline->cpy_start == TRUE)
	{
		copy(stline);
	}
	else if (key == CUTE && stline->cpy_start == TRUE)
	{
		printf("CUTE\n");
	}
	return (TRUE);
}
