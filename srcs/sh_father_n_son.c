#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

static char		*join_exe(char *s1, char *s2)
{
	if (DEBUG == 1)
		printf("------- JOIN EXE ------\n");
	char		*rlt;
	char		*tmp;

	if (s2[0] == '/' || (s2[0] == '.' && s2[1] == '/'))
		return (s2);
	tmp = ft_strjoin("/", s2);
	rlt = ft_strjoin(s1, tmp);
	ft_strdel(&tmp);
	return (rlt);
}

static int		check_fct(char **cmd)
{
	if (DEBUG == 1)
		printf("------- CHECK FCT------\n");

	char		**path;
	char		*tmp;
	t_duo		*env;
	int			i;
	char		**tbl_env;

	path = NULL;
	env = savior(NULL, FALSE);
	tbl_env = duo_to_tbl(env, "=");
	tmp = get_env("PATH");
	if (tmp == NULL)
		return (-1);
	path = ft_strsplit(tmp, ':');
	free(tmp);
	i = 0;
	while (path[i])
	{
		tmp = join_exe(path[i], cmd[0]);
		execve(tmp, cmd, tbl_env);
		free(tmp);
		i++;
	}
	return (0);
}

int				father_n_son(char **cmd)
{
	if (DEBUG == 1)
		printf("------- FATHER N SON ------\n");
	pid_t		father;
	int			stat_loc;

	if ((father = fork()) < 0)
		return (ERROR);
	if (father > 0)
	{
		check_signal(3);
		wait(&stat_loc);
	}
	if (father == 0)
	{
		check_signal(2);
		check_fct(cmd);
		// appeler la fonction d'erreur
		ft_putstr_fd("21sh: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(EXIT_FAILURE);
	}
	return (TRUE);
}


int				father_n_son_for_pipe(char **cmd)
{
	if (DEBUG == 1)
		printf("------- FATHER N SON FOR PIPE ------\n");

	check_fct(cmd);
	ft_putstr_fd("21sh: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);

	return (0);
}

int				check_builtin(t_node *tree, t_lst_fd **lstfd, char **cmd)
{
	//	if (DEBUG == 1)
	printf("------- CHECK BUILTIN ------\n");

	int			ret;

	ret = -1;
	if (is_builtin(cmd) != -1)
	{
		printf("OK2\n");
		if ((ret = handle_builtin(cmd)) == ERROR)
		{
			printf("OK3\n");
			close_lstfd(lstfd);
			if (tree->left != NULL)
				exit(ret);
			return (ERROR);
		}
		printf("OK4\n");
		if (tree->left != NULL)
			exit(ret);
		printf("OK5\n");
		return (TRUE);
	}
	printf("OK6\n");
	return (FALSE);
}

int				father(void)
{
	if (DEBUG == 1)
		printf("------- FATHER ------\n");

	int			stat_loc;

	stat_loc = 0;
	check_signal(3);
	while (waitpid(-1, &stat_loc, WNOHANG) >= 0)
		;
	if (WIFSIGNALED(stat_loc))
		return (-2); // revoir le code de retour
	if (WIFEXITED(stat_loc) != TRUE)
		return (ERROR);
	return (WEXITSTATUS(stat_loc));
}

int				son(t_node *tree, t_lst_fd **lstfd, char **cmd)
{
	if (DEBUG == 1)
		printf("------- SON ------\n");

	if (tree->left != NULL)
		if (redirect(tree->left, lstfd) == ERROR)
			//			close_fd_red(&lstfd, saved_std);
			return (ERROR);

	printf("WHY????????\n");
	if (check_builtin(tree, lstfd, cmd) == TRUE)
		return (TRUE);
	check_signal(2);
	check_fct(cmd);
	// appeler la fonction d'erreur
	ft_putstr_fd("21sh: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putendl_fd(": command not found", 2);
	exit(EXIT_FAILURE);
	return (FALSE);
}

int				handle_fork(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG == 1)
		printf("------- HANDLE FORK ------\n");

	pid_t			fpid;
	char			**cmd;

	fpid = -1;
	cmd = NULL;
	if ((cmd = format_cmd(tree)) == NULL)
		return (ERROR);
//	if (tree->left == NULL)
//	{
		if (check_builtin(tree, lstfd, cmd) == TRUE)
		{
			printf("OK1\n");
			return (TRUE);
		}
//	}
	if ((fpid = fork()) < 0)
		return (ERROR);
	if (fpid == 0)
	{
		son(tree, lstfd, cmd);
	}
	else
	{
		father();
	}
	return (TRUE);
}
