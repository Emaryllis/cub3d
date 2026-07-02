/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 13:42:26 by egoh              #+#    #+#             */
/*   Updated: 2026/07/01 16:26:08 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Translates incoming ASCII characters into designated game engine tiles.
 * Tracks the overall horizontal map span dimension dynamically.
 */
static t_p_tile	char_to_tile(t_p_map *p_map, char c)
{
	t_p_tile	tile;

	if (c == ' ')
		tile = TILE_P_SPACE;
	else if (c == '0')
		tile = TILE_P_EMPTY;
	else if (c == '1')
		tile = TILE_P_WALL;
	else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		tile = TILE_P_PLAYER;
	else
		return (parse_error(INVALID_MAP_CHAR), TILE_P_NULL);
	p_map->grid[p_map->curr.y][p_map->curr.x] = tile;
	if (p_map->curr.x > p_map->max.x)
		p_map->max.x = p_map->curr.x;
	return (tile);
}

/**
 * Evaluates a tile against bitmask constraints to enforce map enclosure.
 *
 * - If it's the first row and the tile is [TILE_P_EMPTY], return a hole error
 *   because there is no wall above to close the top.
 * - If it's not the first row and the bitmask requires a wall at this
 *   column, but the tile is a space or null terminator, return a hole error
 *   because the downward enclosure from the row above was not satisfied.
 * - If the tile is a wall or space, exit early and return success since only
 *   [TILE_P_EMPTY] tiles require enclosure checks or lookahead spawning.
 * - If the tile is [TILE_P_EMPTY] and its immediate left neighbor is a space,
 *   return a hole error to prevent out of bounds ray casting.
 * - If the tile is [TILE_P_EMPTY] and its immediate above neighbor is a space
 *   (either explicitly parsed or implicitly absent due to a shorter previous
 *   row), return a hole error to prevent out of bounds ray casting.
 * - If the current column exceeds the tracked maximum width, update the global
 *   rightmost bounding box edge so implicit-space validation covers the full map.
 * - If the lookahead column exceeds bitmask capacity, expand the bitmask
 *   allocation to safely write the next state without heap overflow.
 * - Add a wall requirement for the below and right neighbor to ensure full
 *   enclosure of the current tile.
 */
static int	eval_gate(t_p_map *p_map, t_p_tile tile, bool first_row)
{
	if (first_row && tile == TILE_P_EMPTY)
		return (hole_loc_err(p_map->curr.x, p_map->curr.y));
	if (!first_row && get_mask_state(p_map->bit_valid, p_map->curr.x) == 2
		&& (tile == TILE_P_SPACE || tile == TILE_P_NULL))
		return (hole_loc_err(p_map->curr.x, p_map->curr.y));
	if (tile == TILE_P_WALL || tile == TILE_P_SPACE)
		return (0);
	if (tile == TILE_P_EMPTY && p_map->curr.x > 0
		&& p_map->grid[p_map->curr.y][p_map->curr.x - 1] == TILE_P_SPACE)
		return (hole_loc_err(p_map->curr.x - 1, p_map->curr.y));
	if (tile == TILE_P_EMPTY && p_map->curr.y > 0
	&& (p_map->grid_len[p_map->curr.y - 1] <= p_map->curr.x
		|| p_map->grid[p_map->curr.y - 1][p_map->curr.x] == TILE_P_SPACE))
		return (hole_loc_err(p_map->curr.x, p_map->curr.y - 1));
	if (p_map->curr.x > p_map->max.x)
		p_map->max.x = p_map->curr.x;
	if (p_map->curr.x + 1 >= p_map->cap.x && realloc_bitmask_cap(p_map) == -1)
		return (-1);
	set_mask_state(p_map->bit_valid, p_map->curr.x + 1, 2);
	set_mask_state(p_map->bit_valid, p_map->curr.x, 3);
	return (0);
}

/**
 * Main caller of parsing & validating helper functions.
 * Also sets the player data used for rendering.
 * @return The next character, -1 on failure
 */
static int	process_char(t_plyr *plyr, t_p_map *p_map, char c,
	const double vecs[4][2])
{
	t_p_tile	tile;
	int			i;

	tile = char_to_tile(p_map, c);
	if (tile == TILE_P_NULL)
		return (-1);
	if (tile == TILE_P_PLAYER)
	{
		if (!isnan(plyr->dir_x))
			return (parse_error(MAP_PLYR));
		i = 0;
		while (FACES[i] && FACES[i] != c)
			i++;
		*plyr = (t_plyr){p_map->curr.x + 0.5, p_map->curr.y + 0.5,
			vecs[i][0], vecs[i][1], -vecs[i][1] * FOV, vecs[i][0] * FOV,
			plyr->last_mouse_x, plyr->mouse_captured};
		p_map->grid[p_map->curr.y][p_map->curr.x] = TILE_P_EMPTY;
		tile = TILE_P_EMPTY;
	}
	if (eval_gate(p_map, tile, p_map->curr.y == 0) == -1)
		return (-1);
	p_map->curr.x++;
	p_map->grid_len[p_map->curr.y] = p_map->curr.x;
	return (read_char(p_map));
}

int	audit_residual_leaks(const t_plyr *plyr, const t_p_map *p_map)
{
	size_t	i;

	i = 0;
	if (p_map->curr.x > 0)
		p_map->grid[p_map->curr.y][p_map->curr.x] = TILE_P_NULL;
	if (isnan(plyr->dir_x))
		return (parse_error(MAP_NO_PLYR));
	while (i <= p_map->max.x)
	{
		if (get_mask_state(p_map->bit_valid, i) == 3)
			return (hole_loc_err(i, p_map->curr.y));
		i++;
	}
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
int	parse_map(t_map *map, t_plyr *plyr, int fd, char *map_line)
{
	const double	vecs[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
	t_p_map			p_map;
	size_t			grid_cap;
	int				c;

	if (init_map(plyr, &p_map, fd, map_line) == -1)
		return (close(fd), -1);
	grid_cap = p_map.cap.y;
	c = read_char(&p_map);
	while (c > 0)
	{
		if (PRINT_MAP_DEBUG)
			printf("%c", c);
		if (c == '\n')
			c = next_row(&p_map, &grid_cap);
		else
			c = process_char(plyr, &p_map, (char)c, vecs);
	}
	if (PRINT_MAP_DEBUG)
		printf("\n");
	close(fd);
	if (c == -1 || audit_residual_leaks(plyr, &p_map) == -1)
		return (cleanup_p_map(&p_map), -1);
	return (finalize_parse(map, &p_map));
}
