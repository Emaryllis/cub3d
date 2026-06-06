/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 14:19:44 by egoh              #+#    #+#             */
/*   Updated: 2026/05/14 14:19:44 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stddef.h>

static void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;
	unsigned char	*s;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	while (n--)
		*d++ = *s++;
	return (dest);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*newp;
	size_t	to_copy;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	if (new_size == old_size)
		return (ptr);
	newp = malloc(new_size);
	if (!newp)
		return (NULL);
	if (old_size < new_size)
		to_copy = old_size;
	else
		to_copy = new_size;
	if (to_copy)
		ft_memcpy(newp, ptr, to_copy);
	free(ptr);
	return (newp);
}
