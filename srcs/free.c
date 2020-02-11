/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:07:34 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 22:30:59 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	unmap_blk(t_blk *blk)
{
	if (blk->prev)
		blk->prev->next = blk->next;
	if (blk->next)
		blk->next->prev = blk->prev;
	if (g_blks == blk)
		g_blks = blk->next;
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
			curr->checksum = hash_mlc(curr);
			mlc->checksum = hash_mlc(mlc);
			return ;
		}
		curr = curr->next_free;
	}
	curr->next_free = mlc;
	curr->checksum = hash_mlc(curr);
	mlc->checksum = hash_mlc(mlc);
}

void		add_free_list(t_blk *blk, t_mlc *mlc)
{
	if (mlc < blk->first_free || !blk->first_free)
	{
		mlc->next_free = blk->first_free;//checksum
		mlc->checksum = hash_mlc(mlc);
		blk->first_free = mlc;
	}
	else
		insert_in_free(blk, mlc);
}

/*
** TODO: some of these need to be rechecksummed
*/

static void	free_mlc(t_blk *blk, t_mlc *mlc)
{
	if (mlc->prev && mlc->prev->in_use == 0) //Case previous malloc is not allocated
	{
		blk->available += sizeof(t_mlc);
		mlc->prev->next = mlc->next;
		mlc->prev->checksum = hash_mlc(mlc->prev);
		mlc->next->prev = mlc->prev;
		mlc->next->checksum = hash_mlc(mlc->next);
		mlc = mlc->prev;
	}
	if (mlc->next < blk->wilderness && mlc->next->in_use == 0)
	{
		blk->available += sizeof(t_mlc);
		mlc->next_free = mlc->next->next_free;
		mlc->next = mlc->next->next;
		mlc->checksum = hash_mlc(mlc);
		mlc->next->prev = mlc;
		mlc->next->checksum = hash_mlc(mlc->next);
	}
	// if (mlc == blk->last) //TODO: this is incorrect; do we really need to push back wilderness?
	// {
	// 	blk->wilderness = mlc;
	// 	blk->last = mlc->prev;
	// }
	add_free_list(blk, mlc);
}

void		free(void *ptr)
{
	t_mlc	*mlc;
	t_blk	*blk;

	mlc = ptr - sizeof(t_mlc);
	malloc_lock();
	if (mlc->checksum != hash_mlc(mlc))
		return malloc_unlock();//TODO: indalid checksum error message
	if (mlc->in_use == 0)
		return malloc_unlock();//TODO: double free error message
	blk = get_blk_from_addr(ptr);
	if (!blk)
		return malloc_unlock();//TODO: could not find blk error message
	blk->num_alloc--;
	if (!blk->num_alloc)
	{
		unmap_blk(blk);
		return (malloc_unlock());
	}
	blk->available += mlc_size(mlc);
	free_mlc(blk, mlc);
	mlc->in_use = 0;
	malloc_unlock();
}
