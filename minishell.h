/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocabas <akocabas@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:33:54 by akocabas          #+#    #+#             */
/*   Updated: 2022/11/17 12:34:49 by akocabas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stddef.h>
# include <argv.h>
/*
** utils.c
*/

int		ft_strcmp(const char *str1, const char *str2);
int		ft_is_builtins(const char *str);
char	*ft_strncpy(char *des, const char *src, size_t n);
char	*str3join(char *s1, char *s2, char *s3);
int		ft_is_valid_env(const char *str);
int		ft_isequal(int c);
int		ft_isstring(const char *str);

t_argv	*g_et;
extern	g_et;
#endif
