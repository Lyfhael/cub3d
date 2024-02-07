#include <unistd.h>

void    print_bits(unsigned char octet)
{
	int i;

	i = 8;
	while (i > 0)
	{
		if (octet & 0x80)
			write(1, "1", 1);
		else
			write(1, "0", 1);
		octet <<= 1;
		i--;
	}
}

void	str_to_bits(unsigned char *str)
{
	int		i;

	i = 0;
	while (str[i] != '\0')
	{
		print_bits(str[i]);
		i++;
		if (str[i] != '\0')
			write(1, " ", 1);
	}
}

int		main(void)
{
	str_to_bits("hello");
	write(1, "\n", 1);
}
