/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 22:48:27 by martalop          #+#    #+#             */
/*   Updated: 2024/11/25 00:27:07 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(t_philo *philos, t_info *info)
{
	int		i;
	t_philo	*tmp;

	i = 0;
	tmp = philos;
	while (i < info->philo_num && tmp)
	{
		pthread_mutex_destroy(&info->fork_array[i]);
		pthread_mutex_destroy(&tmp->check_mutex);
		tmp = tmp->next;
		i++;
	}
	pthread_mutex_destroy(&info->print_mutex);
}

int	create_first_philo(t_philo **philos)
{
	*philos = malloc(sizeof(t_philo) * 1);
	if (!*philos)
		return (1);
	(*philos)->life_status = 1;
	(*philos)->meal_num = 0;
	(*philos)->last_meal_time = 0;
	(*philos)->id = 0;
	(*philos)->next = NULL;
	return (0);
}

int	create_philo(t_philo **philos)
{
	t_philo	*aux;

	aux = *philos;
	if (!*philos)
	{
		if (create_first_philo(philos) == 1)
			return (2);
	}
	else
	{
		while (aux->next)
			aux = aux->next;
		aux->next = malloc(sizeof(t_philo) * 1);
		if (!aux->next)
			return (2);
		aux->next->life_status = 1;
		aux->next->meal_num = 0;
		aux->next->last_meal_time = 0;
		aux->next->id = 0;
		aux->next->next = NULL;
	}
	return (0);
}

int	init_args(char **argv, t_info *info)
{
	int	i;

	i = 0;
	info->philo_num = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (second_arg_check(info, argv[5]) == 1)
		return (1);
	info->start_time = 0;
	info->fork_array = malloc(sizeof(pthread_mutex_t) * info->philo_num);
	if (!info->fork_array)
		return (1);
	while (i < info->philo_num)
	{
		pthread_mutex_init(&info->fork_array[i], NULL);
		i++;
	}
	return (0);
}

int	parsing(int argc, char **argv)
{
	if (argc == 1)
	{
		write(2, "Input:\n\t./philo  philo_num  time_to_die ", 40);
		write(2, " time_to_eat  time_to_sleep  [meals_to_die]\n", 44);
		return (1);
	}
	if (argc > 6)
	{
		write(2, "wrong input: too many args\n", 27);
		return (1);
	}
	if (argc <= 4)
	{
		write(2, "wrong input: too few args\n", 26);
		return (1);
	}
	if (check_args(argv) == 1)
	{
		write(2, "wrong input: non valid args\n", 28);
		return (1);
	}
	return (0);
}
