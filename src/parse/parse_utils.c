/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 15:35:17 by egoh              #+#    #+#             */
/*   Updated: 2026/07/01 16:26:08 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

/** Checks if the current string (get_next_line) is a valid map line */
int	is_map_line(const char *line)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '\n')
	{
		if (line[i] != ' ' && line[i] != '0' && line[i] != '1'
			&& line[i] != 'N' && line[i] != 'S'
			&& line[i] != 'E' && line[i] != 'W')
			return (0);
		i++;
	}
	return (i > 0);
}

/** Used to validate the amount of numbers given per color line */
int	check_commas(const char *line)
{
	int	i;

	if (!line)
		return (-1);
	i = 0;
	while (*line)
	{
		if (*line == ',')
			i++;
		line++;
	}
	return (i);
}

static void	ft_putnbr_fd(size_t n, int fd)
{
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	write(fd, &(char){n % 10 + '0'}, 1);
}

/**
 * Detailed error reporting because looking through 1s just to
 * find 0(s) that shouldn't be there is a disservice to my eyes.
 * @return -1 (only used on failure)
 */
int	hole_loc_err(size_t x, size_t y)
{
	write(STDERR_FILENO, "Error: Found hole in the map at line ", 37);
	ft_putnbr_fd(y + 1, STDERR_FILENO);
	write(STDERR_FILENO, ", column ", 9);
	ft_putnbr_fd(x + 1, STDERR_FILENO);
	write(STDERR_FILENO, "!\n", 2);
	return (-1);
}
