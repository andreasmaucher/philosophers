/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 22:11:02 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	main(int ac, char **av)
{
	t_philo			*philos;
	t_data		data;

	if (valid_input(av) || ac < 5 || ac > 6)
	{	
		printf("Invalid input. Accepted format:\n ./philo <n_philos> ");
		printf("<time_to_die> <time_to_eat> <time_to_sleep> <n_eat_times>\n");
		return(INPUT_ERR);
	}
	philos = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	if (!philos)
		return (MEM_ALLOC_ERROR);
	memset(&data, 0, sizeof(t_data));
	init_structs(philos, &data, av);
	create_threads(&data);
	free_and_destroy(SUCCESS, &data);
	return (0);
}