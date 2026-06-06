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

int	parse_path(const char *value, char **dest)
{
	if (*dest)
		return (parse_error("Duplicate texture identifier"));
	*dest = ft_strdup(value, true);
	if (!*dest)
		return (parse_error("Memory allocation failed"));
	return (0);
}
