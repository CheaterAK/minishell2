#ifndef MINISHELL_H
# define MINISHELL_H
# include <stddef.h>

/*
** utils.c
*/

int				ft_strcmp(const char *str1, const char *str2);
int				ft_is_builtins(const char *str);
char			*ft_strncpy(char *des, const char *src, size_t n);
char *str3join(char *s1, char *s2, char *s3);
int	ft_is_valid_env(const char *str);
int ft_isequal(int c);
int ft_isstring(const char *str);
#endif
