#include "shell.h"

/**
 * get_hist_file - gets history file
 * @data: parameter struct
 * Return: allocated string containg history file
 */

char *get_hist_file(data_d *data)
{
	char *buff, *dir;

	dir = _getenv(data, "HOME=");
	if (!dir)
		return (NULL);
	buff = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buff)
		return (NULL);
	buff[0] = 0;
	_strcpy(buff, dir);
	_strcat(buff, "/");
	_strcat(buff, HIST_FILE);
	return (buff);
}

/**
 * write_hist - creates file, or appends to existing file
 * @data: parameter struct
 * Return: 1 on success, else -1
 */

int write_hist(data_d *data)
{
	ssize_t fd;
	char *filename = get_hist_file(data);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = data->hist; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUFF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_hist - reads history from file
 * @data: parameter struct
 * Return: histcount on success, 0 otherwise
 */

int read_hist(data_d *data)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buff = NULL, *filename = get_hist_file(data);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buff = malloc(sizeof(char) * (fsize + 1));
	if (!buff)
		return (0);
	rdlen = read(fd, buff, fsize);
	buff[fsize] = 0;
	if (rdlen <= 0)
		return (free(buff), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buff[i] == '\n')
		{
			buff[i] = 0;
			build_hist_list(data, buff + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_hist_list(data, buff + last, linecount++);
	free(buff);
	data->histcount = linecount;
	while (data->histcount-- >= HIST_MAX)
		delete_node_at_index(&(data->hist), 0);
	renum_hist(data);
	return (data->histcount);
}

/**
 * build_hist_list - adds entry to history linked list
 * @data: structure containing potential arguments. Used to maintain
 * @buff: buffer
 * @linecount: history linecount, histcount
 * Return: always 0
 */

int build_hist_list(data_d *data, char *buff, int linecount)
{
	list_t *node = NULL;

	if (data->hist)
		node = data->hist;
	add_node_end(&node, buff, linecount);

	if (!data->hist)
		data->hist = node;
	return (0);
}

/**
 * renum_hist - renumbers history linked list after changes
 * @data: structure containing potential arguments. Used to maintain
 * Return: new histcount
 */

int renum_hist(data_d *data)
{
	list_t *node = data->hist;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (data->histcount = i);
}
