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

#include "main.h"

static int	char_to_tile(char c, t_tile *out, bool *has_plyr)
{
	if (c == ' ')
		*out = TILE_SPACE;
	else if (c == '0')
		*out = TILE_EMPTY;
	else if (c == '1')
		*out = TILE_WALL;
	else if (c == 'N')
		*out = TILE_PLAYER_N;
	else if (c == 'S')
		*out = TILE_PLAYER_S;
	else if (c == 'E')
		*out = TILE_PLAYER_E;
	else if (c == 'W')
		*out = TILE_PLAYER_W;
	else
		return (parse_error(INVALID_MAP));
	if (*out == TILE_PLAYER_N || *out == TILE_PLAYER_S
		|| *out == TILE_PLAYER_E || *out == TILE_PLAYER_W)
	{
		if (*has_plyr)
			return (parse_error(MAP_PLYR));
		*has_plyr = true;
	}
	return (0);
}

static int	parse_map_line(char *line, t_tile *row, int width, bool *has_plyr)
{
	int		i;

	if (ft_strcmp(line, "\n") == 0)
		return (parse_error(INVALID_MAP));
	i = 0;
	while (line[i] && line[i] != '\n' && i < width)
	{
		if (char_to_tile(line[i], &row[i], has_plyr) == -1)
			return (-1);
		if (DEBUG)
			printf("%c", line[i]);
		i++;
	}
	while (i < width)
		row[i++] = TILE_SPACE;
	return (0);
}

static int	get_max_width(char **lines, int count)
{
	int	max;
	int	len;
	int	i;

	max = 0;
	i = 0;
	while (i < count)
	{
		len = 0;
		while (lines[i][len] && lines[i][len] != '\n')
			len++;
		if (len > max)
			max = len;
		i++;
	}
	return (max);
}

int	parse_map(t_config *cfg, char **lines, int count, int i)
{
	bool	has_plyr;

	cfg->map = malloc(sizeof(t_tile *) * count);
	if (!cfg->map)
		return (parse_error(MALLOC_ERR));
	cfg->map_h = count;
	cfg->map_w = get_max_width(lines, count);
	i = 0;
	has_plyr = false;
	while (i < count)
	{
		cfg->map[i] = malloc(sizeof(t_tile) * cfg->map_w);
		if (!cfg->map[i])
			return (cfg->map_h = i, parse_error(MALLOC_ERR));
		if (parse_map_line(lines[i], cfg->map[i], cfg->map_w, &has_plyr) == -1)
			return (cfg->map_h = i + 1, -1);
		if (DEBUG)
			printf("\n");
		i++;
	}
	if (!has_plyr)
		return (parse_error(MAP_PLYR));
	return (0);
}
