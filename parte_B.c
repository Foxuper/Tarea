#include <stdio.h>

int sumar (int *elementos)
{
	if (elementos[0] == 10)
		return elementos[0];
	else
		return elementos[0] + sumar(&elementos[1]);
}

int main (int argc, char *argv[])
{
	int elementos[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int resultado = sumar(elementos);
	printf("Suma = %d", resultado);
	getchar();
	return 0;
}
