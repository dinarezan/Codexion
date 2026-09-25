/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:41:12 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 17:41:47 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	set_last_compilation_time(t_coder **coders, t_sim_param *sim_param)
{
	int	i;

	i = 0;
	while (coders[i])
		coders[i++]->last_compile = sim_param->sim_start;
}

void	free_coders(t_coder **coders)
{
	int	i;

	i = 0;
	if (!coders)
		return ;
	while (coders[i])
	{
		free(coders[i]);
		i++;
	}
	free(coders);
}
