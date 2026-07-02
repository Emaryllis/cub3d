/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 15:31:23 by egoh              #+#    #+#             */
/*   Updated: 2026/07/01 16:47:46 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/**
 * Parsing the header first since under normal conditions, it is
 * expected for the header to come before the map content.
 */
int	parse_file(t_game *game, const char *p)
{
	int		fd;
	char	*line;

	if (ft_strlen(p) < 5 || ft_strcmp(p + ft_strlen(p) - 4, ".cub") != 0)
		return (parse_error(INVALID_CONFIG_EXT));
	game->config = (t_config){0};
	game->config.floor_color = INIT_COLOR;
	game->config.ceil_color = INIT_COLOR;
	fd = open(p, O_RDONLY);
	if (fd == -1)
		return (parse_error(OPEN_CONFIG_ERR));
	if (parse_headers(fd, game, &line) == -1)
		return (-1);
	if (parse_map(&game->config.map, &game->config.player, fd, line) == -1)
		return (free(line), -1);
	free(line);
	return (0);
}
