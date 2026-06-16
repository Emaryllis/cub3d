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

static void	destroy_image(void *mlx, t_img *img)
{
	if (img && img->mlx_img)
	{
		mlx_destroy_image(mlx, img->mlx_img);
		img->mlx_img = NULL;
	}
}

void	cleanup(t_game *game)
{
	if (!game)
		return ;
	destroy_image(game->mlx, &game->config.tex_no);
	destroy_image(game->mlx, &game->config.tex_so);
	destroy_image(game->mlx, &game->config.tex_we);
	destroy_image(game->mlx, &game->config.tex_ea);
	//destroy_image(game->mlx, &game->screen);
	if (game->config.map.grid)
	{
		free(game->config.map.grid);
		game->config.map.grid = NULL;
	}
	if (game->mlx)
	{
		if (game->win)
		{
			mlx_destroy_window(game->mlx, game->win);
			game->win = NULL;
		}
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}
