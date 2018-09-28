#include <stdio.h>
#include <stdlib.h>

// Constante | Tamaño //
#define SIZE 4

// Constantes | Inicio, Nulo y Vacio //
#define INICIO 0
#define NULO   (-1)
#define VACIO  (-2)

// Tipo de dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Tipo de dato | Puntero //
typedef int Puntero;

// Tipo de dato | Nodo //
typedef struct Nodo
{
	double valor;
	Puntero next;
} Nodo;

// Tipo de dato | Cola //
typedef struct Cola
{
	Nodo memoria[SIZE];
	Puntero front;
	Puntero back;
} Cola;

// Definiciones | Funciones de Memoria //
Puntero siguiente        (Puntero puntero);
Puntero malloc_Cola      (Cola *cola);
void    free_Cola        (Cola *cola, Puntero puntero);
void    inicializar_Cola (Cola *cola);

// Definiciones | Funciones //
bool   esta_vacia    (Cola *cola);
void   encolar       (Cola *cola, double valor);
double desencolar    (Cola *cola);
double frente        (Cola *cola);
double atras         (Cola *cola);
int    tamanio       (Cola *cola);
void   imprimir_Cola (Cola *cola);

// Funcion de Memoria | Siguiente //
Puntero siguiente (Puntero puntero)
{
	if (puntero < (SIZE - 1))
		return puntero + 1;
	else
		return INICIO;	
}

// Funcion de Memoria | Malloc de Cola //
Puntero malloc_Cola (Cola *cola)
{
	if (!esta_vacia(cola))
	{
		Puntero nodo = siguiente(cola->back);
		
		if (cola->memoria[nodo].next == VACIO)
			return nodo;
		else
			return NULO;
	}
	else
		return INICIO;
}

// Funcion de Memoria | Free de Cola //
void free_Cola (Cola *cola, Puntero puntero)
{
	if ((puntero >= INICIO) && (puntero < SIZE))
		cola->memoria[puntero].next = VACIO;
}

// Funcion de Memoria | Inicializar Cola //
void inicializar_Cola (Cola *cola)
{
	int i;
	cola->front = NULO;
	cola->back = NULO;
	
	for (i = 0; i < SIZE; i++)
		cola->memoria[i].next = VACIO;
}

// Funcion | Esta Vacia //
bool esta_vacia (Cola *cola)
{
	if (cola->front == NULO)
		return true;
	else
		return false;
}

// Funcion | Encolar //
void encolar (Cola *cola, double valor)
{
	Puntero nodo = malloc_Cola(cola);
	
	if (nodo != NULO)
	{
		cola->memoria[nodo].valor = valor;
		cola->memoria[nodo].next = NULO;
		
		if (esta_vacia(cola))
		{
			cola->front = nodo;
			cola->back = nodo;
		}
		else
		{
			cola->memoria[cola->back].next = nodo;
			cola->back = nodo;
		}
	}
	else
	{
		printf("\n Error: Memoria llena");
		getchar();
	}	
}

// Funcion | Desencolar //
double desencolar (Cola *cola)
{
	if (!esta_vacia(cola))
	{
		Puntero front = cola->front;
		double valor = cola->memoria[front].valor;
		
		if (cola->front == cola->back)
		{
			cola->front = NULO;
			cola->back = NULO;
		}
		else
			cola->front = cola->memoria[front].next;
		
		free_Cola(cola, front);
		return valor;
	}
	else
		return 0;
}

// Funcion | Frente //
double frente (Cola *cola)
{
	if (!esta_vacia(cola))
		return cola->memoria[cola->front].valor;
	else
		return 0;
}

// Funcion | Atras //
double atras (Cola *cola)
{
	if (!esta_vacia(cola))
		return cola->memoria[cola->back].valor;
	else
		return 0;
}

// Funcion | Tamaño //
int tamanio (Cola *cola)
{
	if (!esta_vacia(cola))
	{
		int cuenta = 0;
		Puntero actual = cola->front;
		
		while (actual != NULO)
		{
			cuenta = cuenta + 1;
			actual = cola->memoria[actual].next;
		}
		
		return cuenta;
	}
	else
		return 0;
}

// Funcion | Imprimir Cola //
void imprimir_Cola (Cola *cola)
{
	Puntero actual = cola->front;
	
	printf("\n Cola = Front -> ");
	
	while (actual != NULO)
	{
		printf("%lg -> ", cola->memoria[actual].valor);
		actual = cola->memoria[actual].next;
	}
	
	printf("Back");
}

// -------- MAIN -------- //
int main (void)
{
	Cola cola;
	char opcion;
	double valor;
	bool salir = false;
	inicializar_Cola(&cola);
	
	while (!salir)
	{
		system("clear || cls");
		
		imprimir_Cola(&cola);
		printf("\n\n 1) Encolar");
		printf("\n 2) Desencolar");
		printf("\n 3) Frente");
		printf("\n 4) Atras");
		printf("\n 5) Tamanio");
		printf("\n 6) Esta Vacia");
		printf("\n 7) Salir");
		printf("\n\n Opcion: ");
		
		opcion = getchar();
		
		switch (opcion)
		{
			case '1':
				printf("\n Ingrese un valor a encolar: ");
				scanf("%lg", &valor);
				encolar(&cola, valor);
			break;
			case '2':
				valor = desencolar(&cola);
				printf("\n Valor desencolado: %lg", valor);
				getchar();
			break;
			case '3':
				valor = frente(&cola);
				printf("\n Valor del frente: %lg", valor);
				getchar();
			break;
			case '4':
				valor = atras(&cola);
				printf("\n Valor de atras: %lg", valor);
				getchar();
			break;
			case '5':
				printf("\n Tamanio de la cola: %d", tamanio(&cola));
				getchar();
			break;
			case '6':
				if (esta_vacia(&cola))
					printf("\n Esta Vacia: SI");
				else
					printf("\n Esta Vacia: NO");
				getchar();
			break;
			case '7':
				printf("\n Presione una tecla para salir . . .");
				salir = true;
				getchar();
			break;
			default:
				printf("\n Opcion invalida . . .");
				getchar();
			break;
		}
		
		getchar();
	}
	
	return 0;
}
