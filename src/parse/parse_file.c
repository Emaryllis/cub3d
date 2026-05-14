/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 13:39:37 by egoh              #+#    #+#             */
/*   Updated: 2026/05/14 11:40:27 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	is_map_line(const char *line)
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

static int	read_lines(int fd, char ***out_lines, int *out_i, size_t capacity)
{
	char	**lines;
	char	*line;
	int		count;

	count = 0;
	lines = malloc(sizeof(char *) * capacity);
	if (!lines)
		return (parse_error("Memory allocation failed"));
	line = get_next_line(fd);
	while (line)
	{
		if (count >= capacity)
		{
			lines = dynamic_realloc((void **)lines, sizeof(char *), &capacity);
			if (!lines)
				return (parse_error(MALLOC_ERR));
		}
		lines[count++] = line;
		line = get_next_line(fd);
	}
	*out_lines = lines;
	*out_i = count;
	return (0);
}

static int	split_header_map(char **lines, int count, int *map_start)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (lines && is_map_line(lines[i]))
		{
			*map_start = i;
			return (0);
		}
		i++;
	}
	return (parse_error("No map found in file"));
}

static int	parse_header(t_config *cfg, char **lines, int map_start)
{
	char	*trimmed;
	int		i;

	i = 0;
	while (i < map_start)
	{
		if (!is_empty(lines[i]))
		{
			trimmed = ft_strtrim(lines[i], " \t\n");
			if (!trimmed)
				return (parse_error("Memory allocation failed"));
			if (parse_element(cfg, trimmed) == -1)
			{
				free(trimmed);
				return (-1);
			}
			free(trimmed);
		}
		i++;
	}
	if (DEBUG)
		printf("Floor: %d, %d, %d. Ceiling: %d, %d, %d.\n",
			cfg->floor_color[0], cfg->floor_color[1], cfg->floor_color[2],
			cfg->ceil_color[0], cfg->ceil_color[1], cfg->ceil_color[2]);
	return (validate_elements(cfg));
}

/**
 * Parsing the header first since under normal conditions, it is
 * expected for the header to come before the map content.
 * @param game
 * @param p
 * @return
 */
int	parse_file(t_game *game, const char *p)
{
	char	**lines;
	int		count;
	int		m_start;
	int		fd;

	if (DEBUG)
		printf("File Name: %s\n", p);
	if (ft_strlen(p) < 5 || ft_strcmp(p + ft_strlen(p) - 4, ".cub") != 0)
		return (parse_error("File must have .cub extension"));
	fd = open(p, O_RDONLY);
	if (fd == -1)
		return (parse_error("Could not open file"));
	if (read_lines(fd, &lines, &count, 64) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	game->config = (t_config){0, 0, 0, 0, {0}, {0}, 0, -1, -1};
	fd = 0;
	if (split_header_map(lines, count, &m_start) == -1
		|| parse_header(&game->config, lines, m_start) == -1
		|| parse_map(&game->config, lines + m_start, count - m_start, 0) == -1)
		fd = -1;
	return (free_arr_range(lines, 0, count), fd);
}
