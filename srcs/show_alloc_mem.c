/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 16:14:17 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 22:10:48 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static void	print_addr(void *addr)
{
	static char	*hex_charset = "0123456789abcdef";
	char		buff[16];
	size_t		addr_;
	size_t		i;

	addr_ = (size_t)addr;
	i = 16;
	while (addr_ && i--)
	{
		buff[i] = hex_charset[addr_ % 16];
		addr_ /= 16;
	}
	write(1, "0x", 2);
	write(1, buff + i, 16 - i);
}

static void	print_header_blk(t_blk *blk)
{
	if (blk->block_type == TINY)
		ft_putstr("TINY : ");
	else if (blk->block_type == SMALL)
		ft_putstr("SMALL : ");
	else
		ft_putstr("LARGE : ");
	print_addr(blk);
	ft_putstr(" - ");
	print_addr((void*)blk + blk->size);
	ft_putstr("\n");
}

static void	print_mlc(t_mlc *mlc)
{
	void	*mlc_ptr;

	mlc_ptr = (void*)mlc + sizeof(*mlc);
	print_addr(mlc_ptr);
	ft_putstr(" - ");
	print_addr(mlc_ptr + mlc_size(mlc));
	ft_putstr(" : ");
	ft_putsizet(mlc_size(mlc));
	ft_putstr(" octets\n");
}

static void	print_blk(t_blk *blk)
{
	t_mlc	*mlc;

	mlc = (void*)blk + sizeof(*blk);
	print_header_blk(blk);
	while (mlc < blk->wilderness)
	{
		if (mlc->in_use)
			print_mlc(mlc);
		mlc = mlc->next;
	}
}

void		show_alloc_mem(void)
{
	t_blk	*curr_blk;

	curr_blk = g_blks;
	while (curr_blk)
	{
		print_blk(curr_blk);
		curr_blk = curr_blk->next;
	}
}
