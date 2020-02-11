/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:07:28 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 15:03:14 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

/*
** NOTE: check if space is available before allocating a new if needed
*/

void	*realloc(void *ptr, size_t size)
{
	t_mlc	*mlc;

	mlc = ptr - sizeof(mlc);
	(void)size;
	return (ptr);
}
