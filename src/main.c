/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 09:55:39 by egoh              #+#    #+#             */
/*   Updated: 2026/06/18 04:44:07 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	init_struct(t_game *game, const char *file_path, char **envp)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (parse_error(MLX_INIT_ERR));
	game->envp = envp;
	if (parse_file(game, file_path) == -1)
		return (-1);
	if (INFO)
		printf("File parsed successfully!\n");
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_game	game;

	if (ac != 2 || av[1][0] == '\0')
		return (parse_error(NO_FILE) + 2);
	game = (t_game){0};
	if (init_struct(&game, av[1], envp) == -1)
		return (cleanup(&game), EXIT_FAILURE);
	render(&game);
	mlx_loop(game.mlx);
	cleanup(&game);
	return (EXIT_SUCCESS);
}
