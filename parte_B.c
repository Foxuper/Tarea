#include <stdio.h>

// Sumar | Argumentos //
// *elementos - Apuntador a un arreglo
// n          - Cantidad de elementos en el arreglo
int sumar (int *elementos, int n)
{
	if (n == 1)
		return elementos[0];
	else
		return elementos[0] + sumar(&elementos[1], n - 1);
}

// Main | Argumentos //
// void - No son necesarios argumentos
int main (void)
{
	int elementos[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int resultado = sumar(elementos, 10);
	printf("Suma = %d", resultado);
	getchar();
	return 0;
}

