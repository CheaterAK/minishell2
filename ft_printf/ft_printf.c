/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocabas <akocabas@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/26 17:29:37 by akocabas          #+#    #+#             */
/*   Updated: 2022/09/26 00:29:55 by akocabas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ft_printf.h"

int	ft_put(unsigned long nbr, char *str_base, int chk, int base_len)
{
	int	len;
	int	i;

	i = -1;
	len = 0;
	if (chk == 3)
	{
		if (!str_base)
			return (write(1, "(null)", 6));
		else
			while (str_base[++i])
				len += write(1, &str_base[i], 1);
		return (len);
	}
	if (chk == 1)
		len += write(1, "0x", 2);
	if ((int)nbr < 0 && chk == 2)
	{
		nbr *= -1;
		len += write(1, "-", 1);
	}
	if (nbr / base_len != 0)
		len += ft_put(nbr / base_len, str_base, 0, base_len);
	len += write(1, &str_base[nbr % base_len], 1);
	return (len);
}

int	ft_control(char c, va_list args)
{
	int	chr;

	if (c == 'c')
	{
		chr = va_arg(args, int);
		return (write(1, &chr, 1));
	}
	else if (c == 's')
		return (ft_put(0, va_arg(args, char *), 3, 0));
	else if (c == 'p')
		return (ft_put(va_arg(args, unsigned long), "0123456789abcdef", \
		1, 16));
	else if (c == 'd' || c == 'i')
		return (ft_put(va_arg(args, int), "0123456789", 2, 10));
	else if (c == 'u')
		return (ft_put(va_arg(args, unsigned), "0123456789", 0, 10));
	else if (c == 'x')
		return (ft_put(va_arg(args, unsigned), "0123456789abcdef", 0, 16));
	else if (c == 'X')
		return (ft_put(va_arg(args, unsigned), "0123456789ABCDEF", 0, 16));
	else if (c == '%')
		return (write(1, &c, 1));
	else
		return (write(1, &c, 1));
	return (0);
}

int	ft_printf(const char *str, ...)
{
	int		len;
	int		i;
	va_list	args;

	va_start(args, str);
	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			while (str[++i] == ' ')
				;
			len += ft_control(str[i], args);
		}
		else
			len += write(1, &str[i], 1);
		i++;
	}
	va_end(args);
	return (len);
}
