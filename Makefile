# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: egoh <marvin@42.fr>                        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/29 14:07:53 by egoh              #+#    #+#              #
#    Updated: 2024/02/29 14:10:56 by egoh             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Variables
NAME = cub3d
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -I./src/lib/ -I./src/render -I./src/parse
MINILIBX = minilibx-linux
SRC = src/lib/gnl/get_next_line.c src/lib/gnl/get_next_line_utils.c \
	src/lib/libc/ft_bzero.c src/lib/libc/ft_calloc.c \
	src/lib/libc/ft_endswith.c src/lib/libc/ft_isdigit.c \
	src/lib/libc/ft_memcpy.c src/lib/libc/ft_realloc.c \
	src/lib/libc/ft_split.c src/lib/libc/ft_strcmp.c \
	src/lib/libc/ft_strdup.c src/lib/libc/ft_strjoin.c \
	src/lib/libc/ft_strlen.c src/lib/libc/ft_strncmp.c \
	src/lib/libc/ft_strtrim.c src/parse/parse_file.c \
	src/parse/parse_header.c src/parse/parse_map.c \
	src/parse/parse_map_bitmask.c src/parse/parse_map_utils.c \
	src/parse/parse_path.c src/parse/parse_struct.c src/parse/parse_utils.c \
	src/render/render.c src/render/render_init.c \
	src/render/render_listener.c src/render/render_move.c \
	src/render/render_ray.c src/render/render_tex.c src/cleanup.c src/main.c \
	src/utils.c
OBJ = $(SRC:.c=.o)

# Rules
all: $(NAME)

$(NAME): $(OBJ)
	make -C $(MINILIBX)
	$(CC) $(CFLAGS) -o $@ $^ -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm

clean:
	rm -f $(OBJ)
	make -C $(MINILIBX) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
