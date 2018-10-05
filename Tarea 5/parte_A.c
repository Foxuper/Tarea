#include <stdio.h>
#include <stdlib.h>

// Tipo de dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Tipo de dato | Nodo //
typedef struct Nodo
{
	int valor;
	struct Nodo *left;
	struct Nodo *right;
} Nodo;

// Tipo de dato | Cola //
typedef struct Arbol
{
	Nodo *raiz;
} Arbol;

// Definiciones | Funciones de Memoria //
Nodo  *crear_Nodo       (int valor);
Arbol *crear_Arbol      (void);
void   borrar_Arbol     (Arbol *arbol);
void   borrar_Arbol_Aux (Nodo *raiz);

// Definiciones | Funciones //
void insertar            (Arbol *arbol, int valor);
void insertar_Aux        (Nodo *raiz, Nodo *nodo);
void recorrido_Preorden  (Nodo *raiz);
void recorrido_Inorden   (Nodo *raiz);
void recorrido_Postorden (Nodo *raiz);

// Funcion de Memoria | Crear Nodo //
Nodo *crear_Nodo (int valor)
{
	Nodo *nodo = malloc(sizeof(Nodo));
	nodo->valor = valor;
	nodo->left = NULL;
	nodo->right = NULL;
	return nodo;
}

// Funcion de Memoria | Crear Arbol //
Arbol *crear_Arbol (void)
{
	Arbol *arbol = malloc(sizeof(Arbol));
	arbol->raiz = NULL;
	return arbol;
}

// Funcion de Memoria | Borrar Arbol //
void borrar_Arbol (Arbol *arbol)
{
	if (arbol->raiz != NULL)
		borrar_Arbol_Aux(arbol->raiz);
	free(arbol);
}

// Funcion de Memoria | Auxiliar de Borrar Arbol //
void borrar_Arbol_Aux (Nodo *raiz)
{
	if (raiz->left != NULL)
		borrar_Arbol_Aux(raiz->left);
	if (raiz->right != NULL)
		borrar_Arbol_Aux(raiz->right);
	free(raiz);
}

// Funcion | Insertar //
void insertar (Arbol *arbol, int valor)
{
	Nodo *nodo = crear_Nodo(valor);
	
	if (arbol->raiz != NULL)
		insertar_Aux(arbol->raiz, nodo);
	else
		arbol->raiz = nodo;
}

// Funcion | Auxiliar de Insertar //
void insertar_Aux (Nodo *raiz, Nodo *nodo)
{
	int diferencia = nodo->valor - raiz->valor;
	
	if (diferencia < 0)
	{
		if (raiz->left != NULL)
			insertar_Aux(raiz->left, nodo);
		else
			raiz->left = nodo;
	}
	
	if (diferencia > 0)
	{
		if (raiz->right != NULL)
			insertar_Aux(raiz->right, nodo);
		else
			raiz->right = nodo;
	}
}

// Funcion | Recorrido Preorden //
void recorrido_Preorden (Nodo *raiz)
{
	printf("%d ", raiz->valor);
	
	if (raiz->left != NULL)
		recorrido_Preorden(raiz->left);
	
	if (raiz->right != NULL)
		recorrido_Preorden(raiz->right);
}

// Funcion | Recorrido Inorden //
void recorrido_Inorden (Nodo *raiz)
{
	if (raiz->left != NULL)
		recorrido_Inorden(raiz->left);
		
	printf("%d ", raiz->valor);
	
	if (raiz->right != NULL)
		recorrido_Inorden(raiz->right);
}

// Funcion | Recorrido Postorden //
void recorrido_Postorden (Nodo *raiz)
{
	if (raiz->left != NULL)
		recorrido_Postorden(raiz->left);
	
	if (raiz->right != NULL)
		recorrido_Postorden(raiz->right);
	
	printf("%d ", raiz->valor);
}

// -------- MAIN -------- //
int main (void)
{
	int valor;
	char opcion;
	bool salir = false;
	Arbol *arbol = crear_Arbol();
	
	while (!salir)
	{
		system("clear || cls");
		
		printf("\n Preorden -- [ ");
		if (arbol->raiz != NULL)
			recorrido_Preorden(arbol->raiz);
		printf("]");
		
		printf("\n Inorden --- [ ");
		if (arbol->raiz != NULL)
			recorrido_Inorden(arbol->raiz);
		printf("]");
		
		printf("\n Postorden - [ ");
		if (arbol->raiz != NULL)
			recorrido_Postorden(arbol->raiz);
		printf("]");
		
		printf("\n\n 1) Insertar");
		printf("\n 2) Reiniciar");
		printf("\n 3) Salir");
		printf("\n\n Opcion: ");
		
		opcion = getchar();
		
		switch (opcion)
		{
			case '1':
				printf("\n Ingrese un valor a insertar: ");
				scanf("%d", &valor);
				insertar(arbol, valor);
			break;
			case '2':
				borrar_Arbol(arbol);
				arbol = crear_Arbol();
			break;
			case '3':
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
	
	borrar_Arbol(arbol);
	
	return 0;
}
