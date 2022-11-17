/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocabas <akocabas@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 19:53:09 by akocabas          #+#    #+#             */
/*   Updated: 2022/11/17 20:03:39 by akocabas         ###   ########.fr       */
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

int	builtin_exec(t_argv *cmd)
{
	if (!ft_strcmp(cmd->array[0], "echo"))
		return (ft_echo(cmd));
	if (!ft_strcmp(cmd->array[0], "cd"))
		return (ft_cd(cmd));
	if (!ft_strcmp(cmd->array[0], "pwd"))
		return (ft_pwd(cmd));
	if (!ft_strcmp(cmd->array[0], "export"))
		return (ft_export(cmd));
	if (!ft_strcmp(cmd->array[0], "unset"))
		return (ft_unset(cmd));
	if (!ft_strcmp(cmd->array[0], "env"))
		return (ft_env(cmd));
	if (!ft_strcmp(cmd->array[0], "exit"))
		return (ft_exit(cmd));
	return (0);
}

int	is_builtin(t_argv *cmd)
{
	if (!ft_strcmp(cmd->array[0], "echo"))
		return (1);
	if (!ft_strcmp(cmd->array[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->array[0], "pwd"))
		return (1);
	if (!ft_strcmp(cmd->array[0], "export"))
		return (1);
	if (!ft_strcmp(cmd->array[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->array[0], "env"))
		return (1);
	if (!ft_strcmp(cmd->array[0], "exit"))
		return (1);
	return (0);
}

int	builtin_op(t_argv *cmd)
{
	int	save;
	int	out;
	int	status;

	out = dup(1);
	save = dup(0);
	status = exec_this(cmd);
	dup2(save, 0);
	dup2(out, 1);
	close(out);
	close(save);
	return (status);
}
