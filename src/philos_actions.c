/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:57:34 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/25 19:24:04 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking_action(t_philo *philo)
{
	long	now;

	if (!is_dead(philo))
	{
		now = get_time(philo->table).rel_ms;
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld\t%d\tis thinking\n", now, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
}

void	pickup_forks(t_philo *philo)
{
	long	now;

	if (philo->id % 2 == 0)
	{
		take_a_fork(philo, 1);
		take_a_fork(philo, 0);
	}
	else
	{
		ft_usleep(1, philo->table);
		take_a_fork(philo, 0);
		take_a_fork(philo, 1);
	}
	if (is_dead(philo))
		drop_forks(philo);
	else if (philo->has_left && philo->has_right)
	{
		now = get_time(philo->table).rel_ms;
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld\t%d\thas taken a right fork\n", now, philo->id);
		printf("%ld\t%d\thas taken a left fork\n", now, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
}

void	eating_action(t_philo *philo)
{
	long	now;

	now = get_time(philo->table).rel_ms;
	pthread_mutex_lock(&philo->lock);
	philo->last_meal = now;
	philo->meals++;
	pthread_mutex_unlock(&philo->lock);
	if (!is_dead(philo) && !(philo->table->must_eat != -1 \
			&& philo->meals > philo->table->must_eat))
	{
		now = get_time(philo->table).rel_ms;
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld\t%d\tis eating\n", now, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
	ft_usleep(philo->table->time_to_eat, philo->table);
}

void	drop_forks(t_philo *philo)
{
	if (philo->has_left)
		drop_a_fork(philo, 0);
	if (philo->has_right)
		drop_a_fork(philo, 1);
}

void	sleeping_action(t_philo *philo)
{
	long	now;

	if (!is_dead(philo))
	{
		now = get_time(philo->table).rel_ms;
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld\t%d\tis sleeping\n", now, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
	}
	ft_usleep(philo->table->time_to_sleep, philo->table);
}
