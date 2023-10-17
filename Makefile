# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/23 13:24:02 by amaucher          #+#    #+#              #
#    Updated: 2023/08/23 13:24:07 by amaucher         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SOURCES = main.c actions.c initialization.c threads.c utils.c input_check.c \
		supervisor.c time.c \

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