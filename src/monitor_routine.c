/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 20:51:44 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/24 19:22:45 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	dead_checker(t_table *table)
{
	int		i;
	long	last_meal;

	i = 0;
	while (i < table->n_philos)
	{
		pthread_mutex_lock(&table->state_lock);
		last_meal = table->philos[i].last_meal;
		if (get_time(table).rel_ms - last_meal > table->time_to_die)
		{
			table->finished = 1;
			pthread_mutex_unlock(&table->state_lock);
			pthread_mutex_lock(&table->print_lock);
			printf("%ld\t%d\tdied\n", get_time(table).rel_ms, \
					table->philos[i].id);
			pthread_mutex_unlock(&table->print_lock);
			return (true);
		}
		pthread_mutex_unlock(&table->state_lock);
		i++;
	}
	return (false);
}

static int	food_is_gone(t_table *table)
{
	int			i;
	int			meals;

	pthread_mutex_lock(&table->state_lock);
	i = 0;
	while (i < table->n_philos)
	{
		meals = table->philos[i].meals;
		if (meals < table->must_eat)
		{
			pthread_mutex_unlock(&table->state_lock);
			return (false);
		}
		i++;
	}
	pthread_mutex_unlock(&table->state_lock);
	pthread_mutex_lock(&table->print_lock);
	printf("%ld Simulation ended: all philosophers ate %d times\n", \
			get_time(table).rel_ms, table->must_eat);
	pthread_mutex_unlock(&table->print_lock);
	pthread_mutex_lock(&table->state_lock);
	table->finished = 1;
	pthread_mutex_unlock(&table->state_lock);
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_table		*table;

	table = (t_table *)arg;
	while (1)
	{
		pthread_mutex_lock(&table->state_lock);
		if (table->finished)
			return (pthread_mutex_unlock(&table->state_lock), NULL);
		pthread_mutex_unlock(&table->state_lock);
		if (dead_checker(table))
			return (NULL);
		if (table->must_eat != -1 && food_is_gone(table))
			return (NULL);
	}
	return (NULL);
}
