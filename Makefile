CC =	/usr/bin/clang
RM =	/bin/rm
MAKE =	/usr/bin/make
MKDIR =	/bin/mkdir

NAME = 21sh

ROOT =		$(shell /bin/pwd)
OPATH =		$(ROOT)/objs
CPATH =		$(ROOT)/srcs
HPATH =		$(ROOT)/includes
LIBPATH =	$(ROOT)/libft
LIBHPATH =	$(LIBPATH)/includes

CFLAGS = -O3 -Wall -Werror -Wextra -I $(HPATH) -I $(LIBHPATH)
LIBS = -L $(LIBPATH) -lft -ltermcap

SRC = sh_builtin.c \
	  sh_cd.c \
	  sh_env.c \
	  sh_event.c \
	  sh_exit.c \
	  sh_father_n_son.c \
	  sh_fct_read.c \
	  sh_first_steps.c \
	  sh_main.c \
	  sh_modif_line.c \
	  sh_move_in_line.c \
	  sh_setenv.c \
	  sh_signal.c \
	  sh_termcap.c \
	  sh_tputs.c \
	  sh_unsetenv.c

OFILES = $(patsubst %.c, $(OPATH)/%.o, $(SRC))

.PHONY: all clean fclean re lib.fclean

all: $(OPATH) $(NAME)

$(NAME): $(OFILES)
	@echo "$(NAME) : Building Libft"
	@$(MAKE) -C $(LIBPATH)
	@echo "$(NAME) : Building $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "\033[32mDone !\033[0m"

$(OPATH)/%.o: $(CPATH)/%.c
	@echo "$(NAME) : Creating file $@"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OPATH):
	@echo "$(NAME) : Creating objs directory"
	@$(MKDIR) $@

clean:
	@echo "$(NAME) : Deleting objs"
	@$(RM) -rf $(OPATH)

fclean: clean lib.fclean
	@echo "$(NAME) : Deleting $(NAME)"
	@$(RM) -f $(NAME)
	@echo "\033[32mDone !\033[0m"

lib.fclean:
	@$(MAKE) fclean -C $(LIBPATH)

re: fclean all
