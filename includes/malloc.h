/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbraeckm <wbraeckm@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/11 14:05:32 by wbraeckm          #+#    #+#             */
/*   Updated: 2020/02/11 21:29:40 by wbraeckm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <sys/mman.h>
# include <sys/types.h>
# include <unistd.h>

# define TINY 0
# define TINY_MAX 128

/*
** This shouldn't be static in case getpagesize changes
*/

# define TINY_BLK_SIZE 32768
# define SMALL 1
# define SMALL_MAX 1024

/*
** Same
*/

# define SMALL_BLK_SIZE 262144
# define LARGE 2
# ifndef NULL
#  define NULL 0
# endif

typedef struct s_mlc	t_mlc;
typedef struct s_blk	t_blk;
typedef unsigned long	t_u64;

/*
** NOTE: structure is 32 bytes long
*/

struct			s_mlc
{
	t_mlc		*prev;
	int			in_use;
	int			checksum;
	t_mlc		*next;
	t_mlc		*next_free;
};

/*
** NOTE: header for each block
*/

struct			s_blk
{
	t_blk		*prev;
	int			block_type;
	int			checksum;
	t_mlc		*wilderness;
	t_mlc		*last;
	t_mlc		*first_free;
	size_t		num_alloc;
	size_t		size;
	size_t		available;
	t_blk		*next;
};

extern t_blk	*g_blks;

void			*malloc(size_t size);
void			free(void *ptr);
void			*realloc(void *ptr, size_t size);
void			*calloc(size_t count, size_t size);
void			show_alloc_mem(void);

int				hash_mlc(t_mlc *mlc);
int				hash_blk(t_blk *blk);
void			*ft_memset(void *b, int c, size_t len);
size_t			blk_size(int type, size_t size);
t_blk			*get_blk(int type, size_t size);
int				get_type(size_t size);
t_blk			*append_new_blk(int type, size_t size);
t_blk			*get_blk_from_addr(void *ptr);
size_t			mlc_size(t_mlc *mlc);
void			add_free_list(t_blk *blk, t_mlc *mlc);

#endif
