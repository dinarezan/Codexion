# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: drezan <drezan@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/15 14:47:02 by drezan            #+#    #+#              #
#    Updated: 2026/10/03 16:56:36 by drezan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc

CFLAGS = -pthread -Wall -Werror -Wextra
DEPFLAGS = -MMD -MP

NAME = codexion

SRCS = coder.c coder_utils_1.c coder_utils_2.c dongle.c heap.c monitor.c my_time.c parsing_validation.c main.c

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

-include $(DEPS)

all: $(NAME)

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug-asan: CFLAGS += -fsanitize=address -g -O0 -fno-omit-frame-pointer
debug-asan: re

debug-tsan: CFLAGS += -fsanitize=thread -g -O0
debug-tsan: re

.PHONY: all clean fclean re debug-asan debug-tsan