/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocabas <akocabas@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:32:26 by akocabas          #+#    #+#             */
/*   Updated: 2022/11/17 20:12:42 by akocabas         ###   ########.fr       */
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

int		argv_try(t_argv *argv, void *addr, size_t index, int (*fptr)(void *,
				void *));

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

char	*check_token(t_argv *cmd)
{
	size_t		i;
	char		*tmp;
	char		*tmp2;

	i = 0;
	while (i < cmd->len)
	{
		tmp = (char *)cmd->array[i];
		if (ft_strchr("<>|", tmp[0]) && *tmp)
		{
			tmp2 = cmd->array[++i];
			if (!tmp2 || *tmp2 == '\0')
			{
				if (cmd->array[i + 1] == NULL || cmd->len < i + 1)
					return (ft_strdup("newline"));
				tmp = cmd->array[++i];
				if (ft_strchr("<>|", tmp[0]) || !tmp)
					return (ft_strdup(tmp));
				else if (!tmp || *tmp == '\0')
					return (ft_strdup("newline"));
			}
		}
		i++;
	}
	return (NULL);
}

void	*signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ioctl(0, TIOCSTI, "$> ");
	}
	else if (sig == SIGQUIT)
	{
		ioctl(0, TIOCSTI, "$> ");
	}
	return (0);
}

int	check_line(char *line)
{
	while (line && *line)
	{
		if (ft_isspace(*line))
			line++;
		else
			return (0);
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	int		status;
	char	*str;
	t_argv	*cmd;
	t_argv	*env;

	g_et = argv_new(NULL, NULL);
	argv_push(g_et, argv_new((void **)envp, (void *(*)(void *))ft_strdup));
	argv_push(g_et, argv_new(NULL, NULL));
	env = g_et->array[0];
	status = 0;
	while (1)
	{
		signal(SIGINT, (void (*)(int))signal_handler);
		signal(SIGQUIT, SIG_IGN);
		line = readline("$> ");
		if (line == NULL)
			break ;
		if (line[0] == '\0' || check_line(line))
		{
			free(line);
			continue ;
		}
		cmd = argv_new(NULL, NULL);
		add_history(line);
		lexer(cmd, line, status);
		free(line);
		str = check_token(cmd);
		if (str || heredoc_check(cmd))
		{
			printf("minishell: syntax error near unexpected token `%s'\n", str);
			status = 258;
		}
		else if (argv_try(cmd, "|", 0, (int (*)(void *, void *))ft_strcmp) != 0
				&& is_builtin(cmd))
			status = builtin_op(cmd);
		else
			status = exec_all(cmd, find_procces_size(cmd));
		if (str)
			free(str);
		argv_destroy(cmd, free);
	}
	return (0);
}
