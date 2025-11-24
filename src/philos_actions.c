/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:57:34 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/24 19:44:46 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking_action(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_lock);
	if (!is_dead(philo))
		printf("%ld\t%d\tis thinking\n", \
			get_time(philo->table).rel_ms, philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
}

void	pickup_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		take_a_fork(philo, "right");
		if (philo->has_right)
			printf("%ld\t%d\thas taken a right fork\n", \
				get_time(philo->table).rel_ms, philo->id);
		take_a_fork(philo, "left");
		if (philo->has_left)
			printf("%ld\t%d\thas taken a left fork\n", \
				get_time(philo->table).rel_ms, philo->id);
	}
	else
	{
		ft_usleep(1, philo->table);
		take_a_fork(philo, "left");
		if (philo->has_left)
			printf("%ld\t%d\thas taken a left fork\n", \
				get_time(philo->table).rel_ms, philo->id);
		take_a_fork(philo, "right");
		if (philo->has_right)
			printf("%ld\t%d\thas taken a right fork\n", \
				get_time(philo->table).rel_ms, philo->id);
	}
	if (is_dead(philo))
		drop_forks(philo);
}

void	eating_action(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_lock);
	philo->last_meal = get_time(philo->table).rel_ms;
	philo->meals++;
	pthread_mutex_unlock(&philo->table->state_lock);
	pthread_mutex_lock(&philo->table->print_lock);
	if (!is_dead(philo))
		printf("%ld\t%d\tis eating\n", \
			get_time(philo->table).rel_ms, philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	ft_usleep(philo->table->time_to_eat, philo->table);
}

void	drop_forks(t_philo *philo)
{
	if (philo->has_left)
		drop_a_fork(philo, "left");
	if (philo->has_right)
		drop_a_fork(philo, "right");
}

void	sleeping_action(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_lock);
	if (!is_dead(philo))
		printf("%ld\t%d\tis sleeping\n", \
			get_time(philo->table).rel_ms, philo->id);
	pthread_mutex_unlock(&philo->table->print_lock);
	ft_usleep(philo->table->time_to_sleep, philo->table);
}
