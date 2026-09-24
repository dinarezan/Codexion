/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:06:48 by drezan            #+#    #+#             */
/*   Updated: 2026/09/24 14:55:01 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H

# define CODER_H

# include "dongle.h"

typedef struct s_coder
{
	int				id;
	t_dongle		*left;
	t_dongle		*right;
	int				compile_count;
	struct timeval	last_compile;
}					t_coder;

t_coder				**init_coders(t_sim_param *sim_param, t_dongle **dongles);
void				acquire_both_dongles(t_coder *c, t_sim_param *param);
void				release_both_dongles(t_coder *c);
void				*coder_run(void *sim_coder);
void				free_coders(t_coder **coders);

#endif
