/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 23:25:07 by martalop          #+#    #+#             */
/*   Updated: 2024/11/24 23:53:42 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int		signo;
	long	num;
	int		i;

	i = 0;
	num = 0;
	signo = 1;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			signo = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - '0');
		i++;
		if (signo * num > 2147483647)
			return (-1);
	}
	return (signo * num);
}

int	check_args(char **argv)
{
	int	x;
	int	i;

	x = 1;
	while (argv[x])
	{
		i = 0;
		while (argv[x][i])
		{
			if ((argv[x][i] < 48 || argv[x][i] > 57) || argv[x][i] == '-')
				return (1);
			i++;
		}
		if (!argv[x][i] && i == 0)
			return (1);
		x++;
	}
	return (0);
}

int	second_arg_check(t_info *info, char *last_arg)
{
	if (!info->philo_num || !info->time_to_die
		|| !info->time_to_eat || !info->time_to_sleep)
		return (write(2, "wrong input: 0 not allowed\n", 27), 1);
	if (info->philo_num == -1 || (int)info->time_to_die == -1
		|| info->time_to_eat == -1 || info->time_to_sleep == -1)
		return (write(2, "wrong input: int numbers in ms needed\n", 38), 1);
	if (last_arg)
	{
		info->min_meals = ft_atoi(last_arg);
		if (info->min_meals == -1)
			return (write(2, "wrong input: int numbers in ms needed\n", 38), 1);
		if (!info->min_meals)
			return (write(2, "wrong input: 0 not allowed\n", 27), 1);
	}
	else
		info->min_meals = 0;
	return (0);
}

void	free_philo_lst(t_philo **philo_lst)
{
	t_philo	*current;
	t_philo	*tmp;

	if (!*philo_lst)
		return ;
	current = *philo_lst;
	while (current != NULL)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	*philo_lst = NULL;
}

void	free_and_destroy_info(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->philo_num)
	{
		pthread_mutex_destroy(&info->fork_array[i]);
		i++;
	}
	free(info->fork_array);
	info->fork_array = NULL;
}
