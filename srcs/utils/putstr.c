/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putstr.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 21:47:34 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 21:57:12 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	ft_putsizet(size_t nbr)
{
	char	buffer[21];
	size_t	i;

	i = 21;
	while (nbr && i--)
	{
		buffer[i] = '0' + nbr % 10;
		nbr /= 10;
	}
	write(1, buffer + i, 21 - i);
}

void	ft_putstr_fd(char *str, int fd)
{
	char	*end;

	end = str;
	while (*end)
		end++;
	write(fd, str, end - str);
}

void	ft_putstr(char *str)
{
	ft_putstr_fd(str, 1);
}
