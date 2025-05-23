/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:28:39 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/23 14:48:51 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static int	open_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error: open() failed\n", 2);
		exit(1);
	}
	return (fd);
}

static int	count_lines_loop(int fd)
{
	int		lines;
	char	*line;

	lines = 0;
	line = get_next_line(fd);
	while (line)
	{
		lines++;
		free(line);
		line = get_next_line(fd);
	}
	return (lines);
}

static void	count_width(char *line)
{
	int		width;
	char	**split;

	width = 0;
	split = ft_split(line, ' ');
	if (!split)
	{
		free(line);
		ft_putstr_fd("Error: ft_split() failed\n", 2);
		exit(1);
	}
	while (split[width])
		width++;
	ft_free_tab(split);
	_fdf()->width = width;
}

void	count_lines(char *filename)
{
	int		fd;
	int		lines;
	char	*line;

	fd = open_file(filename);
	line = get_next_line(fd);
	if (!line)
	{
		ft_putstr_fd("Error: get_next_line() failed\n", 2);
		close(fd);
		on_close();
		free(line);
		exit(1);
	}
	count_width(line);
	lines = 1 + count_lines_loop(fd);
	free(line);
	close(fd);
	_fdf()->height = lines;
}
