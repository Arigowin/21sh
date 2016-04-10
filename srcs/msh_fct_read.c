#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"
#include "libft.h"

static int		create_list(char cheat[], char (*tmp)[], int *k, t_list **arg)
{
	if (cheat[0] == cheat[1] && (*tmp)[0] != '\0')
	{
		ft_lstpushback(arg, *tmp);
		ft_bzero(*tmp, 1024);
		*k = 0;
	}
	else if (cheat[0] != cheat[1])
		(*tmp)[(*k)++] = cheat[0];
	return (0);
}

char			**read_n_check(char *special, char *read_buff)
{
	t_list			*arg;
	char			tmp[1024];
	char			cheat[2];
	char			**tbl;
	int				i[3];

	ft_bzero(tmp, 1024);
	arg = NULL;
	i[0] = -1;
	i[2] = 0;
	while (read_buff[++i[0]])
	{
		i[1] = 0;
		while (read_buff[i[0]] != special[i[1]] && special[i[1]])
			i[1]++;
		cheat[0] = read_buff[i[0]];
		cheat[1] = special[i[1]];
		create_list(cheat, &tmp, &(i[2]), &arg);
	}
	if (ft_strlen(tmp))
		ft_lstpushback(&arg, tmp);
	tbl = lst_to_tbl(arg);
	free_lst(&arg);
	return (tbl);
}

int				check_home(char **cmd)
{
	int			i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '~')
		{
			ft_putstr("minishell: ");
			ft_putstr(cmd[0]);
			ft_putendl(": no $HOME variable set");
			return (-1);
		}
		i++;
	}
	return (0);
}

int				fct_read(char *read_buff, char **env, t_duo **env_cpy)
{
	int				ret;
	char			**cmd;
	int				i;

	i = 0;
	ret = 0;
	while ((ret = read(0, read_buff, BUFF_SIZE)) > 0)
	{
		if ((cmd = read_n_check(SEP, read_buff)) == NULL || cmd[0] == NULL)
			return (-1);
		while (cmd[++i])
			if (cmd[i][0] == '~')
				manage_tilde(env_cpy, &cmd[i]);
		if (handle_builtin(cmd, env_cpy) != 0)
			break ;
		if (check_home(cmd) < 0)
			break ;
		father_n_son(cmd, env, env_cpy);
		break ;
	}
	if (ret <= 0)
		bi_exit(NULL, env_cpy);
	free_tab(&cmd);
	return (0);
}

/*
gauche : 27 91 68 : 186
droite : 27 91 67 : 185
haut : 27 91 65 : 183
bas : 27 91 66 : 184

backspace : 127 0 0
tab : 9 0 0
*/
