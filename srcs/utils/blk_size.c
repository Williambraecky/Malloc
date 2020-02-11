/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blk_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:52:16 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 16:13:19 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

size_t	blk_size(int type, size_t size)
{
	size_t	blk_size;

	if (type == TINY)
		return (((size_t)getpagesize()) * 8);
	else if (type == SMALL)
		return (((size_t)getpagesize()) * 64);
	blk_size = 1;
	while (blk_size * getpagesize() < (size + sizeof(t_blk) + sizeof(t_mlc)))
		blk_size++;
	return (blk_size * getpagesize());
}
