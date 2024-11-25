/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 21:19:20 by martalop          #+#    #+#             */
/*   Updated: 2024/11/25 00:55:41 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_info
{
	int					philo_num;
	unsigned int		time_to_die;
	int					time_to_sleep;
	int					time_to_eat;
	int					min_meals;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		*fork_array;
	unsigned long		start_time;
}	t_info;

typedef struct s_philo
{
	pthread_t			thread;
	pthread_mutex_t		*print_mutex;
	pthread_mutex_t		check_mutex;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	unsigned long		last_meal_time;
	int					meal_num;
	int					life_status;
	int					id;
	t_info				*info;
	struct s_philo		*next;
}	t_philo;

void			*life_cycle(void *philos_r);
void			my_sleep(t_philo *philos, unsigned long wait_time);
unsigned long	current_time(void);
unsigned long	game_time(unsigned long start_time);
int				ft_atoi(const char *str);
int				check_args(char **argv);
int				parsing(int argc, char **argv);
int				second_arg_check(t_info *info, char *last_arg);
void			free_philo_lst(t_philo **philo_lst);
void			free_and_destroy_info(t_info *info);
void			destroy_forks(t_philo *philos, t_info *info);
int				create_first_philo(t_philo **philos);
int				create_philo(t_philo **philos);
int				init_args(char **argv, t_info *info);
void			init_philos(t_philo *philos, t_info *info);
int				is_alive(t_philo *philo);
int				is_full(t_philo *philo);
int				check_natural_death(int full_philos, t_philo *tmp);
int				check_meals(t_philo *philo, int *full_philos, t_philo *tmp,
					unsigned long *time_passed);
int				kill_all_philos(t_philo *philos);
void			handle_even_delay(t_info *info);

#endif
