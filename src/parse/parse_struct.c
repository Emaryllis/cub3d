/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 06:36:07 by egoh              #+#    #+#             */
/*   Updated: 2026/06/16 06:36:07 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Initializes the t_map struct and the t_p_map struct.
 * @return 0 on success, -1 on failure.
 */
int	init_map(t_config *config, t_p_map *p_map, int fd, char *map_line)
{
	p_map->fd = fd;
	p_map->map_line = map_line;
	p_map->max_x = 0;
	p_map->curr = (t_xy){0, 0};
	p_map->cap = (t_xy){INIT_CAP, INIT_CAP};
	p_map->bit_valid = ft_calloc((p_map->cap.x >> TILE_BITS) + 1, sizeof(char));
	if (!p_map->bit_valid)
		return (parse_error(MASK_MALLOC_ERR));
	p_map->grid = ft_calloc(p_map->cap.y, sizeof(t_p_tile *));
	if (!p_map->grid)
		return (parse_error(GRID_MALLOC_ERR));
	p_map->grid[0] = malloc(sizeof(t_p_tile) * p_map->cap.x);
	if (!p_map->grid[0])
		return (parse_error(GRID_MALLOC_ERR));
	p_map->grid_len = ft_calloc(INIT_CAP, sizeof(size_t));
	if (!p_map->grid_len)
		return (parse_error(GRID_MALLOC_ERR));
	p_map->size = (t_xy){0, 1};
	config->player = (t_plyr){NAN, NAN, NAN, NAN, NAN, NAN, 0, false};
	return (0);
}

int	flatten_map(t_map *map, t_p_map *p_map)
{
	size_t	y;
	size_t	x;

	map->width = p_map->max_x + 1;
	map->height = p_map->size.y;
	map->grid = malloc(sizeof(uint8_t) * map->width * map->height);
	if (!map->grid)
		return (cleanup_p_map(p_map), -1);
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < p_map->grid_len[y])
		{
			map->grid[y * map->width + x] = (uint8_t)(p_map->grid[y][x] - 1);
			x++;
		}
		while (x < map->width)
			map->grid[y * map->width + x++] = TILE_SPACE;
		y++;
	}
	cleanup_p_map(p_map);
	return (0);
}

void	cleanup_p_map(t_p_map *p_map)
{
	size_t	i;

	if (p_map && p_map->grid)
	{
		i = 0;
		while (i < p_map->cap.y)
		{
			if (p_map->grid[i])
				free(p_map->grid[i]);
			i++;
		}
		free(p_map->grid);
		p_map->grid = NULL;
	}
	if (p_map && p_map->grid_len)
	{
		free(p_map->grid_len);
		p_map->grid_len = NULL;
	}
	if (p_map && p_map->bit_valid)
	{
		free(p_map->bit_valid);
		p_map->bit_valid = NULL;
	}
}
