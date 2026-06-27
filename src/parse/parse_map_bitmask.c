/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_bitmask.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:40:16 by egoh              #+#    #+#             */
/*   Updated: 2026/06/05 17:52:40 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Extracts the 2-bit state (0, 2, or 3) for a specific column index X.
 * Compresses the prerequisites into a single byte of the bit_valid array.
 * 0: No requirements
 * 2: Required to be a TILE_WALL
 * 3: Next row at the same x position is required to be a TILE_WALL
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
