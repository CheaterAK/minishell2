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
#define burada ft_printf("burada \n");

t_argv	*g_et;
int		argv_try(t_argv *argv, void *addr, size_t index, int (*fptr)(void *,
				void *));

int	env_cmp(char *s1, char *s2)
{
	int		res;
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

char	**clone(t_argv *env, t_argv *exp)
{
	char	**tmparr;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmparr = (char **)malloc(sizeof(char *) * (env->len + exp->len + 1));
	while (i < env->len)
	{
		tmparr[i] = (char *)ft_strdup(env->array[i]);
		i++;
	}
	while (j < exp->len)
	{
		tmparr[i] = (char *)ft_strdup(exp->array[j]);
		ft_printf("\n%s\n", tmparr[i]);
		i++;
		j++;
	}
	tmparr[i] = NULL;
	return (tmparr);
}

void	sort_array(char **tmparr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (tmparr[i])
	{
		j = i + 1;
		while (tmparr[j])
		{
			if (ft_strcmp(tmparr[i], tmparr[j]) > 0)
			{
				tmp = tmparr[i];
				tmparr[i] = tmparr[j];
				tmparr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	set_sorted_tmp(t_argv **tmp, t_argv *env, t_argv *exp)
{
	char	**tmparr;
	int		i;

	i = 0;
	tmparr = clone(env, exp);
	sort_array(tmparr);
	*tmp = argv_new((void **)tmparr, (void *(*)(void *))ft_strdup);
	i = 0;
	while (tmparr[i])
	{
		free(tmparr[i]);
		i++;
	}
	free(tmparr);
}

void	env_print(t_argv *env)
{
	int		i;
	t_argv	*tmp;
	t_argv	*exp;

	exp = g_et->array[1];
	set_sorted_tmp(&tmp, env, exp);
	i = 0;
	while (i < tmp->len)
		ft_printf("declare -x %s\n", tmp->array[i++]);
	argv_destroy(tmp, free);
}

int	compare_this(char *s1, char *s2)
{
	int		res;
	char	*tmp;

	tmp = ft_strchr(s2, '=');
	res = ft_strncmp(s2, s1, tmp - s2 + 1);
	return (res);
}

int compare_env(char *s1, char *s2)
{
	int		res;
	char	*tmp;

	tmp = ft_strjoin(s2, "=");
	res = ft_strncmp(s1, tmp, ft_strlen(tmp));
	free(tmp);
	return (res);
}

int	compare_exp(char *s1, char *s2)
{
	int		res;
	char	*tmp;

	tmp = ft_strjoin(s1, "=");
	res = ft_strncmp(s2, tmp, ft_strlen(tmp));
	free(tmp);
	return (res);
}

int	ft_export(t_argv *cmd)
{
	t_argv	*env;
	int		i;
	int		status;
	t_argv	*exp;

	i = 1;
	status = 0;
	exp = g_et->array[1];
	env = g_et->array[0];
	if (cmd->len == 1)
	{
		env_print(env);
		// export ciktisi gibi sirali olacak ve export edilmis env olmayan seylerde olacak
		return (0);
	}
	while (i < cmd->len)
	{
		env->try_index = 0;
		exp->try_index = 0;
		if (ft_isstring(cmd->array[i])) // burada biraz sicmisim sorry :(
		{
			if (argv_try(exp, cmd->array[i], 0, (int (*)(void *,
							void *))ft_strcmp) && argv_try(env, cmd->array[i],
					0, (int (*)(void *, void *))compare_env))
				argv_push(exp, ft_strdup(cmd->array[i]));
			else
				++i;
		}
		else if (argv_try(exp, cmd->array[i], 0, (int (*)(void *,
							void *))compare_exp) == 0)
		{
			argv_del_one(exp, exp->try_index, free);
			argv_push(env, ft_strdup(cmd->array[i++]));
		}
		else if ((argv_try(env, cmd->array[i], 0, (int (*)(void *,
								void *))compare_this) == 0)
				&& ft_is_valid_env(cmd->array[i]))
		{
			argv_del_one(env, env->try_index, free);
			argv_insert(env, env->try_index, ft_strdup(cmd->array[i++]));
		}
		else if ((argv_try(env, cmd->array[i], 0, (int (*)(void *,
								void *))compare_this) != 0
					&& ft_is_valid_env(cmd->array[i])))
			argv_push(env, ft_strdup(cmd->array[i++]));
		else
		{
			ft_printf("bash: export: `%s': not a valid identifier\n",
						cmd->array[i++]);
			status = 1;
		}
	}
	return (status);
}

int	ft_unset(t_argv *cmd)
{
	t_argv	*env;
	t_argv	*exp;
	int		i;
	int		status;
	char	*tmp;

	i = 1;
	status = 0;
	env = g_et->array[0];
	exp = g_et->array[1];
	while (i < cmd->len)
	{
		env->try_index = 0;
		if (ft_isstring(cmd->array[i]))
		// orda sictiysam burdada sicmisimdir sorry :( tuy diktim uzerine :( sicip tuy dikmemisim :)
		{
			tmp = ft_strjoin(cmd->array[i], "=");
			if (argv_try(exp, cmd->array + i, 0, (int (*)(void *,
							void *))ft_strcmp) == 0)
				argv_del_one(exp, exp->try_index, free);
			else if (argv_try(env, tmp, env->try_index, (int (*)(void *,
								void *))compare_this) == 0)
				argv_del_one(env, env->try_index, free);
		}
		else
		{
			ft_printf("bash: unset: `%s': not a valid identifier\n",
						cmd->array[i++]);
			status = 1;
		}
		if (tmp)
			free(tmp);
		++i;
	}
	return (status);
}

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

int is_valid_n(char *str)
{
	int i;

	i = 0;
	if (str[i] == '-')
		++i;
	while (str[i])
	{
		if (str[i] == 'n')
			++i;
		else
			return (0);
	}
	return (1);
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
		while (i < cmd->len)
		{
			if (is_valid_n(cmd->array[i]))
				{n = 1;++i;}
			else
				break ;
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

int	ft_cd(t_argv *cmd) //~ icin duzeltme gerekli calismiyor. // duzeltildi
{
	char cwd[512];
	char *path;
	char *pwd;
	t_argv *env;
	char *tmp;

	env = g_et->array[0];
	if (cmd->len == 2)
	{
		tmp = ft_strdup(cmd->array[1]);
		if (tmp[0] == '~')
		{
			path = str3join(get_env(ft_strdup("$HOME")), ft_strdup(""),
					ft_strdup(tmp + 1));
			free(tmp);
		}
		else
			path = tmp;
		if (0 == chdir(path))
		{
			free(path);
			env->try_index = 0;
			getcwd(cwd, 512);
			if (argv_try(env, "OLDPWD=", 0, (int (*)(void *,
							void *))env_cmp) == 0)
			{
				pwd = get_env(ft_strdup("$PWD"));
				argv_del_one(env, env->try_index, free);
				argv_insert(env, env->try_index, ft_strjoin("OLDPWD=", pwd));
				free(pwd);
			}
			env->try_index = 0;
			if (argv_try(env, "PWD=", 0, (int (*)(void *, void *))env_cmp) == 0)
				argv_del_one(env, env->try_index, (void (*)(void *))free);
			argv_insert(env, env->try_index, ft_strjoin("PWD=", cwd));
		}
		else
		{
			ft_printf("minishell: cd: %s: No such file or directory\n", path);
			free(path);
			return (1);
		}
	}
	else
	{
		ft_printf("minishell: cd: Need a directory\n");
		return (1);
	}
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

char	*clear_this(char *line, int c, int status)
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
		return (implement(ret, status));
	return (ft_strdup(ret));
}

char	*lexer_quote(char **string, int status)
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
			ret = clear_this(res, '\"', status);
		else
			ret = clear_this(res, '\'', status);
		free(res);
		return (ret);
	};
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
			continue ;
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

int	lexer(t_argv *cmd, char *line, int status)
{
	int		i;
	char	*tmp;

	i = 0;
	while (line && *line)
	{
		line = lexer_token(cmd, line);
		while (ft_isspace(*line))
			line++;
		line = lexer_word_plus_quit(cmd, line, status);
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

	printf("\n");
	i = 0;
	while (cmd->array[i])
	{
		printf("---->%s\n", cmd->array[i]);
		i++;
	}
	printf("\n");
}

int	find_procces_size(t_argv *exec)
{
	int i; // anlamadım  evet
	i = 0;
	exec->try_index = 0;
	while (!argv_try(exec, "|", exec->try_index, (int (*)(void *,
					void *))ft_strcmp))
	{
		++i;
		exec->try_index++;
	}
	++i;
	return (i);
}

int	wait_all(int pid, int max)
{
	int	i;
	int	st;
	int	last_pid;

	i = 0;
	while (i < max)
	{
		if (waitpid(0, &st, 0) == pid)
			last_pid = st;
		i++;
	}
	return (last_pid >> 8);
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

char	*get_path(char *cmd)
{
	t_argv	*path;
	char	**tmp;
	char	*str;
	char	*ret;

	if (!ft_strncmp("/", cmd, 1) || !ft_strncmp("./", cmd, 2)
		|| !ft_strncmp("../", cmd, 3))
		return (ft_strdup(cmd));
	if (!ft_strncmp("~", cmd, 1))
		return (str3join(ft_strdup(getenv("$HOME")), ft_strdup("/"),
				ft_strdup(cmd + 1)));
	str = get_env(ft_strdup("$PATH"));
	ret = NULL;
	if (!*str)
	{
		free(str);
		return (NULL);
	}
	tmp = ft_split(str, ':');
	free(str);
	path = argv_new((void **)tmp, NULL);
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

int	builtin_tester(t_argv *cmd)
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

void	folder_operations(t_argv *cmd)
{
	int	io[2];
	int	i;

	i = 0;
	while (i < cmd->len)
	{
		cmd->try_index = 0;
		if (argv_try(cmd, "<", cmd->try_index, (int (*)(void *,
						void *))ft_strcmp) == 0)
		{
			io[0] = open(ft_strdup(cmd->array[cmd->try_index + 1]), O_RDONLY);
			if (io[0] == -1)
				printf("minishell: %s: No such file or directory",
						cmd->array[cmd->try_index + 1]);
			dup2(io[0], STDIN_FILENO);
			close(io[0]);
			argv_del_one(cmd, cmd->try_index, free);
			argv_del_one(cmd, cmd->try_index, free);
		}
		else if (argv_try(cmd, ">", cmd->try_index, (int (*)(void *,
							void *))ft_strcmp) == 0)
		{
			io[1] = open(cmd->array[cmd->try_index + 1],
							O_WRONLY | O_CREAT | O_TRUNC,
							0644);
			if (io[1] == -1)
			{
				printf("minishell: %s: No such file or directory\n",
						cmd->array[cmd->try_index + 1]);
				exit(127);
			}
			dup2(io[1], STDOUT_FILENO);
			close(io[1]);
			argv_del_one(cmd, cmd->try_index, free);
			argv_del_one(cmd, cmd->try_index, free);
		}
		else if (argv_try(cmd, ">>", cmd->try_index, (int (*)(void *,
							void *))ft_strcmp) == 0)
		{
			io[1] = open(cmd->array[cmd->try_index + 1],
							O_WRONLY | O_CREAT | O_APPEND,
							0644);
			if (io[1] == -1)
			{
				printf("minishell: %s: No such file or directory",
						cmd->array[cmd->try_index + 1]);
				exit(127);
			}
			dup2(io[1], STDOUT_FILENO);
			close(io[1]);
			argv_del_one(cmd, cmd->try_index, free);
			argv_del_one(cmd, cmd->try_index, free);
		}
		else if (argv_try(cmd, "<<", cmd->try_index, (int (*)(void *,
							void *))ft_strcmp) == 0)
		{
			pipe(io);
			write(io[1], cmd->array[cmd->try_index + 1],
					ft_strlen(cmd->array[cmd->try_index + 1]));
			close(io[1]);
			dup2(io[0], 0);
			close(io[0]);
			argv_del_one(cmd, cmd->try_index, free);
			argv_del_one(cmd, cmd->try_index, free);
		}
		else
			i++;
	}
}

int	exec_this(t_argv *cmd)
{
	char	*path;
	t_argv	*env;
	int		status;

	if (is_builtin(cmd))
		return (builtin_tester(cmd));
	path = get_path(cmd->array[0]);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", cmd->array[0]);
		exit (127);
	}
	folder_operations(cmd);
	env = g_et->array[0];
	status = execve(path, (char **)cmd->array, (char **)env->array);
	if (-1 == status)
	{
		exit(127);
	}
	exit(status);
}

void	child_exec(int io[], t_argv *trgt, int start, int not_end)
{
	int	fd;

	if (start)
		dup2(io[0], STDIN_FILENO);
	if (not_end)
		dup2(io[1], STDOUT_FILENO);
	close(io[0]);
	close(io[1]);
	exec_this(trgt);
}

int	exec_all(t_argv *exec, int max_proc)
{
	int		pid;
	int		i;
	int		io[2];
	t_argv	*trgt;
	int		status;

	i = 0;
	while (i < max_proc)
	{
		if (-1 == argv_try(exec, "|", 0, (int (*)(void *, void *))ft_strcmp))
			trgt = argv_splice(exec, 0, exec->len);
		else
		{
			trgt = argv_splice(exec, 0, exec->try_index);
			argv_del_one(exec, 0, (void (*)(void *))free);
		}
		pipe(io);
		pid = fork();
		if (pid == 0)
			child_exec(io, trgt, (i != 0), (i != max_proc - 1));
		argv_destroy(trgt, (void (*)(void *))free);
		close(io[1]); // bu calışır sanırım
		++i;
	}
	status = wait_all(pid, max_proc);
	return (status);
}

int	builtin_operation(t_argv *cmd)
{
	int	in;
	int	out;
	int	ret;

	in = dup(0);
	out = dup(1);
	ret = exec_this(cmd);
	dup2(in, 0);
	dup2(out, 1);
	close(in);
	close(out);
	return (ret);
}

char	*check_token(t_argv *cmd)
{
	int		i;
	char	*tmp;
	char	*tmp2;

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
		ft_printf("\n");
		rl_on_new_line();
		rl_redisplay();
		ft_printf("$> ");
	}
	else if (sig == SIGQUIT)
	{
		ioctl(0, TIOCSTI);
	}
	return (0);
}

int check_line(char *line)
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
	int		i;
	char	*str;
	t_argv	*cmd;
	t_argv	*env;

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
	status = 0;
	while (1)
	{
		// signal(SIGINT, signal_handler);
		// signal(SIGQUIT, signal_handler);
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
		if (str)
			printf("minishell: syntax error near unexpected token `%s'\n", str);
		else if (argv_try(cmd, "|", 0, (int (*)(void *, void *))ft_strcmp) != 0
				&& is_builtin(cmd))
			status = builtin_operation(cmd);
		else
			status = exec_all(cmd, find_procces_size(cmd));
		// path icin ~ eklemesi yapilacak.
		//print_cmd(cmd);
		if (str)
			free(str);
		argv_destroy(cmd, free);
		//system("leaks minishell");
	}
	return (0);
}
