/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:06:44 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 21:35:45 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

t_blk	*g_blks = 0;

/*
** NOTE: this might not segment all the time
*/

/*
** NOTE: this will check if there is enough space between mlc and mlc->next
**   for the new malloc and another node
*/

 void		mlc_segment(t_blk *blk, t_mlc *mlc, size_t size)
{
	t_mlc		*next;
	t_mlc		*segment;
	size_t		offset;
	size_t		blk_min_size;

	next = mlc->next;
	offset = ((void*)next) - ((void*)mlc + sizeof(t_mlc));
	if (blk->block_type == TINY)
		blk_min_size = sizeof(t_mlc) + 1;
	else
		blk_min_size = sizeof(t_mlc) + TINY_MAX;
	offset -= size;
	if (offset >= blk_min_size)
	{
		segment = (t_mlc*)((void*)mlc + size + sizeof(*mlc));
		segment->in_use = 0;
		mlc->next = segment;
		segment->next = next;
		next->prev = segment;
		next->checksum = hash_mlc(next);
		segment->prev = mlc;
		segment->checksum = hash_mlc(segment);
		add_free_list(blk, segment);
	}
}

 t_mlc	*check_free_mlc(t_blk *blk, size_t size)
{
	t_mlc	*current;
	t_mlc	*prev;

	current = blk->first_free;
	prev = NULL;
	while (current)
	{
		if (size <= mlc_size(current))
		{
			current->in_use = 1;
			if (prev)
				prev->next_free = current->next_free;
			else
				blk->first_free = current->next_free;
			mlc_segment(blk, current, size);
			current->checksum = hash_mlc(current);
			blk->num_alloc++;
			blk->available -= size;
			return (current);
		}
		prev = current;
		current = current->next_free;
	}
	return (NULL);
}

 t_mlc	*get_mlc_in_blk(t_blk *blk, size_t size)
{
	t_mlc		*mlc;

	if ((mlc = check_free_mlc(blk, size)))
		return (mlc);
	mlc = blk->wilderness;
	mlc->prev = blk->last;
	blk->last = mlc;
	mlc->in_use = 1;
	mlc->next = (void*)mlc + sizeof(*mlc) + size;
	blk->wilderness = mlc->next;
	mlc->next_free = NULL;
	mlc->checksum = hash_mlc(mlc);
	blk->checksum = hash_blk(blk);
	blk->num_alloc++;
	blk->available -= sizeof(*mlc) + size;
	return (mlc);
}

/*
** TODO: define malloc type
** TODO: get right blk
** TODO: find the right place for it
** NOTE: blk might not have enough space even though there is enough total space
*/

void			*malloc(size_t size)
{
	t_blk	*blk;
	t_mlc	*mlc;

	if (!(blk = get_blk(get_type(size), size)))
		return (NULL);
	if ((mlc = get_mlc_in_blk(blk, size)))
		return (((void*)mlc) + sizeof(*mlc));
	if (!(blk = append_new_blk(get_type(size), blk_size(get_type(size), size))))
		return (NULL);
	if ((mlc = get_mlc_in_blk(blk, size)))
		return (((void*)mlc) + sizeof(*mlc));
	return (NULL);
}
