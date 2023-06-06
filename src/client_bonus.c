#include "../include/minitalk_bonus.h"

static int	ft_atoi(const char *str)
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
	return (result * sign);
}

void	sendBitMessageToServer(pid_t serverPid, const char *message)
{
	int		i;
	char	ch;
	int		j;
	int		len;

	len = strlen(message);
	if (len >= 1000)
		exit(ft_printf("Message too long (max. 1000 characters)\n"));
	i = 0;
	while (i <= len)
	{
		ch = message[i];
		j = 0;
		while (j < 8)
		{
			if (((ch >> j) & 1) == 1)
				kill(serverPid, SIGUSR1);
			else
				kill(serverPid, SIGUSR2);
			usleep(100);
			j++;
		}
		i++;
	}
}

void	printReceivedMessage(int sig)
{
	if (sig == SIGUSR1)
	{
		ft_printf("Message Received\n");
		system("leaks client_bonus");
		exit(0);
	}
	else if (sig == SIGUSR2)
	{
		ft_printf("Error\n");
		exit(1);
	}
}

int	main(int argc, char **argv)
{
	int	pid;

	signal(SIGUSR1, printReceivedMessage);
	if (argc == 3)
	{
		pid = ft_atoi(argv[1]);
		sendBitMessageToServer(pid, argv[2]);
	}
	usleep(100);
	return (0);
}
