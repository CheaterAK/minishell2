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
char	*implement(char *line_s, int status)
{
	char	*tmp;
	int		i;
	int		len;
	char	*line;

	line = ft_strdup(line_s);
	i = 0;
	while (line[i])
	{
		if (line && line[i] == '$')
		{
			if (line[i + 1] == '$')
			{
				tmp = str3join(ft_substr(line, 0, i), ft_itoa(getpid()),
						ft_strdup(line + i + 2));
				free(line);
				line = tmp;
				continue ;
			}
			else if (!line[i + 1])
			{
				tmp = str3join(ft_substr(line, 0, i), ft_strdup("$"),
						ft_strdup(""));
				free(line);
				return (tmp);
			}
			else if (line[i + 1] == '?')
			{
				tmp = str3join(ft_substr(line, 0, i), ft_itoa(status),
						ft_strdup(line + i + 2));
				free(line);
				line = tmp;
				continue ;
			}
			len = 1;
			while (!ft_strchr(" <>|$", line[i + len]) && line[i + len])
				len++;
			tmp = str3join(ft_substr(line, 0, i), get_env(ft_substr(&line[i], 0,
							len)), ft_substr(&line[i + len], 0,
						ft_strlen(&line[i + len])));
			free(line);
			line = tmp;
		}
		i++;
	}
	return (line);
}
