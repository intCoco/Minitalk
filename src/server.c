#include "../include/minitalk.h"

#define MESSAGE_SIZE 10000

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = -1;
	while (++i < len)
		((char *)b)[i] = (unsigned char)c;
	return (b);
}

void	bitToMessage(int signal)
{
	static unsigned char	message[MESSAGE_SIZE];
	static int				bitIndex = 0;
	static int				charIndex = 0;

	if (signal == SIGUSR1)
		message[charIndex] |= (1 << bitIndex);
	bitIndex++;
	if (bitIndex >= 16)
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

int	main(void)
{
	ft_printf("Server PID : %d\n", getpid());
	signal(SIGUSR1, bitToMessage);
	signal(SIGUSR2, bitToMessage);
	while (1)
		sleep(1);
	return (0);
}
