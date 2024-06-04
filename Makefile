# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/23 13:24:02 by amaucher          #+#    #+#              #
#    Updated: 2024/06/04 22:12:04 by amaucher         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SOURCES = ./src/main.c ./src/actions.c ./src/initialization.c \
			./src/threads.c ./src/utils.c ./src/input_check.c \
			./src/supervisor.c ./src/time.c \

OBJS = $(SOURCES:.c=.o)

CC = cc -g
RM = rm -f
CFLAGS = -Wall -Wextra -Werror -pthread

all: $(NAME)	

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)