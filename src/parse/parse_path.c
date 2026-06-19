/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:53:30 by egoh              #+#    #+#             */
/*   Updated: 2026/05/14 12:53:30 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static int	is_path(const char *path)
{
	int		fd;
	char	buffer[1];

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (parse_error(OPEN_TEXTURE_ERR));
	if (read(fd, buffer, 0) < 0)
	{
		close(fd);
		return (parse_error(TEXTURE_DIR_ERR));
	}
	close(fd);
	return (0);
}

static int	res_tilde(char **dest, char **envp)
{
	size_t	i;
	char	*tmp;

	if (!envp || !*envp || !**envp)
		return (parse_error(NO_ENV_ERR));
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "HOME=", 5) == 0)
		{
			tmp = ft_strjoin(envp[i] + 5, *dest + 1);
			if (!tmp)
				return (parse_error(HOME_MALLOC_ERR));
			free(*dest);
			*dest = tmp;
			return (0);
		}
		i++;
	}
	return (parse_error(NO_ENV_ERR));
}

/**
 * Parses, validates and expands tilde (~) for path.
 * No need to expand minus (-) for old pwd since that
 * is only expected for cd builtin. No need to free
 * on error since main cleanup does that already.
 * Null check isn't needed for pixels since
 * mlx_get_data_addr is an image setter wrapper
 *
 * @param value Value of header key
 * @return 0 on success, -1 on failure
 */
int	parse_path(const char *value, t_img *img, void *mlx, char **envp)
{
	char	*dest;

	if (!value)
		return (parse_error(NO_TEXTURE_FILE));
	if (!ft_endswith(value, TEXTURE_EXT, ft_strlen(value)))
		return (parse_error(INVALID_TEXTURE_EXT));
	if (img->img_ptr)
		return (parse_error(DUP_TEXTURE_ID));
	dest = ft_strdup(value, true);
	if (!dest)
		return (parse_error(TEXTURE_MALLOC_ERR));
	if ((*dest == '~' && res_tilde(&dest, envp) == -1) || is_path(dest) == -1)
		return (free(dest), -1);
	img->img_ptr = mlx_xpm_file_to_image(mlx, dest, &img->width, &img->height);
	free(dest);
	if (!img->img_ptr)
		return (parse_error(CONVERT_MALLOC_ERR));
	img->pixels = (int *)mlx_get_data_addr(img->img_ptr, &img->bpp,
			&img->line_len, &img->endian);
	if (PARSE_DEBUG)
		printf("Texture path: %s", value);
	return (0);
}
