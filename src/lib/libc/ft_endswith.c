/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_endswith.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/07 07:28:18 by egoh              #+#    #+#             */
/*   Updated: 2026/06/07 07:28:18 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

/**
 * Checks if a string ends with a given suffix.
 * @param str The string to check.
 * @param suffix The suffix to look for.
 * @param str_len The pre-calculated length of str.
 * @return int 1 if it matches, 0 if it doesn't.
 */
int	ft_endswith(const char *str, const char *suffix, size_t str_len)
{
	size_t	suf_len;

	if (!str || !suffix)
		return (0);
	suf_len = 0;
	while (suffix[suf_len])
		suf_len++;
	if (suf_len > str_len)
		return (0);
	while (suf_len > 0)
	{
		if (str[str_len - 1] != suffix[suf_len - 1])
			return (0);
		str_len--;
		suf_len--;
	}
	return (1);
}
