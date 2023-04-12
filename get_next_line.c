/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tscasso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 23:36:39 by tscasso           #+#    #+#             */
/*   Updated: 2023/03/26 18:12:38 by tscasso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	append(char **s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(*s1, s2);
	free(*s1);
	*s1 = tmp;
}

static char	*save_and_clear(char **line)
{
	char	*out;
	char	*tmp;
	char	*newline;

	newline = ft_strchr(*line, '\n');
	if (newline)
	{
		out = ft_substr(*line, 0, newline - *line + 1);
		tmp = ft_strdup(newline +1);
		free(*line);
		*line = tmp;
		if (!**line)
		{
			free(*line);
			*line = NULL;
		}
		return (out);
	}
	out = ft_strdup(*line);
	free(*line);
	*line = NULL;
	return (out);
}

static int	read_and_append(char **line, int fd)
{
	ssize_t	bytes_read;
	char	*buf;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (-1);
	bytes_read = read(fd, buf, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buf[bytes_read] = 0;
		if (!*line)
			*line = ft_strdup("");
		append(line, buf);
		if (ft_strchr(buf, '\n'))
			break ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	*persistent;
	char		*return_line;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	bytes_read = read_and_append(&persistent, fd);
	if (bytes_read < 0)
		return (NULL);
	if (!bytes_read && !persistent)
		return (NULL);
	return_line = save_and_clear(&persistent);
	return (return_line);
}
