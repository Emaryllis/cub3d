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
	write(2, "Error: ", 8);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 2);
	return (-1);
}

/**
 * Doubles element capacity and reallocates the buffer.
 * On success: updates *ptr and *capacity.
 * On failure: frees *ptr, sets *ptr to NULL, and returns NULL.
 * @param ptr       Address of the buffer pointer to grow.
 * @param size      Size of one element (e.g. sizeof(char *)).
 * @param capacity  Number of elements in the pointer.
 * @return          New buffer pointer on success, NULL on failure.
 */
void	*dynamic_realloc(void **ptr, size_t size, size_t *capacity)
{
	void	*newp;

	newp = ft_realloc(ptr, size * *capacity, size * *capacity * 2);
	if (!newp)
	{
		free(*ptr);
		*ptr = NULL;
		return (NULL);
	}
	*capacity = *capacity * 2;
	*ptr = newp;
	return (newp);
}
