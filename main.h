/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 09:47:07 by egoh              #+#    #+#             */
/*   Updated: 2026/06/19 08:00:00 by egoh             ###   ########.fr       */
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

# define INFO true

# define KEY_IDX_W     0
# define KEY_IDX_A     1
# define KEY_IDX_S     2
# define KEY_IDX_D     3
# define KEY_IDX_LEFT  4
# define KEY_IDX_RIGHT 5
# define KEY_COUNT     6

// Init struct errors
# define MLX_INIT_ERR "Failed to initalize MLX"
# define MLX_WIN_ERR "Failed to create MLX window"
# define MLX_SCREEN_ERR "Failed to create screen buffer"

// Generic Errors
# define RET_ERR -1
# define MALLOC_ERR "Memory allocation failed"
# define NO_FILE "No file found"

/**
 * Enum used for the map grid.
 * - TILE_SPACE represents a whitespace
 * - TILE_EMPTY represents '0'
 * - TILE_WALL represents '1'
 */
typedef enum e_tile
{
	TILE_SPACE,
	TILE_EMPTY,
	TILE_WALL
}	t_tile;

/**
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
	int		last_mouse_x;
	bool	mouse_captured;
}	t_plyr;

/**
 * Uses a 1D flat array to remove pointer chasing every frame and
 * uses uint8_t to save 75% memory compared to integers
 */
typedef struct s_map
{
	uint8_t	*grid;
	size_t	height;
	size_t	width;
}	t_map;

// MLX Image struct storage
typedef struct s_img
{
	void	*img_ptr;
	int		*pixels;
	int		line_len;
	int		bpp;
	int		endian;
	int		width;
	int		height;
}	t_img;

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
}	t_config;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_img		screen;
	t_config	config;
	char		**envp;
	bool		keys[KEY_COUNT];
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

// Parsing & Validation
int		parse_file(t_game *game, const char *p);

// Main Logic
int		render(t_game *game);

// Cleanup
void	cleanup(t_game *game);

// Utils
void	free_arr(char **arr, int start);
int		is_empty(const char *s);
int		parse_error(const char *msg);
void	*dynamic_realloc(void **ptr, size_t size, size_t *capacity);

#endif