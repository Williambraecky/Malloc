/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:20:50 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 14:22:23 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ptr;
	char	*ptr2;
	size_t	len;

	len = count * size;
	if (!(ptr = malloc(len)))
		return (NULL);
	ptr2 = (char*)ptr;
	while (len--)
		*ptr2++ = 0;
	return (ptr);
}
