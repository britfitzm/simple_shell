#include "shell.h"

/**
 * get_hist_file - gets history file
 * @info: parameter struct
 * Return: allocated string containg history file
 */

char *get_hist_file(info_t *info)
{
	char *buff, *dir;

	dir = _getenv(info, "HOME=");
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
 * @info: parameter struct
 * Return: 1 on success, else -1
 */

int write_hist(info_t *info)
{
	ssize_t fd;
	char *filename = get_hist_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->hist; node; node = node->next)
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
 * @info: parameter struct
 * Return: histcount on success, 0 otherwise
 */

int read_hist(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buff = NULL, *filename = get_hist_file(info);

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
			build_hist_list(info, buff + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_hist_list(info, buff + last, linecount++);
	free(buff);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->hist), 0);
	renum_hist(info);
	return (info->histcount);
}

/**
 * build_hist_list - adds entry to history linked list
 * @info: structure containing potential arguments. Used to maintain
 * @buff: buffer
 * @linecount: history linecount, histcount
 * Return: always 0
 */

int build_hist_list(info_t *info, char *buff, int linecount)
{
	list_t *node = NULL;

	if (info->hist)
		node = info->hist;
	add_node_end(&node, buff, linecount);

	if (!info->hist)
		info->hist = node;
	return (0);
}

/**
 * renum_hist - renumbers history linked list after changes
 * @info: structure containing potential arguments. Used to maintain
 * Return: new histcount
 */

int renum_hist(info_t *info)
{
	list_t *node = info->hist;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}
