/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:19:20 by martalop          #+#    #+#             */
/*   Updated: 2024/11/22 19:17:54 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_info
{
	int					philo_num;
	unsigned int		time_to_die;
	int					time_to_sleep;
	int					time_to_eat;
	int					min_meals;
	pthread_mutex_t 	print_mutex;
	pthread_mutex_t		*fork_array;
	unsigned long		start_time;
}	t_info;


typedef struct	s_philo
{
	pthread_t			thread;
	pthread_mutex_t 	*print_mutex;
	pthread_mutex_t 	check_mutex;
	pthread_mutex_t 	life_mutex;
	pthread_mutex_t 	*left_fork;
	pthread_mutex_t 	*right_fork; 
	unsigned long		last_meal_time; //esto es lo que voy a ir reseteando cada vez que coman
	int					meal_num;
	int					life_status;
	int					id;
	t_info				*info;
	struct s_philo		*next;
}	t_philo;

# endif
