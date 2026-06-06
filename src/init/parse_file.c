/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 15:31:23 by egoh              #+#    #+#             */
/*   Updated: 2026/05/15 15:31:23 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Parsing the header first since under normal conditions, it is
 * expected for the header to come before the map content.
 * @param game
 * @param p
 * @return
 */
int	parse_file(t_game *game, const char *p)
{
	int		fd;
	char	*map_line;

	if (DEBUG)
		printf("File Name: %s\n", p);
	if (ft_strlen(p) < 5 || ft_strcmp(p + ft_strlen(p) - 4, ".cub") != 0)
		return (parse_error("File must have .cub extension"));
	game->config = (t_config){0};
	fd = open(p, O_RDONLY);
	if (fd == -1)
		return (parse_error("Could not open file"));
	if (parse_headers(fd, &game->config, &map_line) == -1)
		return (-1);
	if (parse_map(&game->config, fd, map_line) == -1)
		return (free(map_line), -1);
	free(map_line);
	return (0);
}
