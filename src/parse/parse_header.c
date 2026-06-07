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

#include "parse.h"

static int	parse_color_val(const char *str)
{
	int	val;

	if (!str || !*str)
		return (-1);
	val = 0;
	while (ft_isdigit(*str))
	{
		val = val * 10 + (*str - '0');
		if (val > 255)
			return (-1);
		str++;
	}
	while (*str == '\n')
		str++;
	if (*str != '\0')
		return (-1);
	return (val);
}

/**
 * Parses a line with color into an int array of size 3.
 * Requires 3 numbers separated by only 1 comma with a range from 0 to 255.
 * No character should be present before or after the sequence.
 * @param line should be in format "R,G,B" where R, G, B are integers in [0, 255]
 */
static int	parse_color(const char *line, int out[3])
{
	char	**parts;
	int		i;

	if (check_commas(line) != 2)
		return (parse_error(COLOR_LEN));
	parts = ft_split(line, ',');
	if (!parts)
		return (parse_error(H_SPLIT_ERR));
	i = 0;
	while (parts[i] && i < 3)
	{
		out[i] = parse_color_val(parts[i]);
		if (out[i] == -1)
			return (free_arr(parts, i), parse_error(COLOR_RANGE));
		free(parts[i++]);
	}
	if (i != 3 || parts[3])
		return (free_arr(parts, i), parse_error(COLOR_LEN));
	free(parts);
	return (0);
}

/** Simple switch statement to do basic parsing when given a line. */
static int	parse_header(t_game *game, const char *line)
{
	char	**parts;
	int		ret;

	parts = ft_split(line, ' ');
	if (!parts)
		return (parse_error(H_SPLIT_ERR));
	if (!parts[0] || !parts[1])
		return (free_arr(parts, 0), parse_error(INVALID_ELEM_F));
	if (ft_strcmp(parts[0], "NO") == 0)
		ret = parse_path(parts[1], &game->config.no_path, game->envp);
	else if (ft_strcmp(parts[0], "SO") == 0)
		ret = parse_path(parts[1], &game->config.so_path, game->envp);
	else if (ft_strcmp(parts[0], "WE") == 0)
		ret = parse_path(parts[1], &game->config.we_path, game->envp);
	else if (ft_strcmp(parts[0], "EA") == 0)
		ret = parse_path(parts[1], &game->config.ea_path, game->envp);
	else if (ft_strcmp(parts[0], "F") == 0)
		ret = parse_color(parts[1], game->config.floor_color);
	else if (ft_strcmp(parts[0], "C") == 0)
		ret = parse_color(parts[1], game->config.ceil_color);
	else
		ret = parse_error(INVALID_ELEM_I);
	free_arr(parts, 0);
	return (ret);
}

/** A simpler switch statement */
static int	validate_headers(const t_config *config)
{
	const int	*f = config->floor_color;
	const int	*c = config->ceil_color;

	if (!config->no_path)
		return (parse_error(MISSING_NO));
	if (!config->so_path)
		return (parse_error(MISSING_SO));
	if (!config->we_path)
		return (parse_error(MISSING_WE));
	if (!config->ea_path)
		return (parse_error(MISSING_EA));
	if (DEBUG)
	{
		printf("Floor color: %d, %d, %d. ", f[0], f[1], f[2]);
		printf("Ceiling color: %d, %d, %d.\n", c[0], c[1], c[2]);
		printf("NO: %s. SO: %s. WE: %s. EA %s.\n", config->no_path,
			config->so_path, config->we_path, config->ea_path);
	}
	return (0);
}

/**
 * Iterates through each file line and parses the headers using
 * [parse_header_line] if the current line isn't empty. If the
 * current line is empty it just skips it and moves onto the
 * next line. It stops when it encounters a map line denoted
 * by [is_map_line] and sets [map_line]. It only returns -1 when
 * there is an error in [get_next_line] or [parse_element],
 * or there is no map in the file.
 * @return 0 on success, -1 on failure.
 */
int	parse_headers(const int fd, t_game *game, char **map_line)
{
	char	*line;

	*map_line = NULL;
	line = get_next_line(fd, false);
	while (line && !is_map_line(line))
	{
		if (!is_empty(line) && parse_header(game, line) == -1)
			return (close(fd), free(line), -1);
		free(line);
		line = get_next_line(fd, false);
	}
	if (!line)
	{
		close(fd);
		return (parse_error(H_GNL_ERR));
	}
	if (validate_headers(&game->config) == -1)
		return (close(fd), free(line), -1);
	*map_line = ft_strjoin(line, get_next_line(fd, true));
	free(line);
	if (!*map_line)
		return (close(fd), parse_error(M_GNL_ERR));
	return (0);
}
