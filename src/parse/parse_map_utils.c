/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 19:29:57 by egoh              #+#    #+#             */
/*   Updated: 2026/06/06 03:57:30 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Doubles an existing memory segment, and automatically updating
 * the tracking capacity parameter variable to match the new allocation.
 * @returns The fresh memory address, NULL on failure.
 */
void	*resize_buffer(void *ptr, size_t *cap_track, size_t unit_size)
{
	void	*new_ptr;

	new_ptr = ft_realloc(ptr, *cap_track * unit_size,
			*cap_track * 2 * unit_size);
	if (!new_ptr)
		return (NULL);
	*cap_track *= 2;
	return (new_ptr);
}

/**
 * Fetches the next layout character, prioritizing map_line stream, then
 * the file descriptor stream, expanding tracking arrays mid-flight if the
 * horizontal tile index exceeds the currently allocated tracking width.
 * @returns A character, 0 on EOF, -1 on error.
 */
int	read_char(t_p_map *p_map)
{
	char	c;
	ssize_t	read_ret;

	if (p_map->map_line && *p_map->map_line)
		c = *p_map->map_line++;
	else
	{
		p_map->map_line = NULL;
		read_ret = read(p_map->fd, &c, 1);
		if (read_ret < 0)
			return (parse_error(MAP_READ_ERR));
		if (read_ret == 0)
			return (0);
	}
	if (c == '\n')
		return (c);
	if (p_map->curr.x >= p_map->cap.x && realloc_bitmask_cap(p_map) == -1)
		return (-1);
	return (c);
}

static int	ensure_grid_capacity(t_p_map *p_map, size_t *grid_cap)
{
	t_p_tile	**new_grid;
	size_t		*new_grid_len;
	size_t		old_cap;

	old_cap = *grid_cap;
	if (p_map->curr.y < *grid_cap)
		return (0);
	new_grid = resize_buffer(p_map->grid, grid_cap, sizeof(t_p_tile *));
	if (!new_grid)
		return (parse_error(NR_RESIZE_ERR));
	p_map->grid = new_grid;
	new_grid_len = ft_realloc(p_map->grid_len, old_cap * sizeof(size_t),
			*grid_cap * sizeof(size_t));
	if (!new_grid_len)
		return (parse_error(NR_RESIZE_ERR));
	p_map->grid_len = new_grid_len;
	return (0);
}

/**
 * Concludes the processing of the row segment with a null terminator.
 * Shifts the current x back to 0 and increments y by 1. Calls
 * [migrate_bitwise_states] to update bitmask. It resizes the grid
 * if it has reached capacity via [resize_buffer] and [ft_realloc].
 * It also allocates memory for the new row.
 * @return Return of [read_char]
 */
int	next_row(t_p_map *p_map, size_t *grid_cap)
{
	size_t	x;

	if (p_map->curr.x == 0)
		return (parse_error(EMPTY_LINE_MAP));
	x = p_map->curr.x;
	while (x <= p_map->max.x)
	{
		if (get_mask_state(p_map->bit_valid, x) > 0)
			return (hole_loc_err(x, p_map->curr.y));
		x++;
	}
	p_map->grid[p_map->curr.y++][p_map->curr.x] = TILE_P_NULL;
	p_map->max.y++;
	if (ensure_grid_capacity(p_map, grid_cap) == -1)
		return (-1);
	p_map->curr.x = 0;
	p_map->grid[p_map->curr.y] = malloc(sizeof(t_p_tile) * p_map->cap.x);
	if (!p_map->grid[p_map->curr.y])
		return (parse_error(NEW_ROW_MALLOC_ERR));
	migrate_bitmask_states(p_map);
	return (read_char(p_map));
}
