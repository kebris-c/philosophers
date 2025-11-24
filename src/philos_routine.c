/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 20:42:58 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/24 18:57:52 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*aux_loop(t_philo *philo)
{
	while (1)
	{
		if (is_dead(philo))
			return (drop_forks(philo), NULL);
		thinking_action(philo);
		if (is_dead(philo))
			return (drop_forks(philo), NULL);
		pickup_forks(philo);
		if (is_dead(philo))
			return (drop_forks(philo), NULL);
		eating_action(philo);
		if (is_dead(philo))
			return (drop_forks(philo), NULL);
		drop_forks(philo);
		if (is_dead(philo))
			return (NULL);
		sleeping_action(philo);
	}
	return (NULL);
}

void	*philos_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->n_philos == 1)
	{
		take_a_fork(philo, "left");
		ft_usleep(philo->table->time_to_die, philo->table);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	if (philo->id % 2 != 0)
		ft_usleep(1, philo->table);
	return (aux_loop(philo));
}
