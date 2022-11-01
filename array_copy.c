/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_copy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysay <ysay@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 12:49:25 by ysay              #+#    #+#             */
/*   Updated: 2022/09/15 17:06:02 by ysay             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <array.h>

void	**array_copy_addr(void *dest[], void *src[])
{
	size_t	i;

	if (dest == NULL || src == NULL)
		return (NULL);
	i = 0;
	while (src[i] != NULL)
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = NULL;
	return (dest);
}

void	**array_copy_addr_n(void *dest[], void *src[], size_t n)
{
	size_t	i;

	if (dest == NULL || src == NULL || !n)
		return (NULL);
	i = 0;
	while (src[i] != NULL && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	dest[i] = NULL;
	return (dest);
}

void	**array_deep_copy(void *dest[], void *src[], void *(*copy)(void *))
{
	size_t	i;

	if (dest == NULL || src == NULL || copy == NULL)
		return (NULL);
	i = 0;
	while (src[i] != NULL)
	{
		dest[i] = copy(src[i]);
		++i;
	}
	dest[i] = NULL;
	return (dest);
}

void	**array_deep_copy_n(void *dest[], void *src[],
			void *(*copy)(void *), size_t n)
{
	size_t	i;

	if (dest == NULL || src == NULL || !n || copy == NULL)
		return (NULL);
	i = 0;
	while (src[i] != NULL && i < n)
	{
		dest[i] = copy(src[i]);
		++i;
	}
	dest[i] = NULL;
	return (dest);
}
