/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:57:34 by kebris-c          #+#    #+#             */
/*   Updated: 2025/12/02 19:35:11 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking_action(t_philo *philo)
{
	if (!is_dead(philo))
		print_action(philo->table, philo->id, "is thinking");
	ft_usleep(1, philo->table);
}

void	pickup_forks(t_philo *philo)
{
	int	should_reverse;
	int	start_left;

	should_reverse = philo->id == philo->table->n_philos;
	start_left = (philo->id % 2 == 1) ^ should_reverse;
	if (start_left)
	{
		take_a_fork(philo, 0);
		if (is_dead(philo))
			return (drop_forks(philo));
		take_a_fork(philo, 1);
	}
	else
	{
		take_a_fork(philo, 1);
		if (is_dead(philo))
			return (drop_forks(philo));
		take_a_fork(philo, 0);
	}
	if (is_dead(philo))
		drop_forks(philo);
}

void	eating_action(t_philo *philo)
{
	int		should_print;

	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time(philo->table).rel_ms;
	philo->meals++;
	should_print = !(philo->table->must_eat != -1 \
		&& philo->meals > philo->table->must_eat);
	pthread_mutex_unlock(&philo->meal_lock);
	if (!is_dead(philo) && should_print)
		print_action(philo->table, philo->id, "is eating");
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
	if (!is_dead(philo))
		print_action(philo->table, philo->id, "is sleeping");
	ft_usleep(philo->table->time_to_sleep, philo->table);
}
