/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 22:19:32 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 22:29:35 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

static t_lock	lock;

void				malloc_lock(void)
{
	if (lock.init == 0)
	{
		pthread_mutex_init(&lock.plock, NULL);
		lock.init = 1;
	}
	pthread_mutex_lock(&lock.plock);
}

void				malloc_unlock(void)
{
	if (lock.init == 0)
	{
		pthread_mutex_init(&lock.plock, NULL);
		lock.init = 1;
	}
	pthread_mutex_unlock(&lock.plock);
}
