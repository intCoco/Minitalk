#include "../include/minitalk.h"

#define MESSAGE_SIZE 1000

void	bitToMessage(int signal)
{
	static unsigned char	message[MESSAGE_SIZE];
	static int				bitIndex = 0;
	static int				charIndex = 0;

	if (signal == SIGUSR1)
		message[charIndex] |= (1 << bitIndex);
	bitIndex++;
	if (bitIndex >= 8)
	{
		if (message[charIndex] == '\0')
		{
			ft_printf("Received message from client: %s\n", message);
			ft_memset(message, 0, sizeof(message));
			charIndex = 0;
		}
		else
			charIndex++;
		bitIndex = 0;
	}
}

int	main(int argc, char **argv)
{
	pid_t	pid;

	(void)argv;
	if (argc != 1)
	{
		ft_printf("Error\n");
		return (1);
	}
	pid = getpid();
	ft_printf("%d\n", pid);
	signal(SIGUSR1, bitToMessage);
	signal(SIGUSR2, bitToMessage);
	while (1)
		sleep(1);
	return (0);
}
