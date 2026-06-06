/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 11:14:53 by egoh              #+#    #+#             */
/*   Updated: 2026/05/12 11:41:48 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_empty(const char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '\n')
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

int	parse_error(const char *msg)
{
	const char	*err = strerror(errno);

	write(STDERR_FILENO, "Error: ", 8);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "! (", 3);
	write(STDERR_FILENO, err, ft_strlen(err));
	write(STDERR_FILENO, ")\n", 2);
	return (-1);
}
