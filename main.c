#include "ft_printf/ft_printf.h"
#include "minishell.h"
#include <argv.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#define burada ft_printf("burada \n");

int		argv_try(t_argv *argv, void *addr, size_t index, int (*fptr)(void *,
				void *));
int	ft_exit(t_argv *cmd)
{
	if (cmd->len == 1)
		exit(0);
	else if (cmd->len >= 2)
	{
		if (argv_is(cmd, 1, (int (*)(int))ft_isdigit) == 1)
		{
			if (cmd->len > 2)
			{
				ft_printf("exit: too many arguments\n");
				return (1);
			}
			else
				exit(ft_atoi(cmd->array[1]));
		}
		else
		{
			ft_printf("exit: %s: numeric argument required\n",
						(char *)cmd->array[1]);
			exit(2);
		}
	}
	return (-1);
}

int	ft_echo(t_argv *cmd)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	if (cmd->len == 1)
		ft_printf("\n");
	else
	{
		cmd->try_index = 0;
		while (argv_try(cmd, "-n", i, (int (*)(void *, void *))strcmp) == 0)
		{
			n = 1;
			i++;
		}
		while (i < cmd->len)
		{
			ft_printf("%s", cmd->array[i]);
			if (i < cmd->len - 1)
				ft_printf(" ");
			i++;
		}
		if (n == 0)
			ft_printf("\n");
	}
	return (0);
}

t_argv	*g_et;

int	ft_cd(t_argv *cmd) // env tan pathleri guncellemek lazim
{
	char *path;
	t_argv *env;
	env = g_et->array[0];

	if (cmd->len == 1)
	{
		env->try_index = 0;
		while (argv_try(env, "HOME=", 0, (int (*)(void *, void *))strcmp) == 0)
		{
			path = env->array[env->try_index];
			path = ft_strchr(path, '=');
			path++;
		}
		if (path == NULL)
			ft_printf("cd: HOME not set\n");
		else
			chdir(path);
	}
	else if (cmd->len == 2)
	{
		if (chdir(cmd->array[1]) == -1)
			ft_printf("cd: %s: No such file or directory\n", cmd->array[1]);
	}
	else
		ft_printf("cd: too many arguments\n");
	return (0);
}

int	ft_pwd(t_argv *cmd)
{
	char	*path;

	path = getcwd(NULL, 0);
	ft_printf("%s\n", path);
	free(path);
	return (0);
}

int	ft_env(t_argv *cmd)
{
	t_argv	*env;
	int		i;

	i = 0;
	env = g_et->array[0];
	while (i < env->len)
		ft_printf("%s\n", env->array[i++]);
	return (0);
}

int		jump_to_single_quote(char *line);

int	jump_to_double_quote(char *line)
{
	int	jump;
	int	res;

	jump = 1;
	while (line && *line && line[jump] != '\"')
	{
		if (line[jump] == '\0')
			return (-1);
		if (line[jump] == '\'')
		{
			res = jump_to_single_quote(&line[jump]);
			if (res != -1)
				jump += res;
		}
		jump++;
	}
	return (jump);
}
int	jump_to_single_quote(char *line)
{
	int	jump;
	int	res;

	jump = 1;
	while (line && *line && line[jump] != '\'')
	{
		if (line[jump] == '\0')
			return (-1);
		if (line[jump] == '\"')
		{
			res = jump_to_double_quote(&line[jump]);
			if (res != -1)
				jump += res;
		}
		jump++;
	}
	return (jump);
}

int	env_cmp(char *s1, char *s2)
{
	int		res;
	char	*tmp;
	char	*ret;

	res = ft_strncmp(s2, s1, ft_strlen(s2));
	return (res);
}

char	*get_env(char *str)
{
	t_argv	*env;
	char	*tmp;
	char	*ret;

	env = g_et->array[0];
	tmp = ft_strjoin(str + 1, "=");
	free(str);
	if (argv_try(env, tmp, 0, (int (*)(void *, void *))env_cmp) == 0)
	{
		ret = ft_substr(env->array[env->try_index], ft_strlen(tmp),
				ft_strlen(env->array[env->try_index]) - ft_strlen(tmp));
		free(tmp);
		return (ret);
	}
	else
	{
		free(tmp);
		return (ft_strdup(""));
	}
}

char	*implement(char *line)
{
	char	*tmp;
	int		i;
	int		len;

	i = 0;
	while (line[i])
	{
		if (line && line[i] == '$')
		{
			len = 0;
			while (!ft_strchr(" <>|", line[i + len]) && line[i + len])
				len++;
			tmp = str3join(ft_substr(line, 0, i), get_env(ft_substr(&line[i], 0,
							len)), ft_substr(&line[i + len], 0,
						ft_strlen(&line[i + len])));
		}
		i++;
	}
	return (tmp);
}

char	*clear_this(char *line, int c)
{
	int		i;
	t_argv	*env;
	char	*ret;
	char	*str;

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
		return (implement(ret));
	return (ft_strdup(ret));
}

char	*lexer_quote(char **string)
{
	int		jump;
	char	*tmp;
	char	*res;
	char	*ret;

	jump = 0;
	if (**string)
	{
		if (**string == '\"')
			jump = jump_to_double_quote(*string);
		else if (**string == '\'')
			jump = jump_to_single_quote(*string);
		if (jump < 0)
			return (NULL);
		tmp = *string;
		(*string) += jump + 1;
		res = ft_substr(tmp, 0, jump + 1);
		if (*res == '\"')
			ret = clear_this(res, '\"');
		else
			ret = clear_this(res, '\'');
		free(res);
		return (ret);
	};
	return (NULL);
}

char	*lexer_word_plus_quit(t_argv *cmd, char *line)
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
			rr = lexer_quote(&line);
			if (rr == NULL)
			{
				free(str);
				return (NULL);
			}
			tmp = ft_strjoin(str, rr);
			free(rr);
			free(str);
			str = tmp;
			continue ;
		}
		res = 0;
		while (!ft_strchr("\"\' |<>", *(line + res)))
			res++;
		tmp2 = ft_substr(line, 0, res);
		tmp = ft_strjoin(str, tmp2);
		free(str);
		free(tmp2);
		str = tmp;
		line += res;
		continue ;
	}
	argv_push(cmd, str);
	return (line);
}

int	is_token(char *line)
{
	if (ft_strchr("|<>", *line))
		return (1);
	return (0);
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

char	*lexer_token(t_argv *cmd, char *line)
{
	char	*str;

	if (is_token(line))
	{
		if (!ft_strncmp("<<", line, 2))
			str = ft_strdup("<<");
		else if (!ft_strncmp(">>", line, 2))
			str = ft_strdup(">>");
		else if (!ft_strncmp("|", line, 1))
			str = ft_strdup("|");
		else if (!ft_strncmp(">", line, 1))
			str = ft_strdup(">");
		else if (!ft_strncmp("<", line, 1))
			str = ft_strdup("<");
		argv_push(cmd, str);
		line += ft_strlen(str);
	}
	return (line);
}

char	*heredoc_op(char *line)
{
	char	*str;
	char	*read;
	int		save;

	save = dup(0);
	str = ft_strdup("");
	while (1)
	{
		read = readline("> ");
		if (!read)
			break ;
		if (!ft_strcmp(line, read))
			break ;
		str = str3join(str, read, ft_strdup("\n"));
	}
	if (read)
		free(read);
	dup2(save, 0);
	close(save);
	return (str);
}

int	lexer(t_argv *cmd, char *line)
{
	int		i;
	char	*tmp;

	i = 0;
	while (line && *line)
	{
		line = lexer_token(cmd, line);
		while (ft_isspace(*line))
			line++;
		line = lexer_word_plus_quit(cmd, line);
		if (line == NULL)
			return (-1);
		while (ft_isspace(*line))
			line++;
	}
	cmd->try_index = 0;
	while (argv_try(cmd, "<<", cmd->try_index, (int (*)(void *,
					void *))ft_strcmp) == 0)
	{
		tmp = heredoc_op(cmd->array[cmd->try_index + 1]);
		argv_del_one(cmd, cmd->try_index + 1, free);
		argv_insert(cmd, cmd->try_index + 1, tmp);
		cmd->try_index++;
	}
	return (0);
}

//print all cmd line from t_argv
void	print_cmd(t_argv *cmd)
{
	int	i;

	printf("\n\n\n");
	i = 0;
	while (cmd->array[i])
	{
		printf("%s\n", cmd->array[i]);
		i++;
	}
	printf("\n\n\n");
}


int builtin_tester(t_argv *cmd)
{
    if (!ft_strcmp(cmd->array[0], "echo"))
        return (ft_echo(cmd));
    if (!ft_strcmp(cmd->array[0], "cd"))
        return (ft_cd(cmd));
    if (!ft_strcmp(cmd->array[0], "pwd"))
        return (ft_pwd(cmd));
   // if (!ft_strcmp(cmd->array[0], "export"))
    //    return (ft_export(cmd));
    //if (!ft_strcmp(cmd->array[0], "unset"))
    //    return (ft_unset(cmd));
    if (!ft_strcmp(cmd->array[0], "env"))
        return (ft_env(cmd));
    if (!ft_strcmp(cmd->array[0], "exit"))
        return (ft_exit(cmd));
    return (0);
}


int	main(int argc, char **argv, char **envp)
{
	char	*line;
	int		status;
	int		i;
	t_argv	*cmd;
	t_argv	*env;

	;
	g_et = argv_new(NULL, NULL);
	argv_push(g_et, argv_new((void **)envp, (void *(*)(void *))ft_strdup));
	argv_push(g_et, argv_new(NULL, NULL));
	env = g_et->array[0];
	// Get_line >
	// lexer >
	// parser >
	// "' operation >
	// here doc operation >
	// make group for command >
	// pipe operation >
	// open $ >
	// fd operations >
	// execve | building (edited)
	status = 1;
	while (status)
	{
		line = readline("$> ");
		if (line == NULL)
			break ;
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		cmd = argv_new(NULL, NULL);
		add_history(line);
		lexer(cmd, line);
		free(line);
		builtin_tester(cmd);
		//print_cmd(cmd);
		argv_destroy(cmd, free);
		//system("leaks minishell");
	}
	return (0);
}
