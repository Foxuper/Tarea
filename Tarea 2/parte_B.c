#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tipo de dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Tipo de dato | Nodo //
typedef struct nodo
{
	int valor;
	struct nodo *siguiente;
	struct nodo *anterior;
} nodo;

// Tipo de dato | Lista //
typedef struct lista
{
	nodo *actual;
} lista;

// -------- Funciones | Definiciones -------- //
bool esta_vacia (lista *L);
lista *crear_lista (void);
void borrar_lista (lista *L);
void crear_nodo (lista *L, int valor);
void borrar_nodo (lista *L, nodo *N);
int flavio_josefo (int personas, int saltos);

// Funcion Auxiliar | Verifica si una lista esta vacia //
bool esta_vacia (lista *L)
{
	if (L->actual == NULL)
		return true;
	else
		return false;
}

// Funcion Auxiliar | Crea una lista //
lista *crear_lista (void)
{
	lista *L = malloc(sizeof(lista));
	L->actual = NULL;
	return L;
}

// Funcion Auxiliar | Borra una lista //
void borrar_lista (lista *L)
{
	while (!esta_vacia(L))
		borrar_nodo(L, L->actual);
	free(L);
}

// -------- CRUD | Crear -------- //
void crear_nodo (lista *L, int valor)
{
	nodo *N = malloc(sizeof(nodo));
	N->valor = valor;
	
	// Caso 1 | La lista esta vacia //
	if (esta_vacia(L))
	{
		L->actual = N;
		N->siguiente = N;
		N->anterior = N;
	}
	// Caso 2 | La lista no esta vacia //
	else
	{
		nodo *siguiente = L->actual;
		nodo *anterior = L->actual->anterior;
		
		N->siguiente = siguiente;
		N->anterior = anterior;
		siguiente->anterior = N;
		anterior->siguiente = N;
	}
}

// -------- CRUD | Borrar -------- //
void borrar_nodo (lista *L, nodo *N)
{
	if (N != NULL)
	{
		// Caso 1 | La lista tiene mas de un elemento //
		if (N != N->siguiente)
		{
			nodo *siguiente = N->siguiente;
			nodo *anterior = N->anterior;
			anterior->siguiente = siguiente;
			siguiente->anterior = anterior;
			
			if (N == L->actual)
				L->actual = siguiente;
		}
		// Caso 2 | La lista tiene solo un elemento //
		else
			L->actual = NULL;
		
		free(N);
	}
}

// Funcion | Sobreviviente de Flavio Josefo //
int flavio_josefo (int personas, int saltos)
{
	int i, vivo;
	nodo *muerto, *actual;
	lista *L = crear_lista();
	
	// Se llena una lista circular con (n) personas //
	for (i = 0; i < personas; i++)
		crear_nodo(L, i + 1);
	
	actual = L->actual;
	
	// Se van borrando las personas muertas de la lista //
	while (actual != actual->siguiente)
	{
		for (i = 0; i < saltos; i++)
			actual = actual->siguiente;
		muerto = actual->anterior;
		borrar_nodo(L, muerto);
	}
	
	// Al final solo quedara una persona viva //
	vivo = actual->valor;
	borrar_lista(L);
	
	return vivo;
}

// -------- MAIN -------- //
int main (void)
{
	int personas, saltos;
	
	printf("\n Ingrese el numero de personas [1, n]: ");
	scanf("%d", &personas);
	
	printf(" Ingrese el numero de saltos [0, k]: ");
	scanf("%d", &saltos);
	
	printf("\n Persona viva: %d", flavio_josefo(personas, saltos));
	getchar();
	getchar();
	
	return 0;
}

