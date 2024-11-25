/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:03:04 by martalop          #+#    #+#             */
/*   Updated: 2024/11/25 00:57:36 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_philo_status(t_philo *philo)
{
	unsigned long	time_passed;
	int				full_philos;
	t_philo			*tmp;

	tmp = philo;
	while (1)
	{
		usleep(1000);
		philo = tmp;
		full_philos = 0;
		time_passed = 0;
		while (philo)
		{
			if (check_meals(philo, &full_philos, tmp, &time_passed) == 1)
				return (1);
			philo = philo->next;
		}
		if (check_natural_death(full_philos, tmp) == 1)
			return (1);
	}
	return (0);
}

void	start_game(t_philo *philos, t_info *info)
{
	t_philo	*aux;

	aux = philos;
	pthread_mutex_lock(&info->print_mutex);
	while (aux)
	{
		pthread_create(&aux->thread, NULL, life_cycle, aux);
		aux = aux->next;
	}
	info->start_time = current_time();
	pthread_mutex_unlock(&info->print_mutex);
	check_philo_status(philos);
	aux = philos;
	while (aux)
	{
		pthread_join(aux->thread, NULL);
		aux = aux->next;
	}
}

void	init_philos(t_philo *philos, t_info *info)
{
	t_philo	*aux;
	int		i;

	i = 1;
	aux = philos;
	while (aux)
	{
		aux->id = i;
		if (i == 1)
			aux->left_fork = &info->fork_array[info->philo_num - 1];
		else
			aux->left_fork = &info->fork_array[i - 2];
		aux->right_fork = &info->fork_array[i - 1];
		aux->print_mutex = &info->print_mutex;
		aux->info = info;
		pthread_mutex_init(&aux->check_mutex, NULL);
		aux = aux->next;
		i++;
	}
	pthread_mutex_init(&info->print_mutex, NULL);
}

t_philo	*game_prep(t_info *info)
{
	int			i;
	t_philo		*philos;

	philos = NULL;
	i = 0;
	while (i < info->philo_num)
	{
		if (create_philo(&philos) == 2)
		{
			free_philo_lst(&philos);
			write(2, "malloc err\n", 11);
			return (NULL);
		}
		i++;
	}
	init_philos(philos, info);
	return (philos);
}

int	main(int argc, char **argv)
{
	t_info	info;
	t_philo	*philos;

	if (parsing(argc, argv) == 1)
		return (1);
	if (init_args(argv, &info) == 1)
		return (1);
	philos = game_prep(&info);
	if (!philos)
	{
		free_and_destroy_info(&info);
		return (1);
	}
	start_game(philos, &info);
	destroy_forks(philos, &info);
	free_philo_lst(&philos);
	free(info.fork_array);
	return (0);
}
