/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kebris-c <kebris-c@student.42madrid.c      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 17:51:21 by kebris-c          #+#    #+#             */
/*   Updated: 2025/11/25 15:30:01 by kebris-c         ###   ########.fr       */
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
static int	args_parser(t_table *table, int argc, char **argv)
{
	int		i;

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
	table->finished = 0;
	pthread_mutex_init(&table->print_lock, NULL);
	pthread_mutex_init(&table->state_lock, NULL);
	table->forks = malloc(sizeof(*table->forks) * (table->n_philos));
	if (!table->forks)
		return (exit_fail("malloc", 0, table));
	i = 0;
	while (i < table->n_philos)
		pthread_mutex_init(&table->forks[i++], NULL);
	table->start_time = get_time(table).abs_ms;
	return (EXIT_SUCCESS);
}

static int	init_philos(t_table *table)
{
	int		i;
	t_time	time;

	table->philos = malloc(sizeof(*table->philos) * (table->n_philos));
	if (!table->philos)
		return (exit_fail("malloc", 0, table));
	i = 0;
	time = get_time(table);
	while (i < table->n_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals = 0;
		table->philos[i].has_left = 0;
		table->philos[i].has_right = 0;
		table->philos[i].last_meal = time.rel_ms;
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->n_philos];
		pthread_mutex_init(&table->philos[i].lock, NULL);
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
		if (table->n_philos > 50)
			usleep(500);
		else
			usleep(100);
		i++;
	}
	if (pthread_create(&monitor, NULL, &monitor_routine, table) != 0)
		return (exit_fail("pthread_create monitor", i - 1, table));
	i = 0;
	while (i < table->n_philos)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	return (EXIT_SUCCESS);
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
