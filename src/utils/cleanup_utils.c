/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 13:21:07 by egoh              #+#    #+#             */
/*   Updated: 2026/05/14 13:21:07 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	free_arr_range(char **arr, int start, int end)
{
	int	i;

	i = start;
	while (i < end)
		free(arr[i++]);
	free(arr);
}

void	free_arr(char **arr, int start)
{
	while (arr[start])
		free(arr[start++]);
	free(arr);
}
