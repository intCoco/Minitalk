#include "../include/minitalk_bonus.h"

#define MESSAGE_SIZE 1000

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
	static char	message[MESSAGE_SIZE];
	static int	bitIndex = 0;
	static int	charIndex = 0;

	(void)whatever;
	if (signal == SIGUSR1)
		message[charIndex] |= (1 << bitIndex);
	bitIndex++;
	if (bitIndex >= 8)
	{
		if (message[charIndex] == '\0')
		{
			ft_printf("Received message from client nb: %s\n", message);
			ft_printf("Client pid : %d\n", sa->si_pid);
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
	pid_t				pid;
	struct sigaction	sa;

	pid = getpid();
	ft_printf("Server pid : %d\n", pid);
	sa.sa_sigaction = bitToMessage;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		sleep(1);
	return (0);
}
