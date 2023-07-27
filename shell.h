#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUFF_SIZE 1024
#define WRITE_BUFF_SIZE 1024
#define BUFF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: number field
 * @str: string
 * @next: points to next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passdata - contains pseudo-arguements to pass into function,
 * allowing uniform prototype for function pointer struct
 * @arg: string generated from getline containing arguements
 * @argv: array of strings generated from arg
 * @path: string path for current command
 * @argc: argument count
 * @line_count: error count
 * @err_num: error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @hist: history node
 * @alias: alias node
 * @env_changed: on if environ was changed
 * @status: return status of last exec'd command
 * @cmd_buff: address of pointer to cmd_buff, on if chaining
 * @cmd_buff_type: CMD_type ||, &&, ;
 * @readfd: fd from which to read line input
 * @histcount: history line number count
 */

typedef struct passdata
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *hist;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buff; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buff_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} data_d;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
		0, 0, 0}

/**
 * struct builtin - contains builtin string and related function
 * @type: builtin command flag
 * @func: function
 */

typedef struct builtin
{
	char *type;
	int (*func)(data_d *data);
} builtin_table;

/* shell.c */
int hsh(data_d *data, char **av);
int find_builtin(data_d *data);
void find_cmd(data_d *data);
void fork_cmd(data_d *data);

/* parser.c */
int is_cmd(data_d *data, char *path);
char *dup_chars(char *pathstr, int start, int stop);
char *find_path(data_d *data, char *pathstr, char *cmd);

/* loophsh.c */
int loophsh(char **);

/* errors_1.c */
void _eputs(char *str);
int _eputchar(char c);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* string_1.c */
int _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *starts_with(const char *string, const char *character);
char *_strcat(char *dest, char *src);

/* string_2.c */
char *_strcpy(char *dest, char *src);
char *_strdup(const char *str);
void _puts(char *str);
int _putchar(char c);

/* exit.c */
char *_strncpy(char *dest, char *src, int n);
char *_strncat(char *dest, char *src, int n);
char *_strchr(char *s, char c);

/* tokenize.c */
char **strtow(char *str, char *d);
char **strtow2(char *str, char d);

/* realloc.c */
char *_memset(char *s, char b, unsigned int n);
void _free(char **ss);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/* mem_code.c */
int bfree(void **ptr);

/* _atoi.c */
int interactive(data_d *data);
int is_delim(char c, char *delim);
int _isalpha(int c);
int _atoi(char *s);

/* errors_2.c */
int _erratoi(char *s);
void print_err(data_d *data, char *estr);
int print_d(int input, int fd);
char *convert_num(long int num, int base, int flags);
void remove_comments(char *buff);

/* builtin_1.c */
int _myexit(data_d *data);
int _mycd(data_d *data);
int _myhelp(data_d *data);

/* builtin_2.c */
int _myhist(data_d *data);
int _myalias(data_d *data);
int unset_alias(data_d *data, char *str);
int set_alias(data_d *data, char *str);
int print_alias(list_t *node);

/* getline.c */
ssize_t get_input(data_d *data);
int _getline(data_d *data, char **ptr, size_t *length);
void sigintHandler(__attribute__((unused))int sig_num);
ssize_t read_buff(data_d *data, char *buff, size_t *s);
ssize_t input_buff(data_d *data, char **buff, size_t *len);

/* getinfo.c */
void clear_info(data_d *data);
void set_info(data_d *data, char **av);
void free_info(data_d *data, int all);

/* env.c */
char *_getenv(data_d *data, const char *name);
int _myenv(data_d *data);
int _mysetenv(data_d *data);
int _myunsetenv(data_d *data);
int populate_env_list(data_d *data);

/* getenv.c */
char **get_environ(data_d *data);
int _unsetenv(data_d *data, char *var);
int _setenv(data_d *data, char *var, char *value);

/* history.c */
char *get_hist_file(data_d *data);
int write_hist(data_d *data);
int read_hist(data_d *data);
int build_hist_list(data_d *data, char *buff, int linecount);
int renum_hist(data_d *data);

/* list_1.c */
list_t *add_node(list_t **head, const char *str, int num);
list_t *add_node_end(list_t **head, const char *str, int num);
size_t print_list_str(const list_t *s);
int delete_node_at_index(list_t **head, unsigned int index);
void free_list(list_t **head_ptr);

/* list_2.c */
size_t list_len(const list_t *h);
char **list_to_strings(list_t *head);
size_t print_list(const list_t *s);
list_t *node_starts_with(list_t *node, char *prefix, char c);
ssize_t get_node_index(list_t *head, list_t *node);

/* variables.c */
int is_chain(data_d *data, char *buff, size_t *pb);
void check_chain(data_d *data, char *buff, size_t *pb, size_t sb, size_t len);
int replace_alias(data_d *data);
int replace_vars(data_d *data);
int replace_string(char **old, char *new);

#endif
