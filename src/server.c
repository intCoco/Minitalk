/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chuchard <chuchard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 17:37:03 by chuchard          #+#    #+#             */
/*   Updated: 2023/06/13 17:58:09 by chuchard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minitalk.h"

unsigned char	*ft_mt_strjoin(unsigned char *s1, char c)
{
	unsigned char	*s3;
	int				i;

	i = 0;
	while (s1[i])
		i++;
	s3 = (unsigned char *)malloc(i + 2);
	i = 0;
	if (!s3)
		return (NULL);
	while (s1[i] != '\0')
	{
		s3[i] = s1[i];
		i++;
	}
	s3[i++] = c;
	s3[i] = '\0';
	free(s1);
	return (s3);
}

void	needreset(unsigned char *message, int *newPid, int bitindex)
{
	static int	pid = 0;
	int			i;

	i = 0;
	while (message[i])
		i++;
	if (*newPid != pid && *newPid != 0)
	{
		ft_memset(message, 0, i);
		bitindex = 0;
		pid = *newPid;
	}
}

void	bit2message(int signal, siginfo_t *sa, void *whatever)
{
	static unsigned char	*message;
	static unsigned char	c = 0;
	static int				bitindex = 0;

	(void)whatever;
	if (!message)
		message = ft_calloc(sizeof(char), 1);
	if (!message)
		exit(ft_printf("MALLOC ERROR"));
	needreset(message, &sa->si_pid, bitindex);
	if (signal == SIGUSR1)
		c |= (1 << bitindex);
	if (++bitindex >= 16)
	{
		message = ft_mt_strjoin(message, c);
		if (c == '\0')
		{
			ft_printf("[%d] New message : \"%s\"\n", getpid(), message);
			ft_memset(message, 0, sizeof(message));
		}
		else
			c = 0;
		bitindex = 0;
		kill(sa->si_pid, SIGUSR2);
	}
}

int	main(void)
{
	struct sigaction	sa;

	ft_printf("Server PID : %d\n", getpid());
	sa.sa_sigaction = bit2message;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		sleep(1);
	return (0);
}
