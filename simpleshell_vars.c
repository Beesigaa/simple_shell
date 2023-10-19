#include "simpleshell.h"
/**
 * _ischain - test if current char in buffer is a chain delimeter
 * @inf: the parameter struct
 * @buf: the char buffer
 * @p_add: address of current position in buf
 * Return: 1 if chain delimeter, 0 otherwise
 */
int _ischain(terminfo *inf, char *buf, size_t *p_add)
{
	size_t j = *p_add;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		inf->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		inf->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';')
	{
		buf[j] = NULL;
		inf->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p_add = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * @inf: the parameter struct
 * @buf: the char buffer
 * @p_add: address of current position in buf
 * @s: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void _checkchain(terminfo *inf, char *buf, size_t *p_add, size_t s, size_t len)
{
	size_t j = *p_add;

	if (inf->cmd_buf_type == CMD_AND)
	{
		if (inf->stat_)
		{
			buf[s] = 0;
			j = len;
		}
	}
	if (inf->buf_type_cmd == CMD_OR)
	{
		if (!inf->stat_)
		{
			buf[s] = 0;
			j = len;
		}
	}

	*p_add = j;
}

/**
 * rep_al - replaces an aliases in the tokenized string
 * @inf: the parameter struct
 * Return: 1 if replaced, 0 otherwise
 */
int rep_al(terminfo *inf)
{
	int n;
	list_t *node;
	char *p_add;

	for (n = 0; n < 10; n++)
	{
		node = node_starts_with(inf->alias, inf->arg_v[0], '=');
		if (!node)
			return (0);
		free(inf->arg_v[0]);
		p_add = _strchr(node->str, '=');
		if (!p_add)
			return (0);
		p_add = _strdup(p_add + 1);
		if (!p_add)
			return (0);
		inf->arg_v[0] = p_add;
	}
	return (1);
}

/**
 * rep_vars - replaces vars in the tokenized string
 * @inf: the parameter struct
 * Return: 1 if replaced, 0 otherwise
 */
int rep_vars(terminfo *inf)
{
	int n = 0;
	list_t *node;

	for (n = 0; inf->arg_v[n]; n++)
	{
		if (inf->arg_v[n][0] != '$' || !inf->arg_v[n][1])
			continue;

		if (!_strcmp(inf->arg_v[n], "$?"))
		{
			rep_str(&(inf->arg_v[n]),
				_strdup(convert_number(inf->stat_, 10, 0)));
			continue;
		}
		if (!_strcmp(inf->arg_v[n], "$$"))
		{
			rep_str(&(inf->arg_v[n]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(inf->env, &inf->arg_v[n][1], '=');
		if (node)
		{
			rep_str(&(inf->arg_v[n]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		rep_str(&inf->arg_v[n], _strdup(""));
	}
	return (0);
}

/**
 * rep_str - replaces string
 * @old_str: address of old string
 * @new_str: new string
 * Return: 1 if replaced, 0 otherwise
 */
int rep_str(char **old_str, char *new_str)
{
	free(*old_str);
	*old_str = new_str;
	return (1);
}
