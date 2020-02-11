/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:24:03 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 22:15:47 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/malloc.h"
#include <stdio.h>

/*
** NOTE: DYLD_LIBRARY_PATH to compile
*/

int		main(void)
{
	void	*ptr;
	void	*ptr2;
	size_t	i;

	ptr = malloc(16);
	ptr2 = malloc(64);
	i = 666;
	while (i--)
		malloc(32);
	// show_alloc_mem();
	free(ptr2);
	write(1, "\n\n", 2);
	// show_alloc_mem();
}
