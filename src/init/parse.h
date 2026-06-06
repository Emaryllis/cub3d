/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 15:33:01 by egoh              #+#    #+#             */
/*   Updated: 2026/05/15 15:33:01 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "main.h"
# include "gnl/get_next_line.h"

// Constant params to avoid magic numbers
# define TILE_BITS 2
# define INIT_CAP 64

// Invalid Headers
# define INVALID_ELEM_F "Invalid element format"
# define INVALID_ELEM_I "Invalid element identifier"
# define MISSING_NO "Missing NO texture"
# define MISSING_SO "Missing SO texture"
# define MISSING_WE "Missing WE texture"
# define MISSING_EA "Missing EA texture"
# define COLOR_RANGE "Color value out of range [0, 255]"
# define COLOR_LEN "Color must only have 3 components"

// Malloc failure
# define MASK_MALLOC_ERR "Failed to allocate memory for map validator mask"
# define GRID_MALLOC_ERR "Failed to allocate memory for the grid"
# define H_SPLIT_ERR "Failed to split headers"
# define H_GNL_ERR "Failed to get header line"
# define M_GNL_ERR "Failed to store map line"
# define NR_RESIZE_ERR "Failed to resize grid in new_row"
# define NEW_ROW_MALLOC_ERR "Failed to allocate memory for next row"
# define EBC_RESIZE_ERR "Failed to resize grid in ebc"
# define EBC_MASK_MALLOC_ERR "Failed to resize mask for map validator"
# define MAP_READ_ERR "Read failure while parsing map layout"

// Invalid Map
# define NO_MAP "No map found in the file"
# define INVALID_MAP_CHAR "Invalid map character"
# define MAP_PLYR "Map must only have 1 player"
# define MAP_NO_PLYR "Map does not have a player"

typedef struct s_p_map
{
	int		fd;
	char	*map_line;
	char	*bit_valid;
	size_t	max_x;
	t_xy	curr;
	t_xy	cap;
}	t_p_map;

// Parse Headers
int		parse_headers(const int fd, t_config *config, char **map_line);

int		parse_map(t_config *config, int fd, char *map_line);
int		parse_path(const char *value, char **dest);

// Parse map bitmask getters & setters
int		get_mask_state(const char *bit_valid, size_t x);
void	set_mask_state(char *bit_valid, size_t x, int state);
t_xy	get_curr_coord(const char *curr_coord);
void	set_curr_coord(char *curr_coord, size_t x, size_t y);

// Parse map utils
int		init_map(t_config *config, t_p_map *p_map, int fd, char *map_line);
int		read_char(t_map *map, t_p_map *p_map);
int		next_row(t_map *map, t_p_map *p_map, size_t *grid_cap);
int		is_map_line(const char *line);

// General parse utils
int		check_commas(const char *line);
int		hole_loc_err(int x, int y);

#endif