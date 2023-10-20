#include "simpleshell.h"
/**
 * _myhistory - history list, one command by line, preceded
 *              with line numbers, starting at zero.
 * @inf: Structure containing potential arguments. Used to maintain
 *       constant function prototype.
 *  Return: Always 0
 */
int _myhistory(terminfo *inf)
{
	prnt_list(inf->history);
	return (0);
}

/**
 * unset_alias - sets alias to string
 * @inf: parameter struct
 * @str: the string alias
 * Return: Always 0 on success
 */
int unset_alias(terminfo *inf, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(inf->alias),
		get_node_index(inf->alias, node_starts_with(inf->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(terminfo *info, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
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
 * _myalias - mimics the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int _myalias(terminfo *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->arg_v[i]; i++)
	{
		p = _strchr(info->arg_v[i], '=');
		if (p)
			set_alias(info, info->arg_v[i]);
		else
			print_alias(node_starts_with(info->alias, info->arg_v[i], '='));
	}

	return (0);
}
