#include "shell.h"

/**
 * input_buff - buffers chained commands
 * @data: parameter struct
 * @buff: address of buffer
 * @len: address of len var
 * Return: bytes read
 */

ssize_t input_buff(data_d *data, char **buff, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buff);*/
		free(*buff);
		*buff = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buff, &len_p, stdin);
#else
		r = _getline(data, buff, &len_p);
#endif
		if (r > 0)
		{
			if ((*buff)[r - 1] == '\n')
			{
				(*buff)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			data->linecount_flag = 1;
			remove_comments(*buff);
			build_hist_list(data, *buff, data->histcount++);
			/* if (_strchr(*buff, ';')) is this a command chain? */
			{
				*len = r;
				data->cmd_buff = buff;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets line minus newline
 * @data: parameter struct
 * Return: bytes read
 */

ssize_t get_input(data_d *data)
{
	static char *buff; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buff_p = &(data->arg), *p;

	_putchar(BUFF_FLUSH);
	r = input_buff(data, &buff, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buff position */
		p = buff + i; /* get pointer for return */

		check_chain(data, buff, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(data, buff, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			data->cmd_buff_type = CMD_NORM;
		}

		*buff_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*buff_p = buff; /* else not a chain, pass back buffer from _getline() */
	return (r); /* return length of buffer from _getline() */
}

/**
 * read_buff - reads buffer
 * @data: parameter struct
 * @buff: buffer
 * @s: size
 * Return: r
 */

ssize_t read_buff(data_d *data, char *buff, size_t *s)
{
	ssize_t r = 0;

	if (*s)
		return (0);
	r = read(data->readfd, buff, READ_BUFF_SIZE);
	if (r >= 0)
		*s = r;
	return (r);
}

/**
 * _getline - gets next line of input from STDIN
 * @data: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 * Return: s
 */

int _getline(data_d *data, char **ptr, size_t *length)
{
	static char buff[READ_BUFF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buff(data, buff, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buff + i, '\n');
	k = c ? 1 + (unsigned int)(c - buff) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buff + i, k - i);
	else
		_strncpy(new_p, buff + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: signal number
 * Return: void
 */

void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUFF_FLUSH);
}
