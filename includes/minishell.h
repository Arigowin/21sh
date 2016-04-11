#ifndef MINISHELL_H
# define MINISHELL_H

# define SEP " \t\n\0"
# define BUFF_SIZE 1024
# define MAX_PATH 1024

# include "libft.h"

t_duo			*savior(t_duo *env);

int				display_prompt(t_duo *env);
char			**cpy_env(char **env);
int				manage_tilde(t_duo **env, char **arg);
int				fill_path(char ***env);

int				check_signal(int loc);

int				add_env(t_duo **env, char *name, char *value);
int				change_env(t_duo **env, char *name, char *value);
char			*get_env(t_duo **env, char *name);
int				handle_builtin(char **cmd, t_duo **env);

char			**read_n_check(char *special, char *read_buff);
int				fct_read(char *read_buff, char **env, t_duo **env_cpy);

int				father_n_son(char **cmd, char **env, t_duo **env_cpy);

int				bi_env(char **arg, t_duo **env);

int				bi_exit(char **arg, t_duo **env);

int				bi_setenv(char **arg, t_duo **env);

int				bi_unsetenv(char **arg, t_duo **env);

int				bi_cd(char **arg, t_duo **env);


int				init_term();
int				reset_term();
int				my_outc(int c);
int				event(char buf[], char **line, int *x);
int				backspace(char **line, int *x);
int				insert(char **line, char c, int pos);
int				move(int key, int *x, char *line);


#endif
