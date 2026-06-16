/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 00:33:06 by egoh              #+#    #+#             */
/*   Updated: 2026/06/06 06:17:13 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <fcntl.h>					// open flags
# include <unistd.h>				// open, close, read, write
# include <stdio.h>					// printf, perror
# include <stdlib.h>				// malloc, free, exit
# include <string.h>				// strerror
# include <errno.h>					// errno for strerror
# include <sys/time.h>				// gettimeofday
# include <math.h>					// math library functions
# include "minilibx-linux/mlx.h"	// MLX library functions
# include <X11/keysym.h>			// XK_ key constants
# include <X11/X.h>					// X11 events constants
# include <stdbool.h>				// bool type
# include <limits.h>				// limits
# include <float.h>					// float limits
# include <stdint.h>				// fixed-width integer types
# include "libc/libc.h"				// Libc functions

# define WIN_W 800
# define WIN_H 600
# define WIN_TITLE "cub3d"
# define DEBUG true

// Error messages
# define RET_ERR -1
# define MALLOC_ERR "Memory allocation failed"
# define NO_FILE "No file found"

typedef enum e_tile
{
	TILE_SPACE,		// ' '
	TILE_EMPTY,		// '0'
	TILE_WALL		// '1'
}	t_tile;

typedef struct s_coords
{
	size_t	x;
	size_t	y;
}	t_xy;

/*
 * Struct used for the player entity.
 * - pos_x & pos_y is used as world vectors.
 * - dir_x & dir_y is used as direction vectors.
 * - planeX & planeY is used as camera plane vectors.
 */
typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}   t_plyr;

typedef struct s_map
{
	uint8_t	*grid;  // 1D flat array (saves 75% memory vs int)
	size_t	height;
	size_t	width;
}   t_map;

// MLX Image struct storage
typedef struct s_img
{
	void	*mlx_img;
	int		*pixels;
	int		line_len;
	int		bpp;
	int		endian;
}   t_img;

// Main config (floor_color & ceil_color stores packed RGB ints)
typedef struct s_config
{
	t_map	map;
	t_plyr	player;
	t_img	tex_no;
	t_img	tex_so;
	t_img	tex_we;
	t_img	tex_ea;
	int		floor_color;
	int		ceil_color;
}   t_config;

typedef struct s_game
{
	void        *mlx;
	void		*win;
	t_img		screen;
	t_config	config;
	char		**envp;
}	t_game;

typedef enum e_key
{
	KEY_ESC = XK_Escape,
	KEY_W = XK_w,
	KEY_A = XK_a,
	KEY_S = XK_s,
	KEY_D = XK_d,
	KEY_LEFT = XK_Left,
	KEY_RIGHT = XK_Right,
}	t_key;

// Initializers
void	init_listener(t_game *game);
int		parse_file(t_game *game, const char *p);

// Cleanup
void	cleanup(t_game *game);

// Utils
void	free_arr_range(char **arr, size_t start, size_t end);
void	free_arr(char **arr, int start);
int		is_empty(const char *s);
int		parse_error(const char *msg);
void	*dynamic_realloc(void **ptr, size_t size, size_t *capacity);
#endif
