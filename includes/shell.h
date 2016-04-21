#ifndef MINISHELL_H
# define MINISHELL_H

#define DEBUG 0
#include <stdio.h>

# define SEP " \t\n\0"
# define BUFF_SIZE 1024
# define MAX_PATH 1024
# define LEFT 186
# define RIGHT 185
# define UP 183
# define DOWN 184
# define BACKSPACE 127
# define TAB 9
# define DEL 126
# define RETURN 10
# define CTRL_D 4
# define OP_RIGHT 177
# define OP_LEFT 178

# include "libft.h"

typedef struct s_line
{
	char	*line;
	int		curs_x;
} t_line;

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
int				fct_read(t_line *line, t_duo **env_cpy);

int				father_n_son(char **cmd, t_duo **env_cpy);

int				bi_env(char **arg, t_duo **env);

int				bi_exit(char **arg, t_duo **env);

int				bi_setenv(char **arg, t_duo **env);

int				bi_unsetenv(char **arg, t_duo **env);

int				bi_cd(char **arg, t_duo **env);


int				init_term();
int				reset_term();
int				my_outc(int c);
int				event(char buf[], t_line *stline);
int				backspace(t_line *stline);
int				insert(t_line *stline, char c, int pos);
int				move(int key, t_line *stline);
int				del(t_line *stline);

// le char 'line' doit etre accible en permanance car besoin pour les signaux
//		^C (passe a la ligne suivante et vide 'line')
//		^D ( == del si 'line' contient qqchose)
// 'x' aussi doit etre disponible


/*
gauche : 27 91 68 : 186
droite : 27 91 67 : 185
haut : 27 91 65 : 183
bas : 27 91 66 : 184

backspace : 127 0 0
tab : 9 0 0
*/

#endif
