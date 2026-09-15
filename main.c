/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 16:37:15 by drezan            #+#    #+#             */
/*   Updated: 2026/09/15 15:06:50 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

int main()
{
	printf("Starting program...\n");
	t_coder **coders = init_coders(10);
	printf("coders initialized.\n");
	
	for (int i = 0; i < 10; i++)
	{
		coder_run(coders[i]);
	}
	return 0;
}
