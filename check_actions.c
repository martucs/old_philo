/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:58:10 by martalop          #+#    #+#             */
/*   Updated: 2024/11/25 00:27:25 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_alive(t_philo *philo)
{
	int	res;

	res = 0;
	if (!philo)
		return (-1);
	pthread_mutex_lock(&philo->check_mutex);
	if (!philo->life_status)
		res = 0;
	else
		res = 1;
	pthread_mutex_unlock(&philo->check_mutex);
	return (res);
}

int	is_full(t_philo *philo)
{
	int	full;

	full = 0;
	if (philo && philo->info->min_meals)
	{
		pthread_mutex_lock(&philo->check_mutex);
		full = philo->meal_num >= philo->info->min_meals;
		pthread_mutex_unlock(&philo->check_mutex);
	}
	return (full);
}

int	kill_all_philos(t_philo *philos)
{
	while (philos)
	{
		pthread_mutex_lock(&philos->check_mutex);
		philos->life_status = 0;
		pthread_mutex_unlock(&philos->check_mutex);
		philos = philos->next;
	}
	return (0);
}

int	check_meals(t_philo *philo, int *full_philos,
		t_philo *tmp, unsigned long *time_passed)
{
	pthread_mutex_lock(&philo->check_mutex);
	*time_passed = game_time(philo->info->start_time) - philo->last_meal_time;
	pthread_mutex_unlock(&philo->check_mutex);
	if (*time_passed > (philo->info->time_to_die))
	{
		pthread_mutex_lock(philo->print_mutex);
		printf("%lu %d died\n", game_time(philo->info->start_time), philo->id);
		kill_all_philos(tmp);
		pthread_mutex_unlock(philo->print_mutex);
		return (1);
	}
	if (is_full(philo))
		*full_philos = *full_philos + 1;
	return (0);
}

int	check_natural_death(int full_philos, t_philo *tmp)
{
	if (tmp && full_philos == tmp->info->philo_num)
	{
		pthread_mutex_lock(tmp->print_mutex);
		kill_all_philos(tmp);
		pthread_mutex_unlock(tmp->print_mutex);
		return (1);
	}
	return (0);
}
