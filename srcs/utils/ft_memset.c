/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:42:10 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 14:42:35 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*c_dst;
	unsigned char	u_c;

	c_dst = (unsigned char *)b;
	u_c = (unsigned char)c;
	while (len--)
		*c_dst++ = u_c;
	return (b);
}
