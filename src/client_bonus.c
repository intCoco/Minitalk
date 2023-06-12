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
	if (str[i] != '\0')
		exit(ft_printf("ERROR ARGUMENT"));
	return (result * sign);
}

void	sendBitMessageToServer(pid_t serverPid, unsigned char *message)
{
	int		i;
	char	ch;
	int		j;
	int		len;

	len = strlen((const char *)message);
	if (len >= 10000)
		exit(ft_printf("Message too long (%d/10000 characters)\n", len));
	i = 0;
	while (i <= len)
	{
		ch = message[i];
		j = 0;
		while (j < 16)
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
		exit(0);
	}
}

int	main(int argc, char **argv)
{
	int	pid;

	signal(SIGUSR1, printReceivedMessage);
	if (argc == 3)
	{
		pid = ft_atoi(argv[1]);
		if (pid <= 0)
			return (ft_printf("ERROR ARGUMENT"));
		sendBitMessageToServer(pid, (unsigned char *)argv[2]);
	}
	usleep(1000);
	return (ft_printf("ERROR ARGUMENT"));
}
