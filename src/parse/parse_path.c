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

#include "main.h"

int	parse_path(char **dest, const char *value)
{
	if (*dest)
		return (parse_error("Duplicate texture identifier"));
	*dest = ft_strdup(value);
	if (!*dest)
		return (parse_error("Memory allocation failed"));
	return (0);
}
