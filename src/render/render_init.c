/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 04:22:25 by egoh              #+#    #+#             */
/*   Updated: 2026/06/18 04:44:16 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

int	render_init(t_game *game)
{
	game->win = mlx_new_window(game->mlx, WIN_W, WIN_H, WIN_TITLE);
	if (!game->win)
		return (parse_error(MLX_WIN_ERR));
	game->screen.img_ptr = mlx_new_image(game->mlx, WIN_W, WIN_H);
	if (!game->screen.img_ptr)
		return (parse_error(MLX_SCREEN_ERR));
	game->screen.pixels = (int *)mlx_get_data_addr(game->screen.img_ptr,
			&game->screen.bpp, &game->screen.line_len, &game->screen.endian);
	game->config.player.last_mouse_x = 0;
	game->config.player.mouse_captured = false;
	if (INFO)
		printf("Game struct initialized successfully!\n");
	return (0);
}

int	init_game_loop(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (game->keys[KEY_IDX_LEFT])
		rotate_player(&game->config.player, -ROT_SPEED);
	if (game->keys[KEY_IDX_RIGHT])
		rotate_player(&game->config.player, ROT_SPEED);
	move_player(game);
	raycast(game);
	return (0);
}

static void	init_step(t_init_step step)
{
	*step.step = 1;
	*step.dist = (step.map + 1.0 - step.pos) * step.delta;
	if (step.dir < 0)
	{
		*step.step = -1;
		*step.dist = (step.pos - step.map) * step.delta;
	}
}

void	init_ray(const t_game *game, int x, t_ray *ray)
{
	const t_plyr	*plyr = &game->config.player;

	ray->camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray->dir_x = plyr->dir_x + plyr->plane_x * ray->camera_x;
	ray->dir_y = plyr->dir_y + plyr->plane_y * ray->camera_x;
	ray->map_x = (int)plyr->pos_x;
	ray->map_y = (int)plyr->pos_y;
	if (ray->dir_x != 0)
		ray->delta_dist_x = fabs(1.0 / ray->dir_x);
	else
		ray->delta_dist_x = LARGE_NUM;
	if (ray->dir_y != 0)
		ray->delta_dist_y = fabs(1.0 / ray->dir_y);
	else
		ray->delta_dist_y = LARGE_NUM;
	init_step((t_init_step){&ray->side_dist_x, ray->dir_x,
		plyr->pos_x, ray->delta_dist_x, &ray->step_x, ray->map_x});
	init_step((t_init_step){&ray->side_dist_y, ray->dir_y,
		plyr->pos_y, ray->delta_dist_y, &ray->step_y, ray->map_y});
	ray->side = 0;
}
