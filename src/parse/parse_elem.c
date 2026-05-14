/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 13:41:42 by egoh              #+#    #+#             */
/*   Updated: 2026/05/12 13:46:21 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	parse_color_val(const char *str)
{
	int	val;

	if (!str || !*str)
		return (-1);
	val = 0;
	while (*str >= '0' && *str <= '9')
	{
		val = val * 10 + (*str - '0');
		if (val > 255)
			return (-1);
		str++;
	}
	if (*str != '\0')
		return (-1);
	return (val);
}

static int	check_commas(const char *line)
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
		return (parse_error(MALLOC_ERR));
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
int	parse_element(t_config *config, const char *line)
{
	char	**parts;
	int		ret;

	parts = ft_split(line, ' ');
	if (!parts)
		return (parse_error(MALLOC_ERR));
	if (!parts[0] || !parts[1])
		return (free_arr(parts, 0), parse_error("Invalid element format"));
	if (ft_strcmp(parts[0], "NO") == 0)
		ret = parse_path(&config->no_path, parts[1]);
	else if (ft_strcmp(parts[0], "SO") == 0)
		ret = parse_path(&config->so_path, parts[1]);
	else if (ft_strcmp(parts[0], "WE") == 0)
		ret = parse_path(&config->we_path, parts[1]);
	else if (ft_strcmp(parts[0], "EA") == 0)
		ret = parse_path(&config->ea_path, parts[1]);
	else if (ft_strcmp(parts[0], "F") == 0)
		ret = parse_color(parts[1], config->floor_color);
	else if (ft_strcmp(parts[0], "C") == 0)
		ret = parse_color(parts[1], config->ceil_color);
	else
		ret = parse_error("Unknown element identifier");
	free_arr(parts, 0);
	return (ret);
}

int	validate_elements(const t_config *config)
{
	if (!config->no_path)
		return (parse_error(MISSING_NO));
	if (!config->so_path)
		return (parse_error(MISSING_SO));
	if (!config->we_path)
		return (parse_error(MISSING_WE));
	if (!config->ea_path)
		return (parse_error(MISSING_EA));
	if (DEBUG)
		printf("NO: %s. SO: %s. WE: %s. EA %s.\n", config->no_path,
			config->so_path, config->we_path, config->ea_path);
	return (0);
}
