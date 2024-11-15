# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elgollong <elgollong@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/09 15:14:04 by tfriedri          #+#    #+#              #
#    Updated: 2024/11/15 15:58:19 by elgollong        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra
LIBFTDIR = ./libft
LIBFT = ./libft/libft.a
LDFLAGS		= -L/Users/$(USER)/.brew/opt/readline/lib
CPPFLAGS	= -I/Users/$(USER)/.brew/opt/readline/include
# LDFLAGS		= -L/opt/homebrew/Cellar/readline/8.2.13/lib
# CPPFLAGS	= -I/opt/homebrew/Cellar/readline/8.2.13/include

SRCS =	srcs/main.c							\
		srcs/initialisation.c				\
		srcs/signal_handler.c				\
		\
		srcs/parser/get_path.c				\
		srcs/parser/parser.c				\
		srcs/parser/split_input.c			\
		srcs/parser/handle_metachars.c		\
		srcs/parser/remove_quotes.c			\
		srcs/parser/cmd_array_to_struct.c 	\
		srcs/parser/open_files.c			\
		srcs/parser/get_here_doc.c 			\
		\
		srcs/parser/wildcard_bonus.c		\
		\
		srcs/executer/executer.c			\
		\
		srcs/helpers.c 						\
		srcs/helpers2.c 					\
		\
		srcs/builtins/cd.c					\
		srcs/builtins/echo.c				\
		srcs/builtins/env.c					\
		srcs/builtins/exit.c				\
		srcs/builtins/export.c				\
		srcs/builtins/ft_builtins.c			\
		srcs/builtins/pwd.c					\
		srcs/builtins/unset.c				\

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	@make -s bonus -C $(LIBFTDIR)
	@echo "\033[1;32m ✅ [libft archive created]\033[0m"

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) $(CPPFLAGS) -lreadline -L$(LIBFTDIR) -lft -o $(NAME)
	@echo "\033[1;32m ✅ [minishell created]\033[0m"

clean:
	@rm -f $(OBJS)
	@rm -f $(BOBJS)
	@echo "\033[0;31m 🗑️  [OBJS deleted]\033[0m"
	@cd $(LIBFTDIR) && make clean
	@echo "\033[0;31m 🗑️  [libft deleted]\033[0m"

fclean: clean
	@rm -f $(NAME)
	@cd $(LIBFTDIR) && make fclean
	@echo "\033[0;31m 🗑️  [minishell deleted]\033[0m"

re: fclean all

.SILENT: $(OBJS)
