/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 20:42:58 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/03 21:20:20 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eternal_aux_loop(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->table->state_lock);
		if (philo->table->finished)
		{
			pthread_mutex_unlock(&philo->table->state_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->table->state_lock);
		thinking_action(philo);
		pthread_mutex_lock(&philo->table->state_lock);
		if (philo->table->finished)
		{
			pthread_mutex_unlock(&philo->table->state_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->table->state_lock);
		pickup_forks(philo);
		eating_action(philo);
		drop_forks(philo);
		sleeping_action(philo);
	}
}

void	*philos_routine(void *arg)
{
	t_philo	*philo;
	t_time	time;

	philo = (t_philo *)arg;
	time = get_time(philo->table);
	if (philo->table->n_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		printf("%ld %d has taken a fork\n", \
				time.timestamp, philo->id);
		usleep(philo->table->time_to_die * 1000);
		pthread_mutex_lock(&philo->table->state_lock);
		philo->table->finished = 1;
		pthread_mutex_unlock(&philo->table->state_lock);
		printf("%ld %d died\n", \
				time.timestamp, philo->id);
		return (pthread_mutex_unlock(philo->left_fork), NULL);
	}
	eternal_aux_loop(philo);
	return (NULL);
}
