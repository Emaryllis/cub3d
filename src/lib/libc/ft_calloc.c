/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:40:26 by egoh              #+#    #+#             */
/*   Updated: 2025/09/11 18:45:29 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static void	ft_bzero(void *s, size_t len) // Just memset but always 0
{
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	while (len--)
		*ptr++ = 0;
}

void	*ft_calloc(size_t amt, size_t size)
{
	size_t	total_size;
	void	*ptr;

	if (amt == 0 || size == 0)
		return (malloc(1));
	if (amt > SIZE_MAX / size)
		return (NULL);
	total_size = amt * size;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, total_size);
	return (ptr);
}
