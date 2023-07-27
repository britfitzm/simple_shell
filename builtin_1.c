#include "shell.h"

/**
 * _myexit - exits the shell
 * @data: structure containing potential arguments. Used to maintain
 * Return: exits with a given exit status
 * (0) if info.argv[0] != "exit"
 */

int _myexit(data_d *data)
{
	int exitcheck;

	if (data->argv[1]) /* if there is an exit arguement */
	{
		exitcheck = _erratoi(data->argv[1]);
		if (exitcheck == -1)
		{
			data->status = 2;
			print_err(data, "illegal number: ");
			_eputs(data->argv[1]);
			_eputchar('\n');
			return (1);
		}
		data->err_num = _erratoi(data->argv[1]);
		return (-2);
	}
	data->err_num = -1;
	return (-2);
}

/**
 * _mycd - changes current directory of process
 * @data: structure containing potential arguments. Used to maintain
 * Return: always 0
 */

int _mycd(data_d *data)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("sh: 0: getcwd failed: no such file or directory\n");
	if (!data->argv[1])
	{
		dir = _getenv(data, "HOME=");
		if (!dir)
			chdir_ret = /* todo: what should this be? */
				chdir((dir = _getenv(data, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(data->argv[1], "-") == 0)
	{
		if (!_getenv(data, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(data, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* todo: what should this be? */
			chdir((dir = _getenv(data, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(data->argv[1]);
	if (chdir_ret == -1)
	{
		print_err(data, "can't cd to ");
		_eputs(data->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(data, "OLDPWD", _getenv(data, "PWD="));
		_setenv(data, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp - changes current directory of process
 * @data: structure containing potential arguments. Used to maintain
 * Return: always 0
 */

int _myhelp(data_d *data)
{
	char **arg_array;

	arg_array = data->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
