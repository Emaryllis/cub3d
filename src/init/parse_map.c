/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 13:42:26 by egoh              #+#    #+#             */
/*   Updated: 2026/05/12 13:42:42 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Translates incoming ASCII characters into designated game engine tiles.
 * Tracks the overall horizontal map span dimension dynamically.
 */
static t_tile	char_to_tile(t_map *map, t_p_map *p_map, char c)
{
	t_tile	tile;

	if (c == ' ')
		tile = TILE_SPACE;
	else if (c == '0')
		tile = TILE_EMPTY;
	else if (c == '1')
		tile = TILE_WALL;
	else if (c == 'N')
		tile = TILE_PLAYER_N;
	else if (c == 'S')
		tile = TILE_PLAYER_S;
	else if (c == 'E')
		tile = TILE_PLAYER_E;
	else if (c == 'W')
		tile = TILE_PLAYER_W;
	else
		return (parse_error(INVALID_MAP_CHAR), TILE_NULL);
	map->grid[p_map->curr.y][p_map->curr.x] = tile;
	if (p_map->curr.x > map->size.x)
		map->size.x = p_map->curr.x;
	return (tile);
}

/**
 * Evaluates the active tile character against previous bitmask constraints.
 * If a walkable path is found, it spawns Right (State 2) and Down (State 3)
 * lookahead trackers to guarantee enclosure by subsequent wall segments.
 */
static int	eval_gate(t_map *map, t_p_map *p_map, t_tile tile, bool first_row)
{
	int	state;

	state = 0;
	if (!first_row)
		state = get_mask_state(p_map->bit_valid, p_map->curr.x);
	if (state > 0 && (tile == TILE_SPACE || tile == TILE_NULL))
		return (hole_loc_err(p_map->curr.x, p_map->curr.y));
	if (tile == TILE_WALL || tile == TILE_SPACE)
		return (0);
	if (tile == TILE_EMPTY && p_map->curr.x > 0
		&& map->grid[p_map->curr.y][p_map->curr.x - 1] == TILE_SPACE)
		return (hole_loc_err(p_map->curr.x - 1, p_map->curr.y));
	if (tile == TILE_EMPTY && p_map->curr.y > 0
		&& map->grid[p_map->curr.y - 1]
		&& map->grid[p_map->curr.y - 1][p_map->curr.x] == TILE_SPACE)
		return (hole_loc_err(p_map->curr.x, p_map->curr.y - 1));
	if (p_map->curr.x > p_map->max_x)
		p_map->max_x = p_map->curr.x;
	set_mask_state(p_map->bit_valid, p_map->curr.x + 1, 2);
	set_mask_state(p_map->bit_valid, p_map->curr.x, 3);
	return (0);
}

/**
 *
 * @param map
 * @param p_map
 * @param c
 * @return The next character, -1 on failure
 */
static int	process_char(t_map *map, t_p_map *p_map, char c)
{
	t_tile	tile;

	tile = char_to_tile(map, p_map, c);
	if (tile == TILE_NULL)
		return (-1);
	if (eval_gate(map, p_map, tile, (p_map->curr.y == 0)) == -1)
		return (-1);
	if (tile >= TILE_PLAYER_N && tile <= TILE_PLAYER_W)
	{
		if (map->plyr_face != TILE_NULL)
			return (parse_error(MAP_PLYR));
		map->plyr = p_map->curr;
		map->plyr_face = tile;
	}
	p_map->curr.x++;
	map->grid_len[p_map->curr.y] = p_map->curr.x;
	return (read_char(map, p_map));
}

/**
 *
 */
int	audit_residual_leaks(t_map *map, t_p_map *p_map)
{
	size_t	i;
	int		state;

	i = 0;
	if (p_map->curr.x > 0)
		map->grid[p_map->curr.y][p_map->curr.x] = TILE_NULL;
	if (map->plyr_face == TILE_NULL)
		return (free(p_map->bit_valid), parse_error(MAP_NO_PLYR));
	while (i <= p_map->max_x)
	{
		state = get_mask_state(p_map->bit_valid, i);
		if (state == 2 || state == 3)
		{
			free(p_map->bit_valid);
			if (state == 3)
				return (hole_loc_err(i, p_map->curr.y));
			return (hole_loc_err(i, -1));
		}
		i++;
	}
	free(p_map->bit_valid);
	return (0);
}

/**
 * @param config Main config struct
 * @param fd cub file stream
 * @param map_line The leftover read data from parsing header
 *
 * Uses the file stream & map_line to stream character by
 * character. If it is a newline character, next_row is
 * called to prepare for the next row. The loop ends when
 * there is a read error or EOF.
*/
int	parse_map(t_config *config, int fd, char *map_line)
{
	t_p_map	p_map;
	size_t	grid_cap;
	int		c;

	if (init_map(config, &p_map, fd, map_line) == -1)
		return (-1);
	grid_cap = config->map.cap.y;
	c = read_char(&config->map, &p_map);
	while (c > 0)
	{
		if (DEBUG)
			printf("%c", c);
		if (c == '\n')
			c = next_row(&config->map, &p_map, &grid_cap);
		else
			c = process_char(&config->map, &p_map, (char)c);
	}
	if (c == -1)
	{
		free(p_map.bit_valid);
		return (-1);
	}
	return (audit_residual_leaks(&config->map, &p_map));
}
