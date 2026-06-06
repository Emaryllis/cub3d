/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 14:40:26 by egoh              #+#    #+#             */
/*   Updated: 2025/09/11 20:14:41 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stddef.h>

static size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*joined_str;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	joined_str = malloc(sizeof(char) * (len_s1 + len_s2 + 1));
	if (!joined_str)
		return (NULL);
	i = 0;
	while (i < len_s1)
	{
		joined_str[i] = s1[i];
		i++;
	}
	while (i < len_s1 + len_s2)
	{
		joined_str[i] = s2[i - len_s1];
		i++;
	}
	joined_str[i] = '\0';
	return (joined_str);
}
