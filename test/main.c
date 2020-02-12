/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:24:03 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/12 15:19:15 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"
#include <stdio.h>

/*
** NOTE: DYLD_LIBRARY_PATH to compile
*/

int		main(void)
{
	t_blk	*blk;
	void	*ptr;
	void	*ptr2;
	size_t	i;

	ptr = malloc(16);
	blk = g_blks;
	ptr2 = malloc(96);
	i = 1000;
	while (i--)
	{
		ptr = malloc(3);
		free(ptr);
	}
	free(ptr2);
	show_alloc_mem();
	ptr2 = malloc(64);
	ptr = malloc(16);
	ptr = malloc(16);
	ptr = malloc(16);
	write(1, "\n\n", 2);
	show_alloc_mem();
}
