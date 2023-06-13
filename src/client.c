/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuchard <chuchard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:36:57 by chuchard          #+#    #+#             */
/*   Updated: 2023/06/13 18:07:48 by chuchard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minitalk.h"

int	g_var;

static int	ft_mt_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result *= 10;
		result += str[i] - '0';
		i++;
	}
	if (str[i] != '\0')
		exit(ft_printf("ERROR ARGUMENT"));
	return (result * sign);
}

void	sendbitmessagetoserver(pid_t server_pid, unsigned char *message)
{
	int		i;
	int		j;
	int		len;

	len = ft_strlen((const char *)message);
	i = -1;
	while (++i <= len)
	{
		j = -1;
		while (++j < 16)
		{
			if (((message[i] >> j) & 1) == 1)
			{
				if (kill(server_pid, SIGUSR1) < 0)
					exit(ft_printf("ERROR ARGUMENT"));
			}
			else
			{
				if (kill(server_pid, SIGUSR2) < 0)
					exit(ft_printf("ERROR ARGUMENT"));
			}
			usleep(50);
		}
		while (g_var != 1)
			usleep(50);
		g_var = 0;
	}
}

void	receivedchar(int sig)
{
	if (sig == SIGUSR2)
		g_var = 1;
}

int	main(int argc, char **argv)
{
	int	pid;

	signal(SIGUSR2, receivedchar);
	if (argc == 3)
	{
		pid = ft_mt_atoi(argv[1]);
		if (pid <= 0)
			return (ft_printf("ERROR ARGUMENT"));
		sendbitmessagetoserver(pid, (unsigned char *)argv[2]);
	}
	else
		return (ft_printf("ERROR ARGUMENT"));
	return (0);
}
