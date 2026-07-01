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
static t_p_tile	char_to_tile(t_map *map, t_p_map const *p_map, char c)
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
	if (p_map->curr.x > map->width)
		map->width = p_map->curr.x;
	return (tile);
}

/**
 * Evaluates the active tile character against previous bitmask constraints.
 * If a walkable path is found, it spawns Right (State 2) and Down (State 3)
 * lookahead trackers to guarantee enclosure by subsequent wall segments.
 */
static int	eval_gate(t_p_map *p_map, t_p_tile tile, bool first_row)
{
	int	state;

	state = 0;
	if (!first_row)
		state = get_mask_state(p_map->bit_valid, p_map->curr.x);
	if (state > 0 && (tile == TILE_P_SPACE || tile == TILE_P_NULL))
		return (hole_loc_err(p_map->curr.x, p_map->curr.y));
	if (tile == TILE_P_WALL || tile == TILE_P_SPACE)
		return (0);
	if ((tile == TILE_P_EMPTY || tile == TILE_P_PLAYER) && p_map->curr.x > 0
		&& p_map->grid[p_map->curr.y][p_map->curr.x - 1] == TILE_P_SPACE)
		return (hole_loc_err(p_map->curr.x - 1, p_map->curr.y));
	if ((tile == TILE_P_EMPTY || tile == TILE_P_PLAYER) && p_map->curr.y > 0
		&& p_map->grid[p_map->curr.y - 1]
		&& p_map->grid[p_map->curr.y - 1][p_map->curr.x] == TILE_P_SPACE)
		return (hole_loc_err(p_map->curr.x, p_map->curr.y - 1));
	if (p_map->curr.x > p_map->max_x)
		p_map->max_x = p_map->curr.x;
	set_mask_state(p_map->bit_valid, p_map->curr.x + 1, 2);
	set_mask_state(p_map->bit_valid, p_map->curr.x, 3);
	return (0);
}

/**
 * Main caller of parsing & validating helper functions.
 * Also sets the player data used for rendering.
 * @return The next character, -1 on failure
 */
static int	process_char(t_config *config, t_p_map *p_map, char c,
	const double vecs[4][2])
{
	t_p_tile	tile;
	int			i;

	tile = char_to_tile(&config->map, p_map, c);
	if (tile == TILE_P_NULL)
		return (-1);
	if (eval_gate(p_map, tile, (p_map->curr.y == 0)) == -1)
		return (-1);
	if (tile == TILE_P_PLAYER)
	{
		if (!isnan(config->player.dir_x))
			return (parse_error(MAP_PLYR));
		i = 0;
		while (FACES[i] && FACES[i] != c)
			i++;
		config->player = (t_plyr){p_map->curr.x + 0.5, p_map->curr.y + 0.5,
			vecs[i][0], vecs[i][1], -vecs[i][1] * FOV, vecs[i][0] * FOV,
			config->player.last_mouse_x, config->player.mouse_captured};
		p_map->grid[p_map->curr.y][p_map->curr.x] = TILE_P_EMPTY;
	}
	p_map->curr.x++;
	p_map->grid_len[p_map->curr.y] = p_map->curr.x;
	return (read_char(p_map));
}

int	audit_residual_leaks(const t_config *config, t_p_map *p_map)
{
	size_t	i;
	int		state;

	i = 0;
	if (p_map->curr.x > 0)
		p_map->grid[p_map->curr.y][p_map->curr.x] = TILE_P_NULL;
	if (isnan(config->player.dir_x))
		return (parse_error(MAP_NO_PLYR));
	migrate_bitmask_states(p_map);
	while (i <= p_map->max_x)
	{
		state = get_mask_state(p_map->bit_valid, i);
		if (state == 2)
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
int	parse_map(t_config *config, int fd, char *map_line)
{
	const double	vecs[4][2] = {{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
	t_p_map			p_map;
	size_t			grid_cap;
	int				c;

	if (init_map(config, &p_map, fd, map_line) == -1)
		return (close(fd), -1);
	grid_cap = p_map.cap.y;
	c = read_char(&p_map);
	while (c > 0)
	{
		if (PARSE_DEBUG)
			printf("%c", c);
		if (c == '\n')
			c = next_row(&p_map, &grid_cap);
		else
			c = process_char(config, &p_map, (char)c, vecs);
	}
	if (PARSE_DEBUG)
		printf("\n");
	close(fd);
	if (c == -1 || audit_residual_leaks(config, &p_map) == -1)
		return (cleanup_p_map(&p_map), -1);
	return (flatten_map(&config->map, &p_map));
}
