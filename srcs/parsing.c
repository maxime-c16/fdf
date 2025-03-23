/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:40:36 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/23 10:38:16 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	print_map(void)
{
	t_fdf	*fdf;
	int		i;
	int		j;

	fdf = _fdf();
	i = 0;
	while (i < fdf->height)
	{
		j = 0;
		while (j < fdf->width)
		{
			printf("%d ", fdf->map[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

static void	count_lines(char *filename)
{
	int		fd;
	int		lines;

	lines = 0;
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error: open() failed\n", 2);
		exit(1);
	}
	while (get_next_line(fd))
		lines++;
	close(fd);
	_fdf()->height = lines;
	printf("height: %d\n", _fdf()->height);
}

static void	insert_values(char *line, int i)
{
	t_fdf	*fdf;
	char	**split;
	int		j;

	j = 0;
	fdf = _fdf();
	split = ft_split(line, ' ');
	if (!split)
	{
		ft_putstr_fd("Error: ft_split() failed\n", 2);
		exit(1);
	}
	while (split[j])
	{
		fdf->map[i][j] = ft_atoi(split[j]);
		free(split[j]);
		j++;
	}
	fdf->width = j;
	free(split);
}

static void	read_from_fd(int fd)
{
	char	*line;
	int		i;

	i = 0;
	line = get_next_line(fd);
	_fdf()->map = (int **)malloc(sizeof(int *) * _fdf()->height);
	if (!_fdf()->map)
	{
		ft_putstr_fd("Error: malloc() failed\n", 2);
		exit(1);
	}
	while (line)
	{
		_fdf()->map[i] = (int *)malloc(sizeof(int) * _fdf()->width);
		if (!_fdf()->map[i])
		{
			ft_putstr_fd("Error: malloc() failed\n", 2);
			exit(1);
		}
		insert_values(line, i);
		i++;
		free(line);
		line = get_next_line(fd);
	}
}

void	parsing(char *filename)
{
	t_fdf	*fdf;

	fdf = _fdf();
	count_lines(filename);
	fdf->fd = open(filename, O_RDONLY);
	if (fdf->fd < 0)
	{
		ft_putstr_fd("Error: open() failed\n", 2);
		exit(1);
	}
	read_from_fd(fdf->fd);
	close(fdf->fd);
	fdf->center_x = ((fdf->width - 1) * fdf->camera.zoom) / 2.0;
	fdf->center_y = ((fdf->height - 1) * fdf->camera.zoom) / 2.0;
}
