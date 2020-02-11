/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:32:13 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 14:33:49 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

int	get_type(size_t size)
{
	if (size < TINY_MAX)
		return (TINY);
	else if (size < SMALL_MAX)
		return (SMALL);
	return (LARGE);
}
