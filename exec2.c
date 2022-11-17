/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocabas <akocabas@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 16:14:33 by akocabas          #+#    #+#             */
/*   Updated: 2022/11/17 16:15:20 by akocabas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf/ft_printf.h"
#include "minishell.h"
#include <argv.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

t_argv	*get_trgt(t_argv *exec)
{
	t_argv	*trgt;

	if (-1 == argv_try(exec, "|", 0, (int (*)(void *, void *))ft_strcmp))
		trgt = argv_splice(exec, 0, exec->len);
	else
	{
		trgt = argv_splice(exec, 0, exec->try_index);
		argv_del_one(exec, 0, (void (*)(void *))free);
	}
	return (trgt);
}

int	try_access(char *path, char *cmd)
{
	char	*str;
	int		st;

	str = str3join(ft_strdup(path), ft_strdup("/"), ft_strdup(cmd));
	st = access(str, X_OK);
	free(str);
	return (st);
}

char	*accesable(char *path, t_argv *cmd)
{
	if (argv_try(path, cmd, 0, (int (*)(void *, void *))try_access) == 0)
	{
		ret = str3join(ft_strdup(path->array[path->try_index]), ft_strdup("/"),
				ft_strdup(cmd));
		argv_destroy(path, (void (*)(void *))free);
		return (ret);
	}
	else
	{
		argv_destroy(path, (void (*)(void *))free);
		return (NULL);
	}
}
