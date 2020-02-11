/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:24:03 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 17:38:35 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"

/*
** NOTE: DYLD_LIBRARY_PATH to compile
*/

char	g_charset[] = "0123456789abcdef";

void	putaddr(void *ptr)
{
	char	buff[8];
	size_t	ptr_;
	size_t	i;

	ptr_ = (size_t)ptr;
	write(1, "0x", 2);
	i = 8;
	while (i--)
	{
		buff[i] = g_charset[ptr_ % 16];
		ptr_ /= 16;
	}
	write(1, buff, 8);
	write(1, "\n", 1);
}

int		main(void)
{
	char	*str;
	char	*str2;

	str = malloc(42);
	str2 = malloc(13);
	putaddr(str);
	putaddr(str2);
}
