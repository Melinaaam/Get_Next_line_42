/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: memotyle <memotyle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 11:13:12 by memotyle          #+#    #+#             */
/*   Updated: 2025/03/18 12:51:31 by memotyle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* Steps :
first : read the file
second : store the read in a static variable -> storage
third : check if there is a '\n' in storage
	- if there is a '\n' -> create a new variable 'line'
	that will store everything from storage until the '\n'
	- if there is no '\n' -> read again the file and store the read in storage
fourth : return line
*/

static char	*free_storage(char **storage)
{
	if (*storage)
	{
		free(*storage);
		*storage = NULL;
	}
	return (NULL);
}
/*
here -> free strage more properly in a function
*/

static int	join_storage_and_buffer(char **storage, char *buffer)
{
	char	*temp;

	if (!*storage)
	{
		*storage = ft_strdup(buffer);
		if (!*storage)
			return (0);
	}
	else
	{
		temp = ft_strjoin(*storage, buffer);
		if (!temp)
			return (0);
		free_storage(storage);
		*storage = temp;
	}
	return (1);
}
/*
here -> join storage and buffer
We use ft_strdup to copy buffer in storage
if storage is not empty, we use ft_strjoin to join storage and buffer
We return 1 if everything is ok, 0 if there is an error
The result returned by join_storage_and_buffer is used in ft_read, for the while loop to read the file because we need to read the file until we find a '\n'
*/

static char	*generate_line(char **storage)
{
	size_t	len;
	char	*line;
	char	*temp;

	if (!*storage || **storage == '\0')
		return (NULL);
	if (ft_strchr(*storage, '\n'))
		len = ft_strchr(*storage, '\n') - *storage + 1;
	else
		len = ft_strlen(*storage);
	line = malloc((len + 1) * sizeof(char));
	if (!line)
		return (NULL);
	ft_strlcpy(line, *storage, len + 1);
	if (ft_strchr(*storage, '\n'))
	{
		temp = ft_strdup(ft_strchr(*storage, '\n') + 1);
		free_storage(storage);
		*storage = temp;
	}
	else
		free_storage(storage);
	return (line);
}
/*
Here -> generate line
If there is a '\n' in storage, we calculate the length of the line
If there is no '\n' in storage, we calculate the length of the line

If there is a '\n' in storage, we copy the rest of storage in temp
We free storage and we store temp in storage
If there is no '\n' in storage, we free storage
We return line

*/

static int	ft_read(int fd, char **storage)
{
	char	*buffer;
	int		bytes_read;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (-1);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		if (!join_storage_and_buffer(storage, buffer))
		{
			free(buffer);
			return (-1);
		}
		if (ft_strchr(buffer, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	return (bytes_read);
}
/*
here -> ft_read
We read the file and store the read in buffer
We use join_storage_and_buffer to join storage and buffer
If there is a '\n' in buffer, we break the loop
If there is no '\n' in buffer, we read the file again and store the read in buffer

The result returned by ft_read is used in get_next_line,
for the while loop to read the file because we need to read the file until we find a '\n'


*/

char	*get_next_line(int fd)
{
	static char	*storage = NULL;
	char		*line;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX - 1)
		return (NULL);
	bytes_read = ft_read(fd, &storage);
	if (bytes_read < 0 || (bytes_read == 0 && (!storage || !*storage)))
		return (free_storage(&storage));
	line = generate_line(&storage);
	if (!line || *line == '\0')
	{
		free(line);
		return (free_storage(&storage));
	}
	return (line);
}

/*
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	int fd;
	char *line;
	char *name = "gnl.txt";

	fd = open(name, O_RDONLY);
	line = get_next_line(fd);
	printf("%s\n", line);
	free(line);
	close(fd);
	return (0);
}
*/
