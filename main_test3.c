/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:03:04 by martalop          #+#    #+#             */
/*   Updated: 2024/11/22 19:16:21 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "philo.h"

void	print_philos(t_philo *philos)
{
	t_philo	*aux;

	aux = philos;
	while (aux)
	{
		printf("philo->id: %d, philo address: %p, philo->next: %p\n", aux->id, aux, aux->next);
		aux = aux->next;
	}
}

int	ft_atoi(const char *str)
{
	int	signo;
	long	num;
	int	i;

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
			return (0);
	}
	return (signo * num);
}

unsigned long	get_current_time(void)
{
	struct timeval	tmp;
	unsigned long	current_time;

	gettimeofday(&tmp, NULL);
	current_time = ((unsigned long)tmp.tv_sec * 1000) + (tmp.tv_usec * 0.001);
//	printf("current_time: %u mili\n", current_time);
	return (current_time);
}

unsigned long	get_sim_time(unsigned long start_time)
{
	return (get_current_time() - start_time);
}

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

int	is_full(t_philo *phill)
{
	int	full;

	full = 0;	
	if (phill && phill->info->min_meals)
	{
		pthread_mutex_lock(&phill->check_mutex);
		full = phill->meal_num >= phill->info->min_meals;
		pthread_mutex_unlock(&phill->check_mutex);
	}
	return (full);
}

void	my_sleep(t_philo *philos, unsigned long wait_time)
{
	unsigned long	first_time;

	first_time = get_current_time();
//	first_time = get_sim_time(philos->info->start_time);
//	printf("first_time: %lu\n", );
	while (is_alive(philos) && ((get_current_time() - first_time) < (wait_time / 1000)))
	{
	//	printf("philo id: %d\n", philos->id);
		usleep(300);
	}
}

/*void	my_sleep(t_philo *philos)
{
	int	timePasted = 0;

	while (timePasted < philos->info->time_to_sleep / 1000)
	{
//		printf("time passed: %d\n", timePasted);
//		printf("time to sleep: %d\n", philos->info->time_to_sleep / 1000);
		timePasted++;
		usleep(1000);
	}
}*/

void	*life_cycle(void *philos_r)
{
	t_philo	*philos;

	philos = (t_philo *)philos_r;
	pthread_mutex_lock(&philos->info->print_mutex);
	pthread_mutex_unlock(&philos->info->print_mutex);
	if (philos->id % 2 == 0 /* && philos->info->philo_num % 2 == 0*/)
	{
		usleep(philos->info->time_to_eat / 2);
	//	usleep(150);
	}
	while (is_alive(philos))
	{
		/* PIKING 1sT FORC */
		pthread_mutex_lock(philos->right_fork); // first lock
		pthread_mutex_lock(philos->print_mutex); 
		if (is_alive(philos))
			printf("%lu %d has taken a fork\n", get_sim_time(philos->info->start_time), philos->id); 
		pthread_mutex_unlock(philos->print_mutex); 
		while (philos->info->philo_num == 1)
		{
			if (!is_alive(philos))
			{
				pthread_mutex_unlock(philos->right_fork);
				return (philos);
			}
		}
		/* PIKING 2sT FORC */
		pthread_mutex_lock(philos->left_fork); // second lock
		pthread_mutex_lock(philos->print_mutex); 
		if (is_alive(philos))
			printf("%lu %d has taken a fork\n", get_sim_time(philos->info->start_time), philos->id); 
		pthread_mutex_unlock(philos->print_mutex); 

		/*	iting	*/
		pthread_mutex_lock(philos->print_mutex); 
		if (is_alive(philos))
			printf("%lu %d is eating\n", get_sim_time(philos->info->start_time), philos->id); 
		pthread_mutex_unlock(philos->print_mutex); 
		
		pthread_mutex_lock(&philos->check_mutex);
		philos->last_meal_time = get_sim_time(philos->info->start_time);
		philos->meal_num += 1;
		pthread_mutex_unlock(&philos->check_mutex);
	//	my_sleep(philos);
		if (is_alive(philos))
			usleep(philos->info->time_to_eat);
	
	//	pthread_mutex_lock(&philos->check_mutex);
	//	philos->last_meal_time = get_sim_time(philos->info->start_time);
	//	philos->meal_num += 1;
	//	pthread_mutex_unlock(&philos->check_mutex);

	/*	pthread_mutex_lock(philos->print_mutex); 
		if (is_alive(philos))
			printf("%lu %d is droping forks\n", get_sim_time(philos->info->start_time), philos->id); 
		pthread_mutex_unlock(philos->print_mutex); */
		pthread_mutex_unlock(philos->left_fork); 
		pthread_mutex_unlock(philos->right_fork);  // unlocked both fork mutexes

		/*	driming	*/
		pthread_mutex_lock(philos->print_mutex); 
		if (is_alive(philos))
			printf("%lu %d is sleeping\n", get_sim_time(philos->info->start_time), philos->id); 
		pthread_mutex_unlock(philos->print_mutex); 
	//	int	timePasted = 0;
//		while (is_alive(philos) && timePasted++ < philos->info->time_to_sleep)
//		if (is_alive(philos))
//			usleep(philos->info->time_to_sleep);
		if (is_alive(philos))
			my_sleep(philos, philos->info->time_to_sleep);

		/*	Thking	*/
		pthread_mutex_lock(philos->print_mutex); 
		if (is_alive(philos))
			printf("%lu %d is thinking\n", get_sim_time(philos->info->start_time), philos->id); 
		pthread_mutex_unlock(philos->print_mutex); 
	}
	return (philos);
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

int	check_philo_status(t_philo *philo)
{
	unsigned long	time_passed;
	int				full_philos;
	t_philo	*tmp;
	
	tmp = philo;
	while (1)
	{
		usleep(100);
		philo = tmp;
		full_philos = 0;
		time_passed = 0;
		while (philo)
		{
			pthread_mutex_lock(&philo->check_mutex);
			time_passed = get_sim_time(philo->info->start_time) - philo->last_meal_time;
			pthread_mutex_unlock(&philo->check_mutex);
	//		printf("philo %d current time: %lu\n", philo->id, get_current_time());
	//		printf("philo %d last_meal_time: %lu\n", philo->id, philo->last_meal_time);
	//		printf("philo %d time passed: %lu\n", philo->id, time_passed);
			if (time_passed > (philo->info->time_to_die / 1000)) 
			{
				pthread_mutex_lock(philo->print_mutex);
				printf("%lu %d died\n", get_sim_time(philo->info->start_time), philo->id);
				kill_all_philos(tmp);
				pthread_mutex_unlock(philo->print_mutex);
				return (1);
			}
			if (is_full(philo))
				full_philos++;
			philo = philo->next;
		}
		if (tmp && full_philos == tmp->info->philo_num)
		{
			pthread_mutex_lock(tmp->print_mutex);
			kill_all_philos(tmp);
			pthread_mutex_unlock(tmp->print_mutex);
			return (1);
		}
	}
	return (0);
}

void	start_threads(t_philo *philos, t_info *info)
{
	t_philo *aux;

	aux = philos;
	pthread_mutex_lock(&info->print_mutex);
	while (aux)
	{
		pthread_create(&aux->thread, NULL, life_cycle, aux);
		aux = aux->next;
	}
	info->start_time = get_current_time();
	pthread_mutex_unlock(&info->print_mutex);
	check_philo_status(philos);
//	if (check_philo_status(philos) == 1)
//		free_and_exit();
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
	//	printf("left fork for philo %d: %p\n", aux->id, aux->left_fork);
		aux->right_fork = &info->fork_array[i - 1];
	//	printf("right fork for philo %d: %p\n", aux->id, aux->right_fork);
		aux->print_mutex = &info->print_mutex;
		aux->info = info;
		pthread_mutex_init(&aux->check_mutex, NULL); 
		aux = aux->next;
		i++;
	}
	pthread_mutex_init(&info->print_mutex, NULL); 
}

int	create_philo(t_philo **philos)
{
	t_philo	*aux;

	aux = *philos;
	if (!*philos)
	{
		*philos = malloc(sizeof(t_philo) * 1);
		if (!*philos)
			return (2);
		(*philos)->life_status = 1;
		(*philos)->meal_num = 0;
		(*philos)->last_meal_time = 0;
		(*philos)->id = 0;
		(*philos)->next = NULL;
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
			// funct to free philo list before returning
			write(2, "malloc err\n", 11);
			return (NULL);
		}
		i++;
	}
	init_philos(philos, info);
	return (philos);
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

int	init_args(char **argv, t_info *info)
{
	int	i;

	i = 0;
	info->philo_num = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]) * 1000;
	info->time_to_eat = ft_atoi(argv[3]) * 1000;
	info->time_to_sleep = ft_atoi(argv[4]) * 1000;
	if (argv[5])
		info->min_meals = ft_atoi(argv[5]);
	else
		info->min_meals = 0;
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

void	destroy_forks(t_philo *philos, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->philo_num)
	{
		pthread_mutex_destroy(&info->fork_array[i]);
		pthread_mutex_destroy(&philos->check_mutex);
		i++;
	}
	pthread_mutex_destroy(&info->print_mutex);
}

int	parsing(int argc, char **argv)
{
	if (argc == 1)
	{
		write(2, "Input:\n\t./philo philo_num  time_to_die ", 39);
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
int	main(int argc, char **argv)
{
	t_info	info;
	t_philo	*philos;

	if (parsing(argc, argv) == 1)
	   return (1);
	init_args(argv, &info);
	philos = game_prep(&info);
	if (!philos)
		return (1);
	start_threads(philos, &info);
	destroy_forks(philos, &info);
	return (0);
}
