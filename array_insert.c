/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_insert.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 12:49:32 by ysay              #+#    #+#             */
/*   Updated: 2022/09/15 12:49:33 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <array.h>

int	array_insert(void *array[], void *addr)
{
	if (array == NULL || addr == NULL)
		return (-1);
	array_shift_right(array, 1);
	array[0] = addr;
	return (1);
}

int	array_insert_array(void *array[], void *addr[])
{
	size_t	len;

	if (array == NULL || addr == NULL)
		return (-1);
	len = array_len(addr);
	array_shift_right(array, len);
	array_copy_addr(array, addr);
	return (len);
}

int	array_insert_array_n(void *array[], void *addr[], size_t n)
{
	size_t	len;

	if (array == NULL || addr == NULL || !n)
		return (-1);
	len = array_len(addr);
	if (len < n)
		n = len;
	array_shift_right(array, n);
	array_copy_addr_n(array, addr, n);
	return (n);
}
