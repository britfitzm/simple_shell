#include "shell.h"

/**
 * _eputs - prints input string
 * @str: string to be printed
 * Return: nothing
 */

void _eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar - writes character c to stderr
 * @c: character to print
 * Return: on success 1.
 * On error, -1 is returned, and errno is set appropriately
 */

int _eputchar(char c)
{
	static int i;
	static char buff[WRITE_BUFF_SIZE];

	if (c == BUFF_FLUSH || i >= WRITE_BUFF_SIZE)
	{
		write(2, buff, i);
		i = 0;
	}
	if (c != BUFF_FLUSH)
		buff[i++] = c;
	return (1);
}

/**
 * _putfd - writes character c to given fd
 * @c: character to print
 * @fd: filedescriptor to write to
 * Return: on success 1.
 * On error, -1 is returned, and errno is set appropriately
 */

int _putfd(char c, int fd)
{
	static int i;
	static char buff[WRITE_BUFF_SIZE];

	if (c == BUFF_FLUSH || i >= WRITE_BUFF_SIZE)
	{
		write(fd, buff, i);
		i = 0;
	}
	if (c != BUFF_FLUSH)
		buff[i++] = c;
	return (1);
}

/**
 * _putsfd - prints input string
 * @str: string to be printed
 * @fd: filedescriptor to write to
 * Return: number of chars put
 */

int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putfd(*str++, fd);
	}
	return (i);
}
