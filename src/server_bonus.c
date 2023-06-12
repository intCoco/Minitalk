#include "../include/minitalk_bonus.h"

#define MESSAGE_SIZE 10000

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = -1;
	while (++i < len)
		((char *)b)[i] = (unsigned char)c;
	return (b);
}

void	bitToMessage(int signal, siginfo_t *sa, void *whatever)
{
	static unsigned char	message[MESSAGE_SIZE];
	static int	bitIndex = 0;
	static int	charIndex = 0;

	(void)whatever;
	if (signal == SIGUSR1)
		message[charIndex] |= (1 << bitIndex);
	bitIndex++;
	if (bitIndex >= 16)
	{
		if (message[charIndex] == '\0')
		{
			ft_printf("Received message from client with PID #%d : \"%s\"\n",
				sa->si_pid, message);
			ft_memset(message, 0, sizeof(message));
			kill(sa->si_pid, SIGUSR1);
			charIndex = 0;
		}
		else
			charIndex++;
		bitIndex = 0;
	}
}

int	main(void)
{
	struct sigaction	sa;

	ft_printf("Server PID : %d\n", getpid());
	sa.sa_sigaction = bitToMessage;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		sleep(1);
	return (0);
}
