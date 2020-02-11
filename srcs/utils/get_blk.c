/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_blk.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:34:06 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 21:59:25 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

extern t_blk	*g_blks;

/*
** NOTE: size must always be a multiple of getpagesize
*/

static t_blk	*new_blk(int type, size_t size)
{
	t_blk		*new;

	new = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE,
		-1, 0);
	if (!new)
		return (NULL);
	ft_memset(new, 0, sizeof(*new));
	new->block_type = type;
	new->size = size;
	new->available = size - sizeof(*new) - sizeof(t_mlc);
	new->wilderness = (t_mlc*)((void*)new + sizeof(*new));
	return (new);
}

/*
** NOTE: size must always be a multiple of getpagesize
*/

t_blk			*append_new_blk(int type, size_t size)
{
	t_blk		*new;
	t_blk		*last;

	if (!(new = new_blk(type, size)))
		return (NULL);
	last = g_blks;
	while (last && last->next)
		last = last->next;
	if (last)
	{
		last->next = new;
		new->prev = last;
		last->checksum = hash_blk(last);
	}
	new->checksum = hash_blk(new);
	if (!g_blks)
		g_blks = new;
	return (new);
}

static t_blk	*find_blk(int type, size_t size)
{
	t_blk	*curr;

	curr = g_blks;
	while (curr)
	{
		if (curr->block_type == type && size < curr->available)
			return (curr);
		curr = curr->next;
	}
	return (append_new_blk(type, blk_size(type, size)));
}

t_blk			*get_blk_from_addr(void *ptr)
{
	t_blk		*curr;

	curr = g_blks;
	while (curr)
	{
		if ((void*)curr < ptr && ptr < ((void*)curr) + curr->size)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

t_blk			*get_blk(int type, size_t size)
{
	if (type == LARGE)
		return (append_new_blk(type, blk_size(type, size)));
	return (find_blk(type, size));
}
