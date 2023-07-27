#include "shell.h"

/**
 * clear_info - initializes info_t struct
 * @data: struct address
 */

void clear_info(data_d *data)
{
	data->arg = NULL;
	data->argv = NULL;
	data->path = NULL;
	data->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @data: struct address
 * @av: argument vector
 */

void set_info(data_d *data, char **av)
{
	int i = 0;

	data->fname = av[0];
	if (data->arg)
	{
		data->argv = strtow(data->arg, " \t");
		if (!data->argv)
		{
			data->argv = malloc(sizeof(char *) * 2);
			if (data->argv)
			{
				data->argv[0] = _strdup(data->arg);
				data->argv[1] = NULL;
			}
		}
		for (i = 0; data->argv && data->argv[i]; i++)
			;
		data->argc = i;

		replace_alias(data);
		replace_vars(data);
	}
}

/**
 * free_info - frees info_t struct fields
 * @data: struct address
 * @all: true if freeing all fields
 */

void free_info(data_d *data, int all)
{
	_free(data->argv);
	data->argv = NULL;
	data->path = NULL;
	if (all)
	{
		if (!data->cmd_buff)
			free(data->arg);
		if (data->env)
			free_list(&(data->env));
		if (data->hist)
			free_list(&(data->hist));
		if (data->alias)
			free_list(&(data->alias));
		_free(data->environ);
			data->environ = NULL;
		bfree((void **)data->cmd_buff);
		if (data->readfd > 2)
			close(data->readfd);
		_putchar(BUFF_FLUSH);
	}
}
