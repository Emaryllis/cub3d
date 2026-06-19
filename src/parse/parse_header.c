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
	int			val;
	const char	*tmp = str;

	if (!str || !*str)
		return (-1);
	val = 0;
	while (ft_isdigit(*str))
	{
		val = val * 10 + (*str - '0');
		if (val > COLOR_MAX || str - tmp == COLOR_MAX_LEN)
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
static int	parse_color(const char *line, int *out)
{
	char	**parts;
	int		ret[3];
	int		i;

	if (*out != INIT_COLOR)
		return (parse_error(DUP_COLOR_ID));
	if (check_commas(line) != 2)
		return (parse_error(COLOR_LEN));
	parts = ft_split(line, ',');
	if (!parts)
		return (parse_error(H_SPLIT_ERR));
	i = 0;
	while (parts[i] && i < 3)
	{
		ret[i] = parse_color_val(parts[i]);
		if (ret[i] == -1)
			return (free_arr(parts, i), parse_error(COLOR_RANGE));
		free(parts[i++]);
	}
	if (i != 3 || parts[3])
		return (free_arr(parts, i), parse_error(COLOR_LEN));
	free(parts);
	*out = ret[0] << RED_SHIFT | ret[1] << GREEN_SHIFT | ret[2] << BLUE_SHIFT;
	return (0);
}

/** Simple switch statement to do basic parsing when given a line. */
static int	parse_header(t_game *game, const char *line)
{
	char	**parts;
	int		r;

	parts = ft_split(line, ' ');
	if (!parts)
		return (parse_error(H_SPLIT_ERR));
	if (!parts[0] || !parts[1])
		return (free_arr(parts, 0), parse_error(INVALID_ELEM_F));
	if (ft_strcmp(parts[0], "NO") == 0)
		r = parse_path(parts[1], &game->config.tex_no, game->mlx, game->envp);
	else if (ft_strcmp(parts[0], "SO") == 0)
		r = parse_path(parts[1], &game->config.tex_so, game->mlx, game->envp);
	else if (ft_strcmp(parts[0], "WE") == 0)
		r = parse_path(parts[1], &game->config.tex_we, game->mlx, game->envp);
	else if (ft_strcmp(parts[0], "EA") == 0)
		r = parse_path(parts[1], &game->config.tex_ea, game->mlx, game->envp);
	else if (ft_strcmp(parts[0], "F") == 0)
		r = parse_color(parts[1], &game->config.floor_color);
	else if (ft_strcmp(parts[0], "C") == 0)
		r = parse_color(parts[1], &game->config.ceil_color);
	else
		r = parse_error(INVALID_ELEM_I);
	free_arr(parts, 0);
	return (r);
}

/** A simpler switch statement */
static int	validate_headers(const t_config *config)
{
	if (!config->tex_no.img_ptr)
		return (parse_error(MISSING_NO));
	if (!config->tex_so.img_ptr)
		return (parse_error(MISSING_SO));
	if (!config->tex_we.img_ptr)
		return (parse_error(MISSING_WE));
	if (!config->tex_ea.img_ptr)
		return (parse_error(MISSING_EA));
	if (config->floor_color == INIT_COLOR)
		return (parse_error(MISSING_FLOOR));
	if (config->ceil_color == INIT_COLOR)
		return (parse_error(MISSING_CEIL));
	if (PARSE_DEBUG)
	{
		printf("Floor color: 0x%06X\n", config->floor_color);
		printf("Ceiling color: 0x%06X\n", config->ceil_color);
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
		return (parse_error(GNL_EOL_ERR));
	}
	if (!is_map_line(line))
		return (close(fd), free(line), parse_error(NO_MAP));
	if (validate_headers(&game->config) == -1)
		return (close(fd), free(line), -1);
	*map_line = ft_strjoin(line, get_next_line(fd, true));
	free(line);
	if (!*map_line)
		return (close(fd), parse_error(M_JOIN_ERR));
	return (0);
}
