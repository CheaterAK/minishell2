#include "ft_printf/ft_printf.h"
#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

char *str3join(char *s1, char *s2, char *s3)
{
	char *tmp;
	char *str;

	tmp = ft_strjoin(s1, s2);
	str = ft_strjoin(tmp, s3);
	free (s1);
	free (s2);
	free (s3);
	free (tmp);
	return (str);
}

char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

int	ft_strcmp(const char *str1, const char *str2)
{
	int	i;

	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		++i;
	return ((unsigned char)str1[i] - (unsigned char)str2[i]);
}

int	ft_is_string(const char *line)
{
	int	i;

	i = 0;
	while (ft_isalnum(line[i]) || line[i] == '_')
		i++;
	if (line[i] == '\0')
		return (1);
	return (0);
}

int	ft_is_builtins(const char *str)
{
	if (ft_strcmp(str, "echo") == 0)
		return (1);
	if (ft_strcmp(str, "cd") == 0)
		return (1);
	if (ft_strcmp(str, "pwd") == 0)
		return (1);
	if (ft_strcmp(str, "export") == 0)
		return (1);
	if (ft_strcmp(str, "unset") == 0)
		return (1);
	if (ft_strcmp(str, "env") == 0)
		return (1);
	if (ft_strcmp(str, "exit") == 0)
		return (1);
	return (0);
}
