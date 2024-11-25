/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_cycle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:31:51 by martalop          #+#    #+#             */
/*   Updated: 2024/11/25 00:55:59 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	grab_forks(t_philo *philos)
{
	pthread_mutex_lock(philos->right_fork);
	pthread_mutex_lock(philos->print_mutex);
	if (is_alive(philos))
		printf("%lu %d has taken a fork\n",
			game_time(philos->info->start_time), philos->id);
	pthread_mutex_unlock(philos->print_mutex);
	while (philos->info->philo_num == 1)
	{
		if (!is_alive(philos))
		{
			pthread_mutex_unlock(philos->right_fork);
			return (1);
		}
	}
	pthread_mutex_lock(philos->left_fork);
	pthread_mutex_lock(philos->print_mutex);
	if (is_alive(philos))
		printf("%lu %d has taken a fork\n",
			game_time(philos->info->start_time), philos->id);
	pthread_mutex_unlock(philos->print_mutex);
	return (0);
}

void	eat_and_drop_forks(t_philo *philos)
{
	pthread_mutex_lock(philos->print_mutex);
	if (is_alive(philos))
		printf("%lu %d is eating\n",
			game_time(philos->info->start_time), philos->id);
	pthread_mutex_unlock(philos->print_mutex);
	pthread_mutex_lock(&philos->check_mutex);
	philos->last_meal_time = game_time(philos->info->start_time);
	philos->meal_num += 1;
	pthread_mutex_unlock(&philos->check_mutex);
	my_sleep(philos, philos->info->time_to_eat);
	pthread_mutex_unlock(philos->left_fork);
	pthread_mutex_unlock(philos->right_fork);
}

void	go_sleep(t_philo *philos)
{
	pthread_mutex_lock(philos->print_mutex);
	if (is_alive(philos))
		printf("%lu %d is sleeping\n",
			game_time(philos->info->start_time), philos->id);
	pthread_mutex_unlock(philos->print_mutex);
	my_sleep(philos, philos->info->time_to_sleep);
}

void	think(t_philo *philos)
{
	pthread_mutex_lock(philos->print_mutex);
	if (is_alive(philos))
		printf("%lu %d is thinking\n",
			game_time(philos->info->start_time), philos->id);
	pthread_mutex_unlock(philos->print_mutex);
}

void	*life_cycle(void *philos_r)
{
	t_philo	*philos;

	philos = (t_philo *)philos_r;
	pthread_mutex_lock(&philos->info->print_mutex);
	pthread_mutex_unlock(&philos->info->print_mutex);
	if (philos->id % 2 == 0)
		handle_even_delay(philos->info);
	while (is_alive(philos))
	{
		if (grab_forks(philos) == 1)
			return (NULL);
		eat_and_drop_forks(philos);
		go_sleep(philos);
		think(philos);
	}
	return (philos);
}
