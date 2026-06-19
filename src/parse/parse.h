/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 15:33:01 by egoh              #+#    #+#             */
/*   Updated: 2026/06/19 04:02:39 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "main.h"

# define PARSE_DEBUG true

// Constant params to avoid magic variables
# define TILE_BITS 0b10
# define INIT_CAP 0b100000
# define TEXTURE_EXT ".xpm\n" // GNL adds a nl at the end
# define FACES "NSEW"
# define INIT_COLOR INT_MIN

# define RED_SHIFT 0b10000
# define GREEN_SHIFT 0b01000
# define BLUE_SHIFT 0b00000
# define COLOR_MAX 255
# define COLOR_MAX_LEN 3

# define RED_MASK 0xFF0000
# define GREEN_MASK 0x00FF00
# define BLUE_MASK 0x0000FF

// Invalid file
# define INVALID_CONFIG_EXT "File must have .cub extension"
# define OPEN_CONFIG_ERR "Could not open file"

// Invalid Headers
# define INVALID_ELEM_F "Invalid element format"
# define INVALID_ELEM_I "Invalid element identifier"
# define H_SPLIT_ERR "Failed to split headers"
# define MISSING_NO "Missing NO texture"
# define MISSING_SO "Missing SO texture"
# define MISSING_WE "Missing WE texture"
# define MISSING_EA "Missing EA texture"
# define MISSING_FLOOR "Missing floor color"
# define MISSING_CEIL "Missing ceiling color"
# define DUP_COLOR_ID "Duplicate color identifier"
# define COLOR_RANGE "Color value out of range [0, 255]"
# define COLOR_LEN "Color must only have 3 components"

// Invalid texture file
# define NO_TEXTURE_FILE "No texture file provided"
# define DUP_TEXTURE_ID "Duplicate texture identifier"
# define TEXTURE_MALLOC_ERR "Failed to allocate memory for texture"
# define INVALID_TEXTURE_EXT "Only .xpm texture files are allowed"
# define OPEN_TEXTURE_ERR "Could not open texture file"
# define TEXTURE_DIR_ERR "Error reading texture file"
# define NO_ENV_ERR "Couldn't find home on env for texture path"
# define HOME_MALLOC_ERR "Failed to allocate memory for home path expansion"
# define CONVERT_MALLOC_ERR "Failed to allocate memory when converting texture"

// Invalid Map
# define GNL_EOL_ERR "Unexpected EOL while finding map line"
# define NO_MAP "No map found in the file"
# define M_JOIN_ERR "Failed to store map line"
# define MASK_MALLOC_ERR "Failed to allocate memory for map validator mask"
# define GRID_MALLOC_ERR "Failed to allocate memory for the grid"
# define INVALID_MAP_CHAR "Invalid map character"
# define MAP_READ_ERR "Read failure while parsing map layout"
# define EMPTY_LINE_MAP "Empty lines are not allowed in the map"
# define NR_RESIZE_ERR "Failed to resize grid to fit a new grid row"
# define NEW_ROW_MALLOC_ERR "Failed to allocate memory for grid row"
# define ROW_RESIZE_ERR "Failed to resize grid row"
# define MASK_RESIZE_ERR "Failed to resize mask for map validator"
# define MAP_PLYR "Map must only have 1 player"
# define MAP_NO_PLYR "Map does not have a player"

/**
 * Enum used for the map grid during parsing.
 * - TILE_P_NULL is used for null terminating each row
 * - TILE_P_SPACE represents a whitespace
 * - TILE_P_EMPTY represents '0'
 * - TILE_P_WALL represents '1'
 * - TILE_P_PLAYER represents 'N', 'S', 'E', 'W'
 */
typedef enum e_p_tile
{
	TILE_P_NULL,
	TILE_P_SPACE,
	TILE_P_EMPTY,
	TILE_P_WALL,
	TILE_P_PLAYER
}	t_p_tile;

typedef struct s_coords
{
	size_t	x;
	size_t	y;
}	t_xy;

typedef struct s_p_map
{
	int			fd;
	char		*map_line;
	char		*bit_valid;
	t_p_tile	**grid;
	size_t		*grid_len;
	size_t		max_x;
	t_xy		curr;
	t_xy		size;
	t_xy		cap;
}	t_p_map;

// Parse Headers
int		parse_headers(const int fd, t_game *game, char **map_line);
int		parse_map(t_config *config, int fd, char *map_line);
int		parse_path(const char *value, t_img *img, void *mlx, char **envp);

// Parse map bitmask getters & setters
int		get_mask_state(const char *bit_valid, size_t x);
void	set_mask_state(char *bit_valid, size_t x, int state);
t_xy	get_curr_coord(const char *curr_coord);
void	set_curr_coord(char *curr_coord, size_t x, size_t y);

// Parse map utils
char	*get_next_line(int fd, int ret_stash);
int		read_char(t_p_map *p_map);
int		next_row(t_p_map *p_map, size_t *grid_cap);
void	migrate_bitmask_states(t_p_map *p_map);
int		is_map_line(const char *line);

// General parse utils
int		check_commas(const char *line);
int		hole_loc_err(size_t x, size_t y);

// Map struct manipulation
int		init_map(t_config *config, t_p_map *p_map, int fd, char *map_line);
int		flatten_map(t_map *map, t_p_map *p_map);
void	cleanup_p_map(t_p_map *p_map);

#endif