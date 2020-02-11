/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:07:34 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 16:01:46 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	unmap_blk(t_blk *blk)
{
	if (blk->prev)
		blk->prev->next = blk->next;
	if (blk->next)
		blk->next->prev = blk->prev;
	munmap(blk, blk->size);
}

/*
** NOTE: assumes first_free is not null
*/

static void	insert_in_free(t_blk *blk, t_mlc *mlc)
{
	t_mlc	*curr;

	curr = blk->first_free;
	while (curr->next_free)
	{
		if ((void*)curr < (void*)mlc && (void*)mlc < (void*)curr->next_free)
		{
			mlc->next_free = curr->next_free;
			curr->next_free = mlc;
			return ;
		}
		curr = curr->next_free;
	}
	curr->next_free = mlc;
}

/*
** TODO: some of these need to be rechecksummed
*/

static void	free_mlc(t_blk *blk, t_mlc *mlc)
{
	blk->available += mlc_size(mlc);
	if (mlc->prev && mlc->prev->in_use == 0)
	{
		mlc->prev->next = mlc->next;
		if (mlc->next)
			mlc->next->prev = mlc->prev;
		blk->available += sizeof(t_mlc);
		mlc = mlc->prev;
	}
	if (mlc->next && mlc->next->in_use == 0)
	{
		blk->available += sizeof(t_mlc);
		mlc->next = mlc->next->next;
		mlc->next_free = mlc->next->next_free;
		if (mlc->next->next)
			mlc->next->next->prev = mlc;
	}
	if (mlc < blk->first_free || !blk->first_free)
	{
		mlc->next_free = blk->first_free;
		blk->first_free = mlc;
	}
	else
		insert_in_free(blk, mlc);
}

void		free(void *ptr)
{
	t_mlc	*mlc;
	t_blk	*blk;

	mlc = ptr - sizeof(t_mlc);
	if (mlc->checksum != hash_mlc(mlc))
		return ;//TODO: indalid checksum error message
	if (mlc->in_use == 0)
		return ;//TODO: double free error message
	blk = get_blk_from_addr(ptr);
	if (!blk)
		return ;//TODO: could not find blk error message
	blk->num_alloc--;
	if (!blk->num_alloc)
		return (unmap_blk(blk));
	free_mlc(blk, mlc);
	mlc->in_use = 0;
}
