/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:49:39 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/03 21:17:46 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <time.h>
# include <string.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct timeval	t_timeval;
typedef pthread_mutex_t	t_mutex;
typedef pthread_t		t_thread;

typedef struct s_table
{
	int				n_philos;
	int				must_eat;
	int				finished;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			start_time;
	t_mutex			*forks;
	t_mutex			print_lock;
	t_mutex			state_lock;
	struct s_philo	*philos;
}	t_table;

typedef struct s_philo
{
	int			id;
	int			meals;
	long		last_meal;
	t_mutex		*left_fork;
	t_mutex		*right_fork;
	t_table		*table;
	t_thread	thread;
}	t_philo;

typedef struct s_time
{
	t_timeval	tv;
	long		curr_time;
	long		timestamp;
}	t_time;

int		exit_fail(char *flag);
int		ft_atoi(const char *str);
void	thinking_action(t_philo *philo);
void	pickup_forks(t_philo *philo);
void	eating_action(t_philo *philo);
void	drop_forks(t_philo *philo);
void	sleeping_action(t_philo *philo);
void	free_table(t_table *table);
void	ft_putstr_fd(char *str, int std);
void	*monitor_routine(void *arg);
void	*philos_routine(void *arg);
long	ft_atol(const char *str);
t_time	get_time(t_table *table);

#endif
