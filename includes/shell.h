#ifndef MINISHELL_H
# define MINISHELL_H

#define DEBUG 0
#include <stdio.h>

# define IGN " \t\n\0"
# define SEP "|&>< \t\n\0"
# define BUFF_SIZE 1024
# define MAX_PATH 1024

# define LEFT 414235
# define RIGHT 479771
# define UP 4283163
# define DOWN 4348699

# define BACKSPACE 127
# define TAB 9
# define RETURN 10
# define CTRL_D 4

// - MAC OPTION + [LEFT|RIGHT]
// # define OP_RIGHT 344
// # define OP_LEFT 345
// - LINUX CTRL + [LEFT|RIGHT]
# define OP_RIGHT 346
# define OP_LEFT 347

# define DEL 2117294875
# define HOME 293
# define END 296

# include "libft.h"

typedef enum
{
	NONE,
	EXP,
	RED,
	RED_ARG,
	CMD,
	CMD_ARG,
	PIPE,
	SEMI
} 						types;

typedef struct			s_e_list
{
	char				*data;
	types				type;
	struct s_e_list		*next;
}						t_e_list;

typedef struct			s_line
{
	char				*line;
	int					curs_x;
}						t_line;

typedef struct			s_history
{
	char				*line;
	struct s_history	*prev;
	struct s_history	*next;
}						t_history;

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
int				fct_read(t_line *line, t_duo **env_cpy, t_history **history);

int				father_n_son(char **cmd, t_duo **env_cpy);

int				bi_env(char **arg, t_duo **env);

int				bi_exit(char **arg, t_duo **env);

int				bi_setenv(char **arg, t_duo **env);

int				bi_unsetenv(char **arg, t_duo **env);

int				bi_cd(char **arg, t_duo **env);


int				init_term();
int				reset_term();
int				my_outc(int c);
int				event(int key, t_line *stline, t_history **history);
int				backspace(t_line *stline);
int				insert(t_line *stline, char c, int pos);
int				move(int key, t_line *stline);
int				spec_key(int key, t_line *stline);
void			add_history(t_history **history, char *line);
int				nav_history(int key, t_history **history, t_line *stline);


// X Couper, copier et/ou coller tout ou partie d’une ligne avec la séquence de touches qui vous plaira.
// X Ecrire ET éditer une commande sur plusieurs lignes. Dans ce cas, on apprecie-rait que ctrl+UP et ctrl+DOWN permettent de passer d’une ligne à l’autre de la commande en restant sur la même colonne ou la colonne la plus appropriée sinon.
// X Si une partie parenthésée de la commande n’est pas refermée avant l’appui sur la touche return, le shell revient à la ligne et attend la fin de la commande. Par partie parenthésée, on entend une partie de la commande entre quotes, doubles quotes, back quotes, parenthèses, crochets, accolades, etc.
// X ctrl+D et ctrl+C dans l’édition de la ligne (sachant que le ctrl+C pour arrêter un programme en cours, c’est bien aussi).

// O Editer la ligne à l’endroit où se trouve le curseur.
// O Déplacer le curseur vers la gauche et vers la droite pour pouvoir éditer la ligne à un endroit précis. Les nouveaux caractères doivent bien entendu s’insérer entre les caractères existants de la même manière que dans un shell ordinaire.
// O Se déplacer par "mot" vers la gauche et vers la droite avec ctrl+LEFT et ctrl+RIGHT ou toute autre combinaison de touche raisonnable.
// O Aller directement au début et à la fin d’une ligne avec home et end .
// O Utiliser les fleches du haut et du bas pour naviguer dans l’historique des commandes que l’on pourra alors éditer si le coeur nous en dit (la ligne, pas l’historique, hein).

#endif
