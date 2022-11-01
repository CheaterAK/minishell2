/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 12:49:43 by ysay              #+#    #+#             */
/*   Updated: 2022/09/15 17:05:39 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_H
# define ARRAY_H
# include <stdlib.h>
# include <stddef.h>

size_t	find_max_bit(size_t num);
size_t	array_len(void *array[]);
int		array_shift_right(void *array[], size_t n);
int		array_shift_left(void *array[], size_t n);
int		array_destroy(void *array[], void (*del)(void *));
int		array_del_all(void *array[], void (*del)(void *));
int		array_del_n(void *array[], void (*del)(void *), size_t n);
int		array_del_one(void *array[], void (*del)(void *));
void	**array_copy_addr(void *dest[], void *src[]);
void	**array_copy_addr_n(void *dest[], void *src[], size_t n);
void	**array_deep_copy(void *dest[], void *src[], void *(*copy)(void *));
void	**array_deep_copy_n(void *dest[], void *src[],
			void *(*copy)(void *), size_t n);
void	**array_expand(void *array[], size_t expansion_len);
int		array_insert(void *array[], void *addr);
int		array_insert_array(void *array[], void *addr[]);
int		array_insert_array_n(void *array[], void *addr[], size_t n);
int		array_reverse_all(void *array[]);

#endif