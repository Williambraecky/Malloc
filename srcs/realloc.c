/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:07:28 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/12 16:03:23 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include <stdio.h>

/*
** TODO: check if space is available before allocating a new if needed
** NOTE: special case if size is less than before
** NOTE: automatically malloc new node if block type is bigger than before
**   NOTE: should we also do it if it is smaller? to keep the
**      tiny small large rules?
*/

/*
** NOTE: basically segment the blk
*/

static void	*realloc_segment(t_blk *blk, t_mlc *mlc, size_t size)
{
	t_mlc	*next;
	t_mlc	*segment;
	size_t	offset;
	size_t	blk_min_size;

	next = mlc->next;
	offset = ((void*)next) - ((void*)mlc + sizeof(t_mlc)) - size;
	if (blk->block_type == TINY)
		blk_min_size = sizeof(t_mlc) + 1;
	else
		blk_min_size = sizeof(t_mlc) + TINY_MAX;
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
	mlc->checksum = hash_mlc(mlc);
	return ((void*)mlc + sizeof(*mlc));
}

static void	*realloc_bigger_blk(void *ptr, size_t size, size_t new_size)
{
	void	*new_ptr;

	if (!(new_ptr = malloc(new_size)))
		return (NULL);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

static void	remove_mlc_from_free_list(t_blk *blk, t_mlc *mlc)
{
	t_mlc	*curr;
	t_mlc	*prev_free;

	curr = blk->first_free;
	if (curr == mlc)
	{
		blk->first_free = curr->next_free;
		return ;
	}
	while (curr)
	{
		if (curr == mlc)
		{
			if (prev_free)
				prev_free->next_free = mlc->next_free;
			return ;
		}
		prev_free = curr;
		curr = curr->next_free;
	}
}

/*
** NOTE: try to expand mlc, this can only work if mlc->next
**    is not in use or if it is wilderness
** NOTE: this will try to segment
*/

static void	*realloc_bigger(t_blk *blk, t_mlc *mlc, size_t new_size)
{
	size_t	old_size;
	t_mlc	*next;

	old_size = mlc_size(mlc);
	if (mlc->next == blk->wilderness &&
		!out_of_range(blk, mlc + sizeof(*mlc) + new_size))
	{
		mlc->next = (void*)mlc + sizeof(*mlc) + new_size;
		blk->wilderness = mlc->next;
		return ((void*)mlc + sizeof(*mlc));
	}
	if (mlc->next->in_use || !(new_size <= old_size + mlc_size(mlc->next)))
	{
		return (realloc_bigger_blk((void*)mlc + sizeof(*mlc),
			old_size, new_size));
	}
	remove_mlc_from_free_list(blk, mlc->next);
	next = mlc->next;
	mlc->next = next->next;
	next->prev = mlc->next;
	return (realloc_segment(blk, mlc, new_size));
}

void		*realloc(void *ptr, size_t size)
{
	t_mlc	*mlc;
	t_blk	*blk;
	size_t	old_size;

	mlc = ptr - sizeof(*mlc);
	if (mlc->checksum != hash_mlc(mlc))
		return (NULL); //TODO: error message invalid checksum
	if (mlc->in_use == 0)
		return (NULL); //TODO: error message use after free
	old_size = mlc_size(mlc);
	if (old_size == size)
		return (ptr);//NOTE: same size don't need to do anything
	blk = get_blk_from_addr(mlc);
	if (!blk)
		return (NULL);
	if (size < old_size)
		return (realloc_segment(blk, mlc, size));
	else if (get_type(size) > get_type(old_size))
		return (realloc_bigger_blk(ptr, old_size, size));
	return (realloc_bigger(blk, mlc, size));
}
