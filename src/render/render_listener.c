/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_listener.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 08:38:35 by egoh              #+#    #+#             */
/*   Updated: 2026/07/01 16:26:08 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static int	close_lsr(t_game *game)
{
	mlx_loop_end(game->mlx);
	return (0);
}

static int	get_key_idx(int keysym)
{
	if (keysym == KEY_W)
		return (KEY_IDX_W);
	if (keysym == KEY_A)
		return (KEY_IDX_A);
	if (keysym == KEY_S)
		return (KEY_IDX_S);
	if (keysym == KEY_D)
		return (KEY_IDX_D);
	if (keysym == KEY_LEFT)
		return (KEY_IDX_LEFT);
	if (keysym == KEY_RIGHT)
		return (KEY_IDX_RIGHT);
	return (-1);
}

static int	key_press(int keysym, t_game *game)
{
	int		idx;

	if (keysym == KEY_ESC)
		return (close_lsr(game));
	idx = get_key_idx(keysym);
	if (idx >= 0)
		game->keys[idx] = true;
	return (0);
}

static int	key_release(int keysym, t_game *game)
{
	int		idx;

	idx = get_key_idx(keysym);
	if (idx >= 0)
		game->keys[idx] = false;
	return (0);
}

void	render_listener(t_game *game)
{
	mlx_hook(game->win, KeyPress, KeyPressMask, key_press, game);
	mlx_hook(game->win, KeyRelease, KeyReleaseMask, key_release, game);
	mlx_hook(game->win, DestroyNotify, StructureNotifyMask, close_lsr, game);
	mlx_hook(game->win, MotionNotify, PointerMotionMask, handle_mouse, game);
	mlx_loop_hook(game->mlx, init_game_loop, game);
}
