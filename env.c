#include "shell.h"

/**
 * _myenv - prints current environment
 * @data: structure containing potential arguments. Used to maintain
 * Return: always 0
 */

int _myenv(data_d *data)
{
	print_list_str(data->env);
	return (0);
}

/**
 * _getenv - gets value of environ variable
 * @data: structure containing potential arguments. Used to maintain
 * @name: env var name
 * Return: the value
 */

char *_getenv(data_d *data, const char *name)
{
	list_t *node = data->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - initialize new environment variable,
 *             or modify existing one
 * @data: structure containing potential arguments. Used to maintain
 * Return: always 0
 */

int _mysetenv(data_d *data)
{
	if (data->argc != 3)
	{
		_eputs("incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(data, data->argv[1], data->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - remove environment variable
 * @data: structure containing potential arguments. Used to maintain
 * Return: always 0
 */

int _myunsetenv(data_d *data)
{
	int i;

	if (data->argc == 1)
	{
		_eputs("too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= data->argc; i++)
		_unsetenv(data, data->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @data: structure containing potential arguments. Used to maintain
 * Return: always 0
 */

int populate_env_list(data_d *data)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	data->env = node;
	return (0);
}
