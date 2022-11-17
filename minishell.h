/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocabas <akocabas@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:33:54 by akocabas          #+#    #+#             */
/*   Updated: 2022/11/17 16:16:25 by akocabas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <argv.h>
# include <stddef.h>
/*
** utils.c
*/

int		ft_strcmp(const char *str1, const char *str2);
char	*ft_strncpy(char *des, const char *src, size_t n);
char	*str3join(char *s1, char *s2, char *s3);
int		ft_is_valid_env(const char *str);
int		ft_isequal(int c);
int		ft_isstring(const char *str);
int		is_builtin(t_argv *cmd);

/*
** builtins1.c
*/

int		ft_echo(t_argv *cmd);
int		ft_cd(t_argv *cmd);
int		ft_pwd(t_argv *cmd);
int		ft_export(t_argv *cmd);
int		ft_unset(t_argv *cmd);

/*
** builtins2.c
*/

int		ft_env(t_argv *cmd);
int		ft_exit(t_argv *cmd);

/*
** builtins3.c
*/

int		env_cmp(char *s1, char *s2);

/*
** builtins4.c
*/

char	*get_env(char *str);
void	env_print(t_argv *env);
int		compare_this(char *s1, char *s2);

/*
** builtins5.c
*/

int		compare_env(char *s1, char *s2);
int		compare_exp(char *s1, char *s2);

/*
** exec.c
*/
int		exec_this(t_argv *cmd);
int		exec_all(t_argv *exec, int max_proc);
int		builtin_exec(t_argv *cmd);
int		builtin_op(t_argv *cmd);

/*
** redirects.c
*/

void	folder_operations(t_argv *cmd);

t_argv	*g_et;

#endif
