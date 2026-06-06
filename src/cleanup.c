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
	const t_config	config = game->config;

	free(config.no_path);
	free(config.so_path);
	free(config.we_path);
	free(config.ea_path);
	if (config.map.grid)
		free_arr_range((char **)config.map.grid, 0, game->config.map.cap.y);
	if (config.map.grid_len)
		free(config.map.grid_len);
	if (game->mlx && game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
}
