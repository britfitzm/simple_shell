#include "shell.h"

/**
 * is_chain - test if current char in buffer is chain delimeter
 * @data: parameter struct
 * @buff: char buffer
 * @pb: address of current position in buff
 * Return: 1 if chain delimeter, 0 otherwise
 */

int is_chain(data_d *data, char *buff, size_t *pb)
{
	size_t j = *pb;

	if (buff[j] == '|' && buff[j + 1] == '|')
	{
		buff[j] = 0;
		j++;
		data->cmd_buff_type = CMD_OR;
	}
	else if (buff[j] == '&' && buff[j + 1] == '&')
	{
		buff[j] = 0;
		j++;
		data->cmd_buff_type = CMD_AND;
	}
	else if (buff[j] == ';') /* found end of this command */
	{
		buff[j] = 0; /* replace semicolon with null */
		data->cmd_buff_type = CMD_CHAIN;
	}
	else
		return (0);
	*pb = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @data: parameter struct
 * @buff: char buffer
 * @pb: address of current position in buff
 * @sb: starting position in buff
 * @len: length of buff
 * Return: void
 */

void check_chain(data_d *data, char *buff, size_t *pb, size_t sb, size_t len)
{
	size_t j = *pb;

	if (data->cmd_buff_type == CMD_AND)
	{
		if (data->status)
		{
			buff[sb] = 0;
			j = len;
		}
	}
	if (data->cmd_buff_type == CMD_OR)
	{
		if (!data->status)
		{
			buff[sb] = 0;
			j = len;
		}
	}

	*pb = j;
}

/**
 * replace_alias - replaces aliases in tokenized string
 * @data: parameter struct
 * Return: 1 if replaced, 0 otherwise
 */

int replace_alias(data_d *data)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(data->alias, data->argv[0], '=');
		if (!node)
			return (0);
		free(data->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		data->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in tokenized string
 * @data: parameter struct
 * Return: 1 if replaced, 0 otherwise
 */

int replace_vars(data_d *data)
{
	int i = 0;
	list_t *node;

	for (i = 0; data->argv[i]; i++)
	{
		if (data->argv[i][0] != '$' || !data->argv[i][1])
			continue;

		if (!_strcmp(data->argv[i], "$?"))
		{
			replace_string(&(data->argv[i]),
					_strdup(convert_num(data->status, 10, 0)));
			continue;
		}
		if (!_strcmp(data->argv[i], "$$"))
		{
			replace_string(&(data->argv[i]),
					_strdup(convert_num(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(data->env, &data->argv[i][1], '=');
		if (node)
		{
			replace_string(&(data->argv[i]),
					_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&data->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - replaces string
 * @old: address of old string
 * @new: new string
 * Return: 1 if replaced, 0 otherwise
 */

int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
