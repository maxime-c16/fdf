/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:54:29 by mcauchy           #+#    #+#             */
/*   Updated: 2025/05/19 13:36:00 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static int	open_and_parse(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error: Could not open file\n", 2);
		return (0);
	}
	close(fd);
	parsing(filename);
	return (1);
}

static void	setup_hooks(t_fdf *fdf)
{
	mlx_hook(fdf->win, 17, 0, on_close, 0);
	mlx_hook(fdf->win, ON_KEYDOWN, 1L << 0, key_hook, 0);
	mlx_hook(fdf->win, 4, 1L << 0, mouse_press, 0);
	mlx_hook(fdf->win, 5, 0, mouse_release, 0);
	mlx_hook(fdf->win, 6, 0, mouse_move, 0);
}

int	main(int ac, char **av)
{
	t_fdf	*fdf;

	if (ac != 2)
	{
		ft_putstr_fd("Usage: ./fdf <filename>\n", 2);
		return (1);
	}
	if (!open_and_parse(av[1]))
		return (1);
	fdf = _fdf();
	compute_height_factor(fdf);
	draw_map();
	setup_hooks(fdf);
	mlx_loop(fdf->mlx);
	return (0);
}
