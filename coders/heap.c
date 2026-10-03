/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 14:22:07 by drezan            #+#    #+#             */
/*   Updated: 2026/10/02 12:05:51 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "dongle.h"

t_heap	*create_heap(int capacity, int key)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	heap->size = 0;
	heap->capacity = capacity;
	heap->key = key;
	heap->coders = (t_coder **)malloc(capacity * sizeof(t_coder *));
	return (heap);
}

void	swap(t_heap *heap, int i, int j)
{
	t_coder	*temp;

	temp = heap->coders[i];
	heap->coders[i] = heap->coders[j];
	heap->coders[j] = temp;
}

void	heapify(t_heap *heap, int i)
{
	int			min;
	int			left;
	long long	deadline_left;
	long long	deadline_min;

	if (heap->key == 0 || heap->size < 2)
		return ;
	min = i;
	left = 2 * i + 1;
	deadline_left = heap->coders[left]->last_compile.tv_sec * 1000
		+ heap->coders[left]->last_compile.tv_usec / 1000
		+ heap->coders[left]->time_to_burnout;
	deadline_min = heap->coders[min]->last_compile.tv_sec * 1000
		+ heap->coders[min]->last_compile.tv_usec / 1000
		+ heap->coders[min]->time_to_burnout;
	if (left < heap->size && deadline_left < deadline_min)
		min = left;
	if (min != i)
		swap(heap, i, min);
}

void	insert_heap(t_heap *heap, t_coder *c)
{
	int	i;

	if (heap->size == heap->capacity)
	{
		printf("Heap overflow\n");
		return ;
	}
	heap->size++;
	i = heap->size - 1;
	heap->coders[i] = c;
	heapify(heap, 0);
}

t_coder	*pop_min_from_heap(t_heap *heap)
{
	t_coder	*root;

	if (heap->size <= 0)
		return (NULL);
	if (heap->size == 1)
	{
		heap->size--;
		return (heap->coders[0]);
	}
	root = heap->coders[0];
	heap->coders[0] = heap->coders[heap->size - 1];
	heap->size--;
	heapify(heap, 0);
	return (root);
}
