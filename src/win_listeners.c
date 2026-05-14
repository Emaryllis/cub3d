/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   win_listeners.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:21:06 by egoh              #+#    #+#             */
/*   Updated: 2026/05/12 11:56:35 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	close_lsr(const t_game *game)
{
	mlx_loop_end(game->mlx);
	return (EXIT_SUCCESS);
}

static int	handle_keypress(const int keysym, const t_game *game)
{
	if (keysym == KEY_ESC)
		return (close_lsr(game));
	return (EXIT_SUCCESS);
}

void	init_listener(t_game *game)
{
	mlx_hook(game->win, KeyPress, KeyPressMask, handle_keypress, game);
	mlx_hook(game->win, DestroyNotify, StructureNotifyMask, close_lsr, game);
}
