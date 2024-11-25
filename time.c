/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:36:22 by martalop          #+#    #+#             */
/*   Updated: 2024/11/25 00:50:20 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_even_delay(t_info *info)
{
	int	delay;

	delay = 0;
	if (((unsigned int)info->time_to_eat > info->time_to_die)
		|| ((unsigned int)info->time_to_sleep > info->time_to_die))
		delay = (5000);
	else
		delay = (info->time_to_eat * 1000 / 2);
	usleep((delay));
}

void	my_sleep(t_philo *philos, unsigned long wait_time)
{
	unsigned long	first_time;

	first_time = current_time();
	while (is_alive(philos) && ((current_time() - first_time) < wait_time))
		usleep(500);
}

unsigned long	current_time(void)
{
	struct timeval	tmp;
	unsigned long	current_time;

	gettimeofday(&tmp, NULL);
	current_time = ((unsigned long)tmp.tv_sec * 1000) + (tmp.tv_usec * 0.001);
	return (current_time);
}

unsigned long	game_time(unsigned long start_time)
{
	return (current_time() - start_time);
}
