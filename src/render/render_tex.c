/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_tex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 01:15:46 by egoh              #+#    #+#             */
/*   Updated: 2026/06/19 01:15:46 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

static void	draw_ceiling(t_game *game, int x, const t_ray *ray)
{
	int	y;

	y = 0;
	while (y < ray->draw_start)
		game->screen.pixels[y++ *WIN_W + x] = game->config.ceil_color;
}

static void	draw_floor(t_game *game, int x, const t_ray *ray)
{
	int	y;

	y = ray->draw_end;
	while (y < WIN_H)
		game->screen.pixels[y++ *WIN_W + x] = game->config.floor_color;
}

static void	draw_wall(t_game *game, int x, const t_ray *ray, const t_img *tex)
{
	double	step;
	double	tex_pos;
	int		tex_xy[2];
	int		y;

	tex_xy[0] = (int)(ray->wall_x * tex->width);
	if ((ray->side == 0 && ray->dir_x < 0)
		|| (ray->side == 1 && ray->dir_y > 0))
		tex_xy[0] = tex->width - tex_xy[0] - 1;
	step = (double)tex->height / ray->line_height;
	tex_pos = (ray->draw_start - (WIN_H - ray->line_height) / 2.0) * step;
	y = ray->draw_start;
	while (y < ray->draw_end)
	{
		tex_xy[1] = (int)tex_pos % tex->height;
		if (tex_xy[1] < 0)
			tex_xy[1] += tex->height;
		game->screen.pixels[y * WIN_W + x] = tex->pixels[tex_xy[1]
			* tex->width + tex_xy[0]];
		tex_pos += step;
		y++;
	}
}

void	draw_vertical_strip(t_game *game, int x, const t_ray *ray,
	const t_img *tex)
{
	draw_ceiling(game, x, ray);
	draw_floor(game, x, ray);
	draw_wall(game, x, ray, tex);
}
