/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:57:34 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/03 20:25:30 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking_action(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%ld %d is thinking\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
}

void	pickup_forks(t_philo *philo)
{
	long	ts;

	ts = get_timestamp(philo->table);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d has taken a fork\n", ts, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d has taken a fork\n", ts, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d has taken a fork\n", ts, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld %d has taken a fork\n", ts, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
}

void	eating_action(t_philo *philo)
{
	t_timeval	tv;
	long		abs_time;

	pthread_mutex_lock(&philo->table->print_lock);
	printf("%ld %d is eating\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	gettimeofday(&tv, NULL);
	abs_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	pthread_mutex_lock(&philo->table->state_lock);
	philo->last_meal = abs_time;
	philo->meals++;
	pthread_mutex_unlock(&philo->table->state_lock);
	usleep(philo->table->time_to_eat * 1000);
}

void	drop_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	sleeping_action(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_lock);
	printf("%ld %d is sleeping\n", get_timestamp(philo->table), philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	usleep(philo->table->time_to_sleep * 1000);
}
