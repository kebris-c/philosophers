/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:51:21 by kebris-c          #+#    #+#             */
/*   Updated: 2025/12/02 18:48:27 by kebris-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Args:
 *	number_of_philosophers | time_to_die | time_to_eat | time_to_sleep 
 *	ex:
 *		./philo 20 200 5 15
 *	Addidionaly, can be added [number_of_times_each_philosopher_must_eat]:
 *		./philo 20 200 5 15 7
*/
/*void	print_dbg(t_philo *philo)
{
	long	now;
	long	last_meal;

	pthread_mutex_lock(&philo->meal_lock);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_lock);
	now = get_time(philo->table).rel_ms;
	pthread_mutex_lock(&philo->table->print_lock);
	printf("[DBG] philo %d: now=%ld last=%ld diff=%ld die=%ld\n", \
		philo->id, now, last_meal, now - last_meal, \
		philo->table->time_to_die);
	pthread_mutex_unlock(&philo->table->print_lock);
}*/

static int	args_parser(t_table *table, int argc, char **argv)
{
	int			i;

	table->n_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	if (table->n_philos < 1 || table->time_to_die < 1 \
		|| table->time_to_eat < 1 || table->time_to_sleep < 1)
		return (exit_fail("Bad args", 0, table));
	if (argc == 6)
		table->must_eat = ft_atoi(argv[5]);
	else
		table->must_eat = -1;
	pthread_mutex_init(&table->print_lock, NULL);
	pthread_mutex_init(&table->state_lock, NULL);
	table->forks = malloc(sizeof(*table->forks) * (table->n_philos));
	if (!table->forks)
		return (exit_fail("malloc", 0, table));
	i = 0;
	while (i < table->n_philos)
		pthread_mutex_init(&table->forks[i++], NULL);
	return (EXIT_SUCCESS);
}

static int	init_philos(t_table *table)
{
	int		i;
	long	now;

	table->philos = malloc(sizeof(*table->philos) * (table->n_philos));
	if (!table->philos)
		return (exit_fail("malloc", 0, table));
	now = get_time(table).rel_ms;
	pthread_mutex_lock(&table->state_lock);
	table->start_time = now;
	pthread_mutex_unlock(&table->state_lock);
	i = 0;
	while (i < table->n_philos)
	{
		memset(&table->philos[i], 0, sizeof(*table->philos));
		table->philos[i].id = i + 1;
		table->philos[i].table = table;
		table->philos[i].last_meal = get_time(table).rel_ms;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->n_philos];
		pthread_mutex_init(&table->philos[i].meal_lock, NULL);
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	philosophers(t_table *table)
{
	int			i;
	t_thread	monitor;

	i = 0;
	while (i < table->n_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL, &philos_routine, \
				&table->philos[i]) != 0)
			return (exit_fail("pthread_create philo", i, table));
		i++;
	}
	if (pthread_create(&monitor, NULL, &monitor_routine, table) != 0)
		return (exit_fail("pthread_create monitor", i - 1, table));
	i = 0;
	while (i < table->n_philos)
		pthread_join(table->philos[i++].thread, NULL);
	return (pthread_join(monitor, NULL), EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
		return (exit_fail("Bad arguments", 0, &table));
	memset(&table, 0, sizeof(table));
	if (args_parser(&table, argc, argv) == EXIT_FAILURE)
		return (free_table(&table), EXIT_FAILURE);
	if (init_philos(&table) == EXIT_FAILURE)
		return (free_table(&table), EXIT_FAILURE);
	printf("Format:\n\ttimestamp_in_ms X action\n\nOutput:\n");
	if (philosophers(&table) == EXIT_FAILURE)
		return (free_table(&table), EXIT_FAILURE);
	return (free_table(&table), EXIT_SUCCESS);
}
