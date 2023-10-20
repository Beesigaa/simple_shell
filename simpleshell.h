#ifndef _SIMPLESHELL_H_
#define _SIMPLESHELL_H_

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
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* for convert_number() */
#define LOWERCASE_CONVERTER 1
#define UNSIGNED_CONVERTER 2

/* 1 if using system getline() */
#define _GETLINE 0
#define _STRTOK 0

#define FILE_HIST	".simple_shell_history"
#define MAX_HIST	4096

extern char **envir;


/**
 * struct str_list - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct str_list
{
	int num;
	char *str;
	struct str_list *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@count_line: the error count
 *@err_num: the error code for exit()s
 *@flag_linecount: if on count this line of input
 *@f_name: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_chang: on if environ was changed
 *@stat_: the return status of the last exec'd command
 *@buf_cmd: address of pointer to cmd_buf, on if chaining
 *@buf_type_cmd: CMD_type ||, &&, ;
 *@readfd_: the fd from which to read line input
 *@hist_count: the history line number count
 */
typedef struct info_pass
{
	char *ar;
	char **arg_v;
	char *pa;
	int argc;
	unsigned int count_line;
	int num_err;
	int flag_linecount;
	char *f_name;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **envir;
	int env_chang;
	int stat_;

	char **buf_cmd; /* pointer ; chain buffer, memory mangement */
	int buf_type_cmd; /* CMD_type ||, &&, ; */
	int readfd_;
	int hist_count;
} terminfo;

#define INF_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct built_in - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct built_in
{
	char *type;
	int (*func)(terminfo *);
} builtin_table;


/* simpleshell_loop.c */
int hsh(terminfo *, char **);
int find_builtin(terminfo *);
void find_cmd(terminfo *);
void fork_cmd(terminfo *);

/* simpleshell_parser.c */
int is_cmd(terminfo *, char *);
char *dup_chars(char *, int, int);
char *find_path(terminfo *, char *, char *);

/* loop_ashell.c */
int loop_ashell(char **);

/* simpleshell_errors.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

/* simpleshell_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* simpleshell_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* simpleshell_exits.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* simpleshell_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* simpleshell_realloc.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* simpleshell_memory.c */
int bfree(void **);

/* simpleshell_atoi.c */
int interactive(terminfo *);
int is_delim(char, char *);
int _isalpha(int);
int _atoi(char *);

/* simpleshell_errors1.c */
int _erratoi(char *);
void print_error(terminfo *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comments(char *);

/* simpleshell_builtin.c */
int _myexit(terminfo *);
int _mycd(terminfo *);
int _myhelp(terminfo *);

/* simpleshell_builtin1.c */
int _myhistory(terminfo *);
int _myalias(terminfo *);

/*simpleshell_getline.c */
ssize_t get_input(terminfo *);
int _getline(terminfo *, char **, size_t *);
void sigintHandler(int);

/* simpleshell_getinfo.c */
void clear_info(terminfo *);
void set_info(terminfo *, char **);
void free_info(terminfo *, int);

/* simpleshell_environ.c */
char *_getenv(terminfo *, const char *);
int _myenv(terminfo *);
int _mysetenv(terminfo *);
int _myunsetenv(terminfo *);
int populate_env_list(terminfo *);

/* simpleshell_getenv.c */
char **get_environ(terminfo *);
int _unsetenv(terminfo *, char *);
int _setenv(terminfo *, char *, char *);

/* simpleshell_history.c */
char *get_history_file(terminfo *info);
int write_history(terminfo *info);
int read_history(terminfo *info);
int build_history_list(terminfo *info, char *buf, int linecount);
int renumber_history(terminfo *info);

/* simpleshell_lists.c */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* simpleshell_lists1.c */
size_t _lenlist(const list_t *);
char **list_to_strings(list_t *);
size_t prnt_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* simpleshell_vars.c */
int _ischain(terminfo *, char *, size_t *);
void _checkchain(terminfo *, char *, size_t *, size_t, size_t);
int rep_al(terminfo *);
int rep_vars(terminfo *);
int rep_str(char **, char *);

#endif
