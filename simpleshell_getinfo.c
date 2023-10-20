#include "simpleshell.h"
/**
 * clear_info - initializes terminfo struct
 * @info: struct address
 */
void clear_info(terminfo *info)
{
	info->ar = NULL;
	info->arg_v = NULL;
	info->pa = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes terminfo struct
 * @info: struct address
 * @av: argument vector
 */
void set_info(terminfo *info, char **av)
{
	int i = 0;

	info->f_name = av[0];
	if (info->ar)
	{
		info->arg_v = strtow(info->ar, " \t");
		if (!info->arg_v)
		{

			info->arg_v = malloc(sizeof(char *) * 2);
			if (info->arg_v)
			{
				info->arg_v[0] = _strdup(info->ar);
				info->arg_v[1] = NULL;
			}
		}
		for (i = 0; info->arg_v && info->arg_v[i]; i++)
			;
		info->argc = i;

		rep_al(info);
		rep_vars(info);
	}
}

/**
 * free_info - frees terminfo struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_info(terminfo *info, int all)
{
	ffree(info->arg_v);
	info->arg_v = NULL;
	info->pa = NULL;
	if (all)
	{
		if (!info->buf_cmd)
			free(info->ar);
		if (info->env)
			free_list(&(info->env));
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		ffree(info->envir);
			info->envir = NULL;
		bfree((void **)info->buf_cmd);
		if (info->readfd_ > 2)
			close(info->readfd_);
		_putchar(BUF_FLUSH);
	}
}
