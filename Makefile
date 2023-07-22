# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amaucher <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/21 10:58:29 by amaucher          #+#    #+#              #
#    Updated: 2023/07/21 10:58:31 by amaucher         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = data
SOURCES = main.c threads.c time.c utils.c
OBJS = $(SOURCES:.c=.o)

CC = cc -g -pthread
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)	

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)
