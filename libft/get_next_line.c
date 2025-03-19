/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:53:17 by mcauchy           #+#    #+#             */
/*   Updated: 2025/03/19 13:57:22 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*read_line(int fd, char *line)
{
	char	buffer[BUFFER_SIZE + 1];
	int		bytes_read;
	char	*temp;

	bytes_read = 1;
	while (bytes_read > 0 && has_newline(line) < 1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0 || (bytes_read == 0 && !line))
		{
			free(line);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp = ft_strjoin_gnl(line, buffer);
		free(line);
		line = ft_strdup_gnl(temp);
		free(temp);
	}
	if (bytes_read == 0 && !line)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*line = NULL;
	char		*str;
	char		*temp;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = read_line(fd, line);
	if (!line)
		return (NULL);
	str = ft_substr(line, 0, has_newline(line) + 1);
	if (!str && line && line[0] != '\0')
	{
		str = ft_strdup(line);
		free(line);
	}
	else
	{
		temp = ft_strdup(line + has_newline(line) + 1);
		free(line);
		line = ft_strdup(temp);
		free(temp);
	}
	return (str);
}
