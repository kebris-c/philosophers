/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 20:42:58 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/25 19:27:26 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*aux_loop(t_philo *philo)
{
	while (1)
	{
		if (is_dead(philo))
			return (NULL);
		thinking_action(philo);
		pickup_forks(philo);
		if (is_dead(philo))
			return (drop_forks(philo), NULL);
		eating_action(philo);
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
	long	now;

	philo = (t_philo *)arg;
	if (philo->table->n_philos == 1)
	{
		take_a_fork(philo, 0);
		now = get_time(philo->table).rel_ms;
		pthread_mutex_lock(&philo->table->print_lock);
		printf("%ld\t%d\thas taken a left fork\n", now, philo->id);
		pthread_mutex_unlock(&philo->table->print_lock);
		ft_usleep(philo->table->time_to_die, philo->table);
		drop_a_fork(philo, 0);
		return (NULL);
	}
	if (philo->id % 2 != 0)
		ft_usleep(philo->table->time_to_eat / 10, philo->table);
	return (aux_loop(philo));
}
