/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queto.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocabas <akocabas@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 20:08:48 by akocabas          #+#    #+#             */
/*   Updated: 2022/11/18 03:02:22 by akocabas         ###   ########.fr       */
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

// int		jump_to_single_quote(char *line);

// int	jump_to_double_quote(char *line)
// {
// 	int	jump;
// 	int	res;

// 	jump = 1;
// 	while (line && *line && line[jump] != '\"')
// 	{
// 		if (line[jump] == '\0')
// 			return (-1);
// 		if (line[jump] == '\'')
// 		{
// 			res = jump_to_single_quote(&line[jump]);
// 			if (res != -1)
// 				jump += res;
// 		}
// 		jump++;
// 	}
// 	return (jump);
// }

// int	jump_to_single_quote(char *line)
// {
// 	int	jump;
// 	int	res;

// 	jump = 1;
// 	while (line && *line && line[jump] != '\'')
// 	{
// 		if (line[jump] == '\0')
// 			return (-1);
// 		if (line[jump] == '\"')
// 		{
// 			res = jump_to_double_quote(&line[jump]);
// 			if (res != -1)
// 				jump += res;
// 		}
// 		jump++;
// 	}
// 	return (jump);
// }

// char	*clear_this(char *line, int c, int status)
// {
// 	int		i;
// 	t_argv	*env;
// 	char	*ret;

// 	ret = line;
// 	env = g_et->array[0];
// 	while (*line)
// 	{
// 		if (*line == (char)c)
// 		{
// 			i = 0;
// 			while (line[i])
// 			{
// 				line[i] = line[i + 1];
// 				i++;
// 			}
// 		}
// 		else
// 			line++;
// 	}
// 	if (c == '\"')
// 		return (implement(ret, status));
// 	return (ft_strdup(ret));
// }

// char	*lexer_quote(char **string, int status)
// {
// 	int		jump;
// 	char	*tmp;
// 	char	*res;
// 	char	*ret;

// 	jump = 0;
// 	if (**string)
// 	{
// 		if (**string == '\"')
// 			jump = jump_to_double_quote(*string);
// 		else if (**string == '\'')
// 			jump = jump_to_single_quote(*string);
// 		if (jump == NULL)
// 			return (NULL);
// 		tmp = *string;
// 		(*string) += jump + 1;
// 		res = ft_substr(tmp, 0, jump + 1);
// 		if (*res == '\"')
// 			ret = clear_this(res, '\"', status);
// 		else
// 			ret = clear_this(res, '\'', status);
// 		free(res);
// 		return (ret);
// 	}
// 	return (NULL);
// }

char	*clear_this(char *line, int c, int status)
{
	int		i;
	t_argv	*env;
	char	*ret;

	ret = line;
	env = g_et->array[0];
	while (*line)
	{
		if (*line == (char)c)
		{
			i = 0;
			while (line[i])
			{
				line[i] = line[i + 1];
				i++;
			}
		}
		else
			line++;
	}
	if (c == '\"')
		return (implement(ret, status));
	return (ft_strdup(ret));
}

char *jump_to_double_quote(char *line)
{
	char* len;
	len = ft_strchr(line + 1, '\"');
		if (len == 0)
			return (NULL);
		else
			len = len - line + (char *)1;
	return(ft_substr(line, 0, (size_t)len));
}

char *jump_to_single_quote(char *line)
{
	char* len;
	len = ft_strchr(line + 1, '\'');
		if (len == 0)
			return (0);
		else
			len = len - line + (char *)1;
	return(ft_substr(line, 0, (size_t)len));
}

char	*lexer_quote(char **string, int status)
{
	int		jump;
	char	*tmp;
	char	*res;
	char	*ret;

	if (**string)
	{
		if (**string == '\"')
			tmp = jump_to_double_quote(*string);
		else if (**string == '\'')
			tmp = jump_to_single_quote(*string);
		if (tmp == NULL)
			return (NULL);
		res = tmp;
		(*string) += ft_strlen(tmp);
		if (*res == '\"')
			ret = clear_this(res, '\"', status);
		else
			ret = clear_this(res, '\'', status);
		free(res);
		return (ret);
	}
	return (NULL);
}

char	*lexer_word_plus_quit(t_argv *cmd, char *line, int status)
{
	int		res;
	char	*rr;
	char	*str;
	char	*tmp;
	char	*tmp2;

	str = ft_strdup("");
	while (!ft_strchr(" |<>", *line))
	{
		if (ft_strchr("\"\'", *line))
		{
			rr = lexer_quote(&line, status);
			if (rr == NULL)
			{
				free(str);
				return (NULL);
			}
			tmp = ft_strjoin(str, rr);
			free(rr);
			free(str);
			str = tmp;
		}
		res = 0;
		while (!ft_strchr("\"\' |<>", *(line + res)))
			res++;
		tmp = ft_substr(line, 0, res);
		tmp2 = implement(tmp, status);
		free(tmp);
		tmp = ft_strjoin(str, tmp2);
		free(str);
		free(tmp2);
		str = tmp;
		line += res;
	}
	argv_push(cmd, str);
	return (line);
}
