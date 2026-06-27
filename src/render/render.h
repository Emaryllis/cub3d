/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 04:19:45 by egoh              #+#    #+#             */
/*   Updated: 2026/06/19 03:52:32 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "main.h"

# define WIN_W 800
# define WIN_H 600
# define LARGE_NUM 1e30
# define MOVE_SPEED 0.005 // Tiles per frame
# define ROT_SPEED 0.003 // Radians per frame
# define MOUSE_SENSITIVITY 0.003
# define PLYR_HITBOX 0.12
# define WIN_TITLE "cub3d"

typedef struct s_ray
{
	double	camera_x;
	double	dir_x;
	double	dir_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	double	perp_wall_dist;
	double	wall_x; // Exact hit point on wall face (0.0 to 1.0)
	int		step_x;
	int		step_y;
	int		map_x;
	int		map_y;
	int		draw_start;
	int		draw_end;
	int		line_height;
	uint8_t	side; // 0 = NS face, 1 = EW face
}	t_ray;

typedef struct s_init_step
{
	double	*dist;
	double	dir;
	double	pos;
	double	delta;
	int		*step;
	int		map;
}	t_init_step;

// Initialization
int		render_init(t_game *game);
void	init_ray(const t_game *game, int x, t_ray *ray);

void	draw_vertical_strip(t_game *game, int x, const t_ray *ray,
			const t_img *tex);

// Listeners
void	render_listener(t_game *game);
int		init_game_loop(void *param);
int		handle_mouse(int x, int y, void *param);
void	rotate_player(t_plyr *plyr, double rot_speed);
void	move_player(t_game *game);
void	raycast(t_game *game);

#endif
