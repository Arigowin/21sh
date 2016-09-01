# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: avacher <avacher@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/25 17:44:48 by avacher           #+#    #+#              #
#    Updated: 2016/01/20 14:45:40 by avacher          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME = libft.a

CC =	/usr/bin/clang
RM =	/bin/rm
MAKE =	/usr/bin/make
MKDIR =	/bin/mkdir
AR =	/usr/bin/ar
RANLIB = /usr/bin/ranlib

SRC = ft_memset.c \
	  ft_bzero.c \
	  ft_memcpy.c \
	  ft_memccpy.c \
	  ft_memmove.c \
	  ft_memchr.c \
	  ft_memcmp.c \
	  ft_strlen.c \
	  ft_strdup.c \
	  ft_strcpy.c \
	  ft_strncpy.c \
	  ft_strcat.c \
	  ft_strncat.c \
	  ft_strlcat.c \
	  ft_strchr.c \
	  ft_strrchr.c \
	  ft_strstr.c \
	  ft_strnstr.c \
	  ft_strcmp.c \
	  ft_strncmp.c \
	  ft_atoi.c \
	  ft_isalpha.c \
	  ft_isdigit.c \
	  ft_isstrnum.c \
	  ft_isalnum.c \
	  ft_isascii.c \
	  ft_isprint.c \
	  ft_toupper.c \
	  ft_tolower.c \
\
	  ft_memalloc.c \
	  ft_memdel.c \
	  ft_memset.c \
	  ft_strnew.c \
	  ft_strdel.c \
	  ft_strclr.c \
	  ft_striter.c \
	  ft_striteri.c \
	  ft_strmap.c \
	  ft_strmapi.c \
	  ft_strequ.c \
	  ft_strnequ.c \
	  ft_strsub.c \
	  ft_strjoin.c \
	  ft_strtrim.c \
	  ft_strsplit.c \
	  ft_itoa.c \
	  ft_putchar.c \
	  ft_putstr.c \
	  ft_putendl.c \
	  ft_putnbr.c \
	  ft_putchar_fd.c \
	  ft_putstr_fd.c \
	  ft_putendl_fd.c \
	  ft_putnbr_fd.c \
\
	  ft_lstnew.c \
	  ft_lstdelone.c \
	  ft_lstdel.c \
	  ft_lstadd.c \
	  ft_lstiter.c \
	  ft_lstmap.c \
\
	  ft_putnbrendl.c \
	  ft_strnlen.c \
	  ft_lstpushback.c \
	  ft_itoa_base.c \
	  ft_split_to_lst.c \
	  tbl_to_duo.c \
	  srch_value.c \
	  srch_begining.c \
	  duo_pushbk.c \
	  duo_del.c \
	  free_duo.c \
	  free_tab.c \
	  free_lst.c \
	  lst_to_tbl.c \
	  ft_lst_count.c \
	  tbl_len.c \
	  ft_free_tbl.c \
	  ft_properjoin.c \
	  ft_atoi_base.c \
	  ft_hidenp.c \
	  ft_swap.c \
	  int_swap.c \
	  intcmp.c

OBJ = $(patsubst %.c, $(OPATH)/%.o, $(SRC))

CFLAGS = -Wall -Wextra -Werror -I $(HPATH)

ROOT =		$(shell /bin/pwd)
OPATH =		$(ROOT)/objs
CPATH =		$(ROOT)/srcs
HPATH =		$(ROOT)/includes

.PHONY: all clean fclean re

all: $(OPATH) $(NAME)

$(NAME): $(OBJ)
	@echo "Libft - Creating OBJ files"
	@echo "Libft - Building $@"
	@$(AR) rc $@ $(OBJ)
	@$(RANLIB) $@
	@echo "\033[34mLibft - All is done!\033[0m"
	
$(OPATH)/%.o: $(CPATH)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OPATH):
	@echo "Libft - Creating OBJ directory"
	@$(MKDIR) $@

clean:
	@echo "Libft - Deleting OBJS"
	@$(RM) -Rf $(OPATH)

fclean: clean
	@echo "Libft - Deleting $(NAME)"
	@$(RM) -f $(NAME)
	@echo "\033[34mLibft - All clear!\033[0m"

re: fclean all
