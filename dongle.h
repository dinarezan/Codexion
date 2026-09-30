/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:14:57 by drezan            #+#    #+#             */
/*   Updated: 2026/09/30 15:39:46 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H

# define DONGLE_H

# include "simulation.h"
# include <pthread.h>

typedef struct s_heap
{
	t_coder			**coders;
	int				size;
	int				capacity;
	int				key;
}					t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	dongle;
	pthread_cond_t	condition;
	struct timeval	last_compile;
	t_heap			*queue;
}					t_dongle;

t_dongle			**init_dongles(t_sim_param *sim_param);
void				free_dongles(t_dongle **dongles);
t_heap				*create_heap(int capacity, int key);
void				swap(t_coder *a, t_coder *b);
void				heapify(t_heap *heap, int i);
void				insert_heap(t_heap *heap, t_coder *c);
t_coder				*pop_min_from_heap(t_heap *heap);

#endif