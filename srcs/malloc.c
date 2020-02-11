/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:06:44 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 18:57:35 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_blk	*g_blks = 0;

/*
** NOTE: this might not segment all the time
*/

/*
** NOTE: this will check if there is enough space between mlc and mlc->next
**   for the new malloc and another node
*/
char	g_charset[] = "0123456789abcdef";

void	putaddr(void *ptr)
{
	char	buff[8];
	size_t	ptr_;
	size_t	i;

	ptr_ = (size_t)ptr;
	write(1, "0x", 2);
	i = 8;
	while (i--)
	{
		buff[i] = g_charset[ptr_ % 16];
		ptr_ /= 16;
	}
	write(1, buff, 8);
	write(1, "\n", 1);
}

void	putnbr(size_t nbr)
{
	char	buff[50];
	size_t	i;

	i = 32;
	while (i--)
	{
		buff[i] = '0' + nbr % 10;
		nbr /= 10;
	}
	write(1, buff, 32);
	write(1, "\n", 1);
}

 void		mlc_segment(t_blk *blk, t_mlc *mlc, t_mlc *prev, size_t size)
{
	t_mlc		*next;
	t_mlc		*segment;
	size_t		offset;
	size_t		blk_min_size;

	next = mlc->next;
	(void)segment;
	(void)prev;
	write(1, "segment\n", 8);
	putaddr(mlc);
	putaddr(next);
	offset = ((void*)next) - ((void*)mlc + sizeof(t_mlc));
	putnbr(offset);
	if (blk->block_type == TINY)
		blk_min_size = sizeof(t_mlc) + 1;
	else
		blk_min_size = sizeof(t_mlc) + TINY_MAX;
	offset -= size;
	write(1, "post segment\n", 13);
	if (offset >= blk_min_size)
	{
		segment = (t_mlc*)((void*)mlc + size);
		segment->in_use = 0;
		mlc->next = segment;
		segment->next = next;
		if (next)
		{
			next->prev = segment;
			next->checksum = hash_mlc(next);
		}
		segment->prev = mlc;
		segment->checksum = hash_mlc(segment);
		if (prev)
		{
			prev->next_free = segment;
			prev->checksum = hash_mlc(prev);
		}
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
		if (size < (size_t)(current->next - current))
		{
			current->in_use = 1;
			if (prev)
				prev->next_free = current->next_free;
			else
				blk->first_free = current->next_free;
			mlc_segment(blk, current, prev, size);
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
	write(1, "malloc\n", 7);
	putaddr(mlc->next);
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
