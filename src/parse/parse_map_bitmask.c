/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_bitmask.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:40:16 by egoh              #+#    #+#             */
/*   Updated: 2026/07/01 16:47:46 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Extracts the 2-bit state (0, 2, or 3) for a specific column index X.
 * Compresses the prerequisites into a single byte of the bit_valid array.
 * 0: No requirements
 * 2: Required to be a [TILE_WALL]
 * 3: Next row at the same x position is required to be a [TILE_WALL]
 * @return The bitmask state
 */
int	get_mask_state(const char *bit_valid, size_t x)
{
	size_t	idx;
	int		mask;
	int		shift;

	idx = x >> TILE_BITS;
	mask = (1 << TILE_BITS) - 1;
	shift = (x & mask) << 1;
	return (bit_valid[idx] >> shift & mask);
}

/**
 * Safely overwrites the 2-bit state of a specific column index X
 * without altering or corrupting neighboring columns.
 */
void	set_mask_state(char *bit_valid, size_t x, int state)
{
	size_t	idx;
	int		mask;
	int		shift;

	idx = x >> TILE_BITS;
	mask = (1 << TILE_BITS) - 1;
	shift = (x & mask) << 1;
	bit_valid[idx] = (bit_valid[idx] & ~(mask << shift)) | state << shift;
}

/**
 * Allocates a larger bitmask for horizontal tracking, copying existing
 * packed bytes into the expanded tracking footprint via [ft_realloc].
 * @returns 1 on success, -1 on failure
 */
int	realloc_bitmask_cap(t_p_map *p_map)
{
	char		*new_ptr;
	t_p_tile	*new_row;
	size_t		old_bytes;
	size_t		new_bytes;

	old_bytes = (p_map->cap.x >> TILE_BITS) + 1;
	new_bytes = ((p_map->cap.x * 2) >> TILE_BITS) + 1;
	new_ptr = ft_realloc(p_map->bit_valid, old_bytes, new_bytes);
	if (!new_ptr)
		return (parse_error(MASK_RESIZE_ERR));
	ft_bzero(new_ptr + old_bytes, new_bytes - old_bytes);
	p_map->bit_valid = new_ptr;
	new_row = resize_buffer(p_map->grid[p_map->curr.y],
			&p_map->cap.x, sizeof(t_p_tile));
	if (!new_row)
		return (parse_error(ROW_RESIZE_ERR));
	p_map->grid[p_map->curr.y] = new_row;
	return (0);
}

/** Transmutes bitmask states to prepare for the next row. */
void	migrate_bitmask_states(t_p_map *p_map)
{
	size_t	i;

	i = 0;
	while (i <= p_map->max.x)
	{
		if (get_mask_state(p_map->bit_valid, i) == 3)
			set_mask_state(p_map->bit_valid, i, 2);
		else
			set_mask_state(p_map->bit_valid, i, 0);
		i++;
	}
}
