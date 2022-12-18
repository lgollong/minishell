# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/09 15:14:04 by tfriedri          #+#    #+#              #
#    Updated: 2022/12/17 19:18:06 by tfriedri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra
LIBFTDIR = ./libft
LIBFT = ./libft/libft.a
LDFLAGS		= -L/Users/$(USER)/.brew/opt/readline/lib
CPPFLAGS	= -I/Users/$(USER)/.brew/opt/readline/include

SRCS =	srcs/main.c							\
		srcs/initialisation.c				\
		srcs/signal_handler.c				\
		srcs/parser/get_path.c				\
		srcs/parser/parser.c				\
		srcs/parser/split_input.c			\
		srcs/parser/handle_metachars.c		\
		srcs/parser/remove_quotes.c			\
		srcs/parser/cmd_array_to_struct.c 	\
		srcs/parser/open_files.c			\
		srcs/parser/get_here_doc.c 			\
		srcs/executer/executer.c			\
		srcs/helpers.c 						\
		srcs/helpers2.c 					\
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
	@make bonus -C $(LIBFTDIR)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) $(CPPFLAGS) -lreadline -L$(LIBFTDIR) -lft -o $(NAME)

clean:
	@rm -f $(OBJS)
	@cd $(LIBFTDIR) && make clean

fclean: clean
	@rm -f $(NAME)
	@cd $(LIBFTDIR) && make fclean

re: fclean all
