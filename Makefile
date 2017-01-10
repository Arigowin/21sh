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

CFLAGS = -g -Wall -Werror -Wextra -I $(HPATH) -I $(LIBHPATH)
LIBS = -L $(LIBPATH) -lft -ltermcap

SRC = sh_main.c \
	  sh_saviors.c \
	  sh_init.c \
	  sh_cd.c \
	  sh_env.c \
	  sh_exit.c \
	  sh_setenv.c \
	  sh_unsetenv.c \
	  sh_builtin.c \
	  sh_tokenizer.c \
	  sh_lexer.c \
	  sh_t_e_list_handler.c \
	  sh_parser.c \
	  sh_parser_additional_items.c \
	  sh_event.c \
	  sh_signal.c \
	  sh_history.c \
	  sh_fct_read.c \
	  sh_first_steps.c \
	  sh_father_n_son.c \
	  sh_main.c \
	  sh_insert_in_line.c \
	  sh_delete_in_line.c \
	  sh_move_in_line.c \
	  sh_move_up_down.c \
	  sh_setenv.c \
	  sh_signal.c \
	  sh_termcap.c \
	  sh_tputs.c \
	  sh_termcap.c \
	  sh_spec_key.c \
	  sh_copy_paste.c \
	  sh_add_del_in_cpy.c \
	  sh_highlight.c \
	  sh_move_in_line.c \
	  sh_cmd.c \
	  sh_red.c \
	  sh_manage_fd.c \
	  sh_red_handler.c\
	  sh_right_red.c \
	  sh_left_red.c \
	  sh_pipe.c \
	  sh_create_tree.c \
	  sh_free_tree.c \
	  sh_tree_traversal.c \
	  sh_heredoc.c

OFILES = $(patsubst %.c, $(OPATH)/%.o, $(SRC))

.PHONY: all clean fclean re lib.fclean

all: $(OPATH) $(NAME)

$(NAME): $(OFILES)
	@$(MAKE) -C $(LIBPATH) -j 8
	@echo "Creating OBJ files"
	@echo "Building $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "\033[36mAll is done!\033[0m"

$(OPATH)/%.o: $(CPATH)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OPATH):
	@echo "Creating OBJ directory"
	@$(MKDIR) $@

clean:
	@echo "Deleting OBJ files"
	@$(RM) -rf $(OPATH)

lib.fclean:
	@$(MAKE) fclean -C $(LIBPATH)

fclean: clean lib.fclean
	@echo "Deleting $(NAME)"
	@$(RM) -f $(NAME)
	@echo "\033[36mAll clear!\033[0m"

re: fclean all

norm: all
	norminette **/*.[ch]

#	  sh_tokenizer.c \
	  sh_lexer.c \#

# lil memo
# $@ = rule's name
# $^ = all the rule dependecies
# $< = only the first dependence
# -j 8 => pour que la lib complie en multi thread ;)


# Color for c
#   reset	"\033[0m"
#   RED  	"\033[31m"
#   GRN  	"\033[32m"
#   YEL  	"\033[33m"
#   BLU  	"\033[34m"
#   MAG  	"\033[35m"
#   CYN  	"\033[36m"
#   WHT  	"\033[37m"
