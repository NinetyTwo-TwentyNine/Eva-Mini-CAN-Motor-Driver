#include "utilities_general.h"

int abs(int num)
{
	if (num < 0)
	{
		return (num * -1);
	}
	return num;
}

int min(int num1, int num2)
{
	return num1 < num2 ? num1 : num2;
}

int max(int num1, int num2)
{
	return num1 < num2 ? num2 : num1;
}
