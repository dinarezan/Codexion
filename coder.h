/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:06:48 by drezan            #+#    #+#             */
/*   Updated: 2026/09/17 15:50:25 by drezan           ###   ########.fr       */
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

t_coder				**init_coders(t_sim_param *sim_param);
void				*coder_run(void *sim_coder);

#endif
