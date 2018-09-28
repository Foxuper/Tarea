#include <stdio.h>
#include <stdlib.h>

// Tipo de dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Tipo de dato | Nodo //
typedef struct Nodo
{
	double valor;
	struct Nodo *next;
} Nodo;

// Tipo de dato | Cola //
typedef struct Cola
{
	Nodo *front;
	Nodo *back;
} Cola;

// Definiciones | Funciones de Memoria //
Cola  *crear_Cola    (void);
void   borrar_Cola   (Cola *cola);

// Definiciones | Funciones //
bool   esta_vacia    (Cola *cola);
void   encolar       (Cola *cola, double valor);
double desencolar    (Cola *cola);
double frente        (Cola *cola);
double atras         (Cola *cola);
int    tamanio       (Cola *cola);
void   imprimir_Cola (Cola *cola);

// Funcion de Memoria | Crear Cola //
Cola *crear_Cola (void)
{
	Cola *cola = malloc(sizeof(Cola));
	cola->front = NULL;
	cola->back = NULL;
	return cola;
}

// Funcion de Memoria | Borrar Cola //
void borrar_Cola (Cola *cola)
{
	while (!esta_vacia(cola))
		desencolar(cola);
	free(cola);
}

// Funcion | Esta Vacia //
bool esta_vacia (Cola *cola)
{
	if (cola->front == NULL)
		return true;
	else
		return false;
}

// Funcion | Encolar //
void encolar (Cola *cola, double valor)
{
	Nodo *nodo = malloc(sizeof(Nodo));
	
	if (nodo != NULL)
	{
		nodo->valor = valor;
		nodo->next = NULL;
		
		if (esta_vacia(cola))
		{
			cola->front = nodo;
			cola->back = nodo;
		}
		else
		{
			cola->back->next = nodo;
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
		Nodo *front = cola->front;
		double valor = cola->front->valor;
		
		if (cola->front == cola->back)
		{
			cola->front = NULL;
			cola->back = NULL;
		}
		else
			cola->front = front->next;
		
		free(front);
		return valor;
	}
	else
		return 0;
}

// Funcion | Frente //
double frente (Cola *cola)
{
	if (!esta_vacia(cola))
		return cola->front->valor;
	else
		return 0;
}

// Funcion | Atras //
double atras (Cola *cola)
{
	if (!esta_vacia(cola))
		return cola->back->valor;
	else
		return 0;
}

// Funcion | Tamaño //
int tamanio (Cola *cola)
{
	if (!esta_vacia(cola))
	{
		int cuenta = 0;
		Nodo *actual = cola->front;
		
		while (actual != NULL)
		{
			cuenta = cuenta + 1;
			actual = actual->next;
		}
		
		return cuenta;
	}
	else
		return 0;
}

// Funcion | Imprimir Cola //
void imprimir_Cola (Cola *cola)
{
	Nodo *actual = cola->front;
	
	printf("\n Cola = Front -> ");
	
	while (actual != NULL)
	{
		printf("%lg -> ", actual->valor);
		actual = actual->next;
	}
	
	printf("Back");
}

// -------- MAIN -------- //
int main (void)
{
	char opcion;
	double valor;
	bool salir = false;
	Cola *cola = crear_Cola();
	
	while (!salir)
	{
		system("clear || cls");
		
		imprimir_Cola(cola);
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
				encolar(cola, valor);
			break;
			case '2':
				valor = desencolar(cola);
				printf("\n Valor desencolado: %lg", valor);
				getchar();
			break;
			case '3':
				valor = frente(cola);
				printf("\n Valor del frente: %lg", valor);
				getchar();
			break;
			case '4':
				valor = atras(cola);
				printf("\n Valor de atras: %lg", valor);
				getchar();
			break;
			case '5':
				printf("\n Tamanio de la cola: %d", tamanio(cola));
				getchar();
			break;
			case '6':
				if (esta_vacia(cola))
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
	
	borrar_Cola(cola);
	
	return 0;
}
