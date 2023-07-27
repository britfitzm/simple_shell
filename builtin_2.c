#include "shell.h"

/**
 * _myhist - displays history list, one command by line, preceded
 *              with line numbers, starting at 0
 * @data: structure containing potential arguments. Used to maintain
 * Return: always 0
 */

int _myhist(data_d *data)
{
	print_list(data->hist);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @data: parameter struct
 * @str: string alias
 * Return: always 0 on success, 1 on error
 */

int unset_alias(data_d *data, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(data->alias),
		get_node_index(data->alias, node_starts_with(data->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @data: parameter struct
 * @str: string alias
 * Return: always 0 on success, 1 on error
 */

int set_alias(data_d *data, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(data, str));

	unset_alias(data, str);
	return (add_node_end(&(data->alias), str, 0) == NULL);
}

/**
 * print_alias - prints alias string
 * @node: alias node
 * Return: always 0 on success, 1 on error
 */

int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
		_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - mimics alias builtin (man alias)
 * @data: structure containing potential arguments. Used to maintain
 *  Return: always 0
 */

int _myalias(data_d *data)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (data->argc == 1)
	{
		node = data->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; data->argv[i]; i++)
	{
		p = _strchr(data->argv[i], '=');
		if (p)
			set_alias(data, data->argv[i]);
		else
			print_alias(node_starts_with(data->alias, data->argv[i], '='));
	}

	return (0);
}
