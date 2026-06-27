/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 07:02:01 by egoh              #+#    #+#             */
/*   Updated: 2026/06/19 03:53:52 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static t_img	*select_texture(t_config *config, uint8_t side,
	double ray_dir_x, double ray_dir_y)
{
	if (side == 0)
	{
		if (ray_dir_x > 0)
			return (&config->tex_ea);
		return (&config->tex_we);
	}
	if (ray_dir_y > 0)
		return (&config->tex_so);
	return (&config->tex_no);
}

static void	cast_ray(const t_game *game, t_ray *ray)
{
	int	hit;

	hit = 0;
	while (!hit)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_y < 0 || ray->map_y >= (int)game->config.map.height
			|| ray->map_x < 0 || ray->map_x >= (int)game->config.map.width)
			break ;
		if (game->config.map.grid[ray->map_y * game->config.map.width
				+ ray->map_x] == TILE_WALL)
			hit = 1;
	}
}

static void	calc_wall_bounds(t_ray *ray)
{
	if (ray->side == 0)
		ray->perp_wall_dist = ray->side_dist_x - ray->delta_dist_x;
	else
		ray->perp_wall_dist = ray->side_dist_y - ray->delta_dist_y;
	if (ray->perp_wall_dist <= DBL_EPSILON)
		ray->perp_wall_dist = DBL_EPSILON;
	ray->line_height = (int)(WIN_H / ray->perp_wall_dist);
	ray->draw_start = (-ray->line_height + WIN_H) / 2;
	ray->draw_end = (ray->line_height + WIN_H) / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	if (ray->draw_end >= WIN_H)
		ray->draw_end = WIN_H;
}

static void	calc_tex_coords(t_game *game, t_ray *ray, t_img **tex)
{
	const t_plyr	*plyr = &game->config.player;

	if (ray->side == 0)
		ray->wall_x = plyr->pos_y + ray->perp_wall_dist * ray->dir_y;
	else
		ray->wall_x = plyr->pos_x + ray->perp_wall_dist * ray->dir_x;
	ray->wall_x -= floor(ray->wall_x);
	*tex = select_texture(&game->config, ray->side,
			ray->dir_x, ray->dir_y);
}

void	raycast(t_game *game)
{
	t_ray	ray;
	t_img	*tex;
	int		x;

	x = 0;
	while (x < WIN_W)
	{
		init_ray(game, x, &ray);
		cast_ray(game, &ray);
		calc_wall_bounds(&ray);
		calc_tex_coords(game, &ray, &tex);
		draw_vertical_strip(game, x, &ray, tex);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->screen.img_ptr, 0, 0);
}
