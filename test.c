#include <stdio.h>
#include <unistd.h>

int main()
{
	while (1)
	{
		write(1, "ok\n", 3);
		sleep(1);
	}
}
