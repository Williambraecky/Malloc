/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:24:03 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 23:18:25 by wbraeckm         ###   ########.fr       */
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
	ptr2 = malloc(96);
	show_alloc_mem();
	ptr2 = realloc(ptr2, 63);
	ptr = malloc(16);
	ptr = malloc(16);
	ptr = malloc(16);
	write(1, "\n\n", 2);
	show_alloc_mem();
}
