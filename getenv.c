#include "shell.h"

/**
 * get_environ - returns string array copy of our environ
 * @data: structure containing potential arguments. Used to maintain
 * Return: always 0
 */

char **get_environ(data_d *data)
{
	if (!data->environ || data->env_changed)
	{
		data->environ = list_to_strings(data->env);
		data->env_changed = 0;
	}

	return (data->environ);
}

/**
 * _unsetenv - remove environment variable
 * @data: structure containing potential arguments. Used to maintain
 * @var: the string env var property
 * Return: 1 on delete, 0 otherwise
 */

int _unsetenv(data_d *data, char *var)
{
	list_t *node = data->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			data->env_changed = delete_node_at_index(&(data->env), i);
			i = 0;
			node = data->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (data->env_changed);
}

/**
 * _setenv - initialize new environment variable,
 *             or modify existing one
 * @data: structure containing potential arguments. Used to maintain
 * @var: string env var property
 * @value: string env var value
 * Return: always 0
 */

int _setenv(data_d *data, char *var, char *value)
{
	char *buff = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buff = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buff)
		return (1);
	_strcpy(buff, var);
	_strcat(buff, "=");
	_strcat(buff, value);
	node = data->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buff;
			data->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(data->env), buff, 0);
	free(buff);
	data->env_changed = 1;
	return (0);
}
