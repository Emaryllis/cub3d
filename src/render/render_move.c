/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 07:38:58 by egoh              #+#    #+#             */
/*   Updated: 2026/07/01 17:32:15 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

/**
 * Makes mouse movement smooth
 * The delta check ensures the mouse doesn't make the fov snap when the mouse
 * pointer enters the window. I would have liked to capture the mouse movement
 * without actually moving the mouse, but that would require forbidden X11
 * function calls so (╯°□°)╯︵ ┻━┻
 */
int	handle_mouse(int x, int y, void *param)
{
	t_plyr	*plyr;
	int		delta;

	(void)y;
	plyr = &((t_game *)param)->config.player;
	if (!plyr->mouse_captured)
	{
		plyr->last_mouse_x = x;
		plyr->mouse_captured = true;
		return (0);
	}
	delta = x - plyr->last_mouse_x;
	plyr->last_mouse_x = x;
	if (delta > 100 || delta < -100)
		return (0);
	rotate_player(plyr, delta * MOUSE_SENSITIVITY);
	return (0);
}

/** Rotates the player by setting direction and plane using simple trig */
void	rotate_player(t_plyr *plyr, double rot_speed)
{
	const double	old_dir_x = plyr->dir_x;
	const double	old_plane_x = plyr->plane_x;

	plyr->dir_x = plyr->dir_x * cos(rot_speed) - plyr->dir_y * sin(rot_speed);
	plyr->dir_y = old_dir_x * sin(rot_speed) + plyr->dir_y * cos(rot_speed);
	plyr->plane_x = plyr->plane_x * cos(rot_speed) - plyr->plane_y
		* sin(rot_speed);
	plyr->plane_y = old_plane_x * sin(rot_speed) + plyr->plane_y
		* cos(rot_speed);
}

static bool	is_wall(const t_map *map, double x, double y)
{
	const int	map_x = (int)x;
	const int	map_y = (int)y;

	if (map_y < 0 || map_y >= (int)map->height
		|| map_x < 0 || map_x >= (int)map->width)
		return (true);
	return (map->grid[map_y * map->width + map_x] == TILE_WALL);
}

/**
 * Calculates dx & dy by taking advantage of bool state arithmetic
 * Simple checks for wall collisions that ignores movement to
 * prevent wall bouncing when pressing 2 keys at once.
 */
void	move_player(t_game *game)
{
	t_map	*map;
	t_plyr	*p;
	double	dx;
	double	dy;

	map = &game->config.map;
	p = &game->config.player;
	dx = (p->dir_x * (game->keys[KEY_IDX_W] - game->keys[KEY_IDX_S])
			+ p->dir_y * (game->keys[KEY_IDX_A] - game->keys[KEY_IDX_D]))
		* MOVE_SPEED;
	dy = (p->dir_y * (game->keys[KEY_IDX_W] - game->keys[KEY_IDX_S])
			- p->dir_x * (game->keys[KEY_IDX_A] - game->keys[KEY_IDX_D]))
		* MOVE_SPEED;
	if (dx == 0 && dy == 0)
		return ;
	if (!is_wall(map, p->pos_x + dx + PLYR_HITBOX * ((dx > 0) - (dx < 0)),
			p->pos_y))
		p->pos_x += dx;
	if (!is_wall(map, p->pos_x, p->pos_y + dy + PLYR_HITBOX
			* ((dy > 0) - (dy < 0))))
		p->pos_y += dy;
}
