/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 15:35:17 by egoh              #+#    #+#             */
/*   Updated: 2026/05/15 15:35:17 by egoh             ###   ########.fr       */
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
	config->map.cap = (t_xy){INIT_CAP, INIT_CAP};
	config->map.grid = ft_calloc(config->map.cap.y, sizeof(t_tile *));
	if (!config->map.grid)
		return (parse_error(GRID_MALLOC_ERR));
	config->map.grid[0] = malloc(sizeof(t_tile) * config->map.cap.x);
	if (!config->map.grid[0])
		return (parse_error(GRID_MALLOC_ERR));
	config->map.grid_len = ft_calloc(INIT_CAP, sizeof(size_t));
	if (!config->map.grid_len)
		return (parse_error(GRID_MALLOC_ERR));
	config->map.size = (t_xy){0, 1};
	config->map.plyr = (t_xy){0, 0};
	config->map.plyr_face = TILE_NULL;
	return (0);
}

/** Checks if the current string (get_next_line) is a valid map line */
int	is_map_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ' && line[i] != '0' && line[i] != '1'
			&& line[i] != 'N' && line[i] != 'S'
			&& line[i] != 'E' && line[i] != 'W')
			return (0);
		i++;
	}
	return (i > 0);
}

/** Used to validate the amount of numbers given per color line */
int	check_commas(const char *line)
{
	int	i;

	if (!line)
		return (-1);
	i = 0;
	while (*line)
	{
		if (*line == ',')
			i++;
		line++;
	}
	return (i);
}

static void	ft_putnbr_fd(size_t n, int fd)
{
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	write(fd, &(char){n % 10 + '0'}, 1);
}

/**
 * Detailed error reporting because looking through 1s just to
 * find 0(s) that shouldn't be there is a disservice to my eyes.
 * @return -1 (only used on failure)
 */
int	hole_loc_err(size_t x, size_t y)
{
	write(STDERR_FILENO, "Error: Found hole in the map at line ", 37);
	ft_putnbr_fd(y + 1, STDERR_FILENO);
	write(STDERR_FILENO, ", column ", 9);
	ft_putnbr_fd(x + 1, STDERR_FILENO);
	write(STDERR_FILENO, "!\n", 2);
	return (-1);
}
