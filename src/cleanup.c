/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 13:12:57 by egoh              #+#    #+#             */
/*   Updated: 2026/05/12 13:12:57 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	cleanup(t_game *game)
{
	free(game->config.no_path);
	free(game->config.so_path);
	free(game->config.we_path);
	free(game->config.ea_path);
	if (game->config.map)
		free_arr_range((char **)game->config.map, 0, game->config.map_h);
	if (game->mlx && game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}
