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
static void	*resize_buffer(void *ptr, size_t *cap_track, size_t unit_size)
{
	size_t	old_size;
	size_t	new_size;
	void	*new_ptr;

	old_size = *cap_track * unit_size;
	new_size = *cap_track * 2 * unit_size;
	new_ptr = ft_realloc(ptr, old_size, new_size);
	if (!new_ptr)
		return (NULL);
	*cap_track *= 2;
	return (new_ptr);
}

/**
 * Allocates a larger bitmask for horizontal tracking, copying existing
 * packed bytes into the expanded tracking footprint via [ft_realloc].
 * @returns 1 on success, -1 on failure
 */
static int	realloc_bitmask_cap(t_p_map *p_map)
{
	char		*new_ptr;
	t_p_tile	*new_row;
	size_t		old_bytes;
	size_t		new_bytes;

	old_bytes = (p_map->cap.x >> TILE_BITS) + 1;
	new_bytes = ((p_map->cap.x * 2) >> TILE_BITS) + 1;
	new_ptr = ft_realloc(p_map->bit_valid, old_bytes, new_bytes);
	if (!new_ptr)
		return (parse_error(EBC_MASK_MALLOC_ERR));
	ft_bzero(new_ptr + old_bytes, new_bytes - old_bytes);
	p_map->bit_valid = new_ptr;
	new_row = resize_buffer(p_map->grid[p_map->curr.y],
			&p_map->cap.x, sizeof(t_p_tile));
	if (!new_row)
		return (parse_error(EBC_RESIZE_ERR));
	p_map->grid[p_map->curr.y] = new_row;
	return (0);
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

/** Transmutes bitmask states to prepare for the next row. */
void	migrate_bitmask_states(t_p_map *p_map)
{
	size_t	i;
	int		state;

	i = 0;
	while (i <= p_map->max_x)
	{
		state = get_mask_state(p_map->bit_valid, i);
		if (state == 3)
			set_mask_state(p_map->bit_valid, i, 2);
		else
			set_mask_state(p_map->bit_valid, i, 0);
		i++;
	}
	set_mask_state(p_map->bit_valid, p_map->max_x + 1, 0);
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
	t_p_tile		**new_grid;
	size_t			*new_grid_len;
	const size_t	old_cap = *grid_cap;

	if (p_map->curr.x == 0)
		return (parse_error(EMPTY_LINE_MAP));
	p_map->grid[p_map->curr.y++][p_map->curr.x] = TILE_P_NULL;
	p_map->size.y++;
	if (p_map->curr.y >= *grid_cap)
	{
		new_grid = resize_buffer(p_map->grid, grid_cap, sizeof(t_p_tile *));
		if (!new_grid)
			return (parse_error(NR_RESIZE_ERR));
		p_map->grid = new_grid;
		new_grid_len = ft_realloc(p_map->grid_len, old_cap * sizeof(t_p_tile *),
				*grid_cap * sizeof(size_t));
		if (!new_grid_len)
			return (parse_error(NR_RESIZE_ERR));
		p_map->grid_len = new_grid_len;
	}
	p_map->curr.x = 0;
	p_map->grid[p_map->curr.y] = malloc(sizeof(t_p_tile) * p_map->cap.x);
	if (!p_map->grid[p_map->curr.y])
		return (parse_error(NEW_ROW_MALLOC_ERR));
	return (migrate_bitmask_states(p_map), read_char(p_map));
}
