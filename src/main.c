/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egoh <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 00:33:06 by egoh              #+#    #+#             */
/*   Updated: 2026/05/12 13:48:45 by egoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	init_struct(t_game *game, const char *file_path, char **envp)
{
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, WIN_W, WIN_H, WIN_TITLE);
	game->envp = envp;
	return (parse_file(game, file_path));
}

int	main(int ac, char **av, char **envp)
{
	t_game	game;

	if (ac != 2 || av[1][0] == '\0')
		return (parse_error(NO_FILE) + 2);
	game = (t_game){0};
	if (init_struct(&game, av[1], envp) == -1)
		return (cleanup(&game), EXIT_FAILURE);
	init_listener(&game);
	mlx_loop(game.mlx);
	cleanup(&game);
	return (EXIT_SUCCESS);
}
