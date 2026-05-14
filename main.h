/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 00:33:06 by egoh              #+#    #+#             */
/*   Updated: 2026/05/12 10:20:04 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <fcntl.h>					// open flags
# include <unistd.h>				// open, close, read, write
# include <stdio.h>					// printf, perror
# include <stdlib.h>				// malloc, free, exit
# include <string.h>				// strerror
# include <sys/time.h>				// gettimeofday
# include <math.h>					// math library functions
# include "minilibx-linux/mlx.h"	// MLX library functions
# include <X11/keysym.h>			// XK_ key constants
# include <X11/X.h>					// X11 events constants
# include <stdbool.h>

#define WIN_W 800
#define WIN_H 600
#define WIN_TITLE "cub3d"
#define DEBUG true

// Error messages
#define MALLOC_ERR "Memory allocation failed"
#define COLOR_RANGE "Color value out of range [0, 255]"
#define COLOR_LEN "Color must only have 3 components"
#define INVALID_MAP "Invalid map character!"
#define MAP_PLYR "Map must only have 1 player!"
#define MISSING_NO "Missing NO texture"
#define MISSING_SO "Missing SO texture"
#define MISSING_WE "Missing WE texture"
#define MISSING_EA "Missing EA texture"

typedef enum e_tile
{
	TILE_SPACE,		// ' '
	TILE_EMPTY,		// '0'
	TILE_WALL,		// '1'
	TILE_PLAYER_N,	// 'N'
	TILE_PLAYER_S,	// 'S'
	TILE_PLAYER_E,	// 'E'
	TILE_PLAYER_W,	// 'W'
}	t_tile;

typedef struct s_config
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		floor_color[3];
	int		ceil_color[3];
	t_tile	**map;
	int		map_w;
	int		map_h;
}	t_config;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_config	config;
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
} t_key;

// Initializers
void	init_listener(t_game *game);

// Cleanup
void	cleanup(t_game *game);

// Parser
int		parse_element(t_config *config, const char *line);
int		parse_file(t_game *game, const char *p);
int		parse_map(t_config *cfg, char **lines, int count, int i);
int		parse_path(char **dest, const char *value);

// Validators
int		validate_elements(const t_config *config);

// Utils
void	free_arr_range(char **arr, int start, int end);
void	free_arr(char **arr, int start);
int		is_empty(const char *s);
int		parse_error(const char *msg);
void	*dynamic_realloc(void **ptr, size_t size, size_t *capacity);

// LIBC Functions
int		ft_atoi(const char *s);
void	ft_bzero(void *s, size_t len);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	**ft_split(char const *s, char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strtrim(char const *s1, char const *set);
char	*get_next_line(int fd);
#endif
