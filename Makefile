# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/09 15:14:04 by lgollong          #+#    #+#              #
#    Updated: 2025/01/20 15:48:18 by rwegat           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address
LIBFTDIR = ./libft
LIBFT = ./libft/libft.a
USER := $(shell whoami)

ifeq ($(USER), rmwegat)
	LDFLAGS  = -L/opt/homebrew/Cellar/readline/8.2.13/lib
	CPPFLAGS = -I/opt/homebrew/Cellar/readline/8.2.13/include
else
	LDFLAGS  = -L/Users/$(USER)/.brew/opt/readline/lib
	CPPFLAGS = -I/Users/$(USER)/.brew/opt/readline/include
endif

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
		
BSRCS =	srcs_bonus/main.c						\
		srcs_bonus/initialisation.c				\
		srcs_bonus/signal_handler.c				\
		\
		srcs_bonus/parser/get_path.c			\
		srcs_bonus/parser/parser.c				\
		srcs_bonus/parser/split_input.c			\
		srcs_bonus/parser/handle_metachars.c	\
		srcs_bonus/parser/remove_quotes.c		\
		srcs_bonus/parser/cmd_array_to_struct.c \
		srcs_bonus/parser/cmd_array_to_struct2.c\
		srcs_bonus/parser/open_files.c			\
		srcs_bonus/parser/get_here_doc.c 		\
		\
		srcs_bonus/parser/wildcard_bonus.c		\
		\
		srcs_bonus/executer/executer.c			\
		srcs_bonus/executer/executer2.c			\
		\
		srcs_bonus/helpers.c 					\
		srcs_bonus/helpers2.c 					\
		\
		srcs_bonus/builtins/cd.c				\
		srcs_bonus/builtins/echo.c				\
		srcs_bonus/builtins/env.c				\
		srcs_bonus/builtins/exit.c				\
		srcs_bonus/builtins/export.c			\
		srcs_bonus/builtins/ft_builtins.c		\
		srcs_bonus/builtins/pwd.c				\
		srcs_bonus/builtins/unset.c				\
		\
		
OBJS = $(SRCS:.c=.o)
BOBJS = $(BSRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	@make -s bonus -C $(LIBFTDIR)
	@echo "\033[1;32m ✅ [libft archive created]\033[0m"

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) $(CPPFLAGS) -g -fsanitize=address -lreadline -L$(LIBFTDIR) -lft -o $(NAME)
	@echo "\033[1;32m ✅ [minishell created]\033[0m"

bonus: $(LIBFT) $(BOBJS)
	@$(CC) $(BOBJS) $(LDFLAGS) $(CPPFLAGS) -g -fsanitize=address -lreadline -L$(LIBFTDIR) -lft -o $(NAME)
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

.SILENT: $(OBJS) $(BOBJS)

