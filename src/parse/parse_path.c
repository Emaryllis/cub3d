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

static int	is_valid_path(const char *path)
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

static int	expand_tilde(char **dest, char **envp)
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
 *
 * @param value Value of header key
 * @param dest Storage address of value
 * @return 0 on success, -1 on failure
 */
int	parse_path(const char *value, char **dest, char **envp)
{
	if (!value)
		return (parse_error(NO_TEXTURE_FILE));
	if (*dest)
		return (parse_error(DUP_TEXTURE_ID));
	*dest = ft_strdup(value, true);
	if (!*dest)
		return (parse_error(TEXTURE_MALLOC_ERR));
	if (!ft_endswith(*dest, TEXTURE_EXT, ft_strlen(*dest)))
		return (parse_error(INVALID_TEXTURE_EXT));
	if (**dest == '~' && expand_tilde(dest, envp) == -1)
		return (-1);
	return (is_valid_path(*dest));
}
