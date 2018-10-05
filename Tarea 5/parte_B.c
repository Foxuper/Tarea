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

// Definiciones | CRUD | Create //
void insertar            (Arbol *arbol, int valor);
void insertar_Aux        (Nodo *raiz, Nodo *nodo);

// Definiciones | CRUD | Retrieve //
Nodo *buscar             (Arbol *arbol, int valor);
Nodo **buscar_Ref     (Arbol *arbol, int valor);
Nodo **buscar_Ref_Aux (Nodo *raiz, int valor);

// Definiciones | CRUD | Delete //
void   borrar         (Arbol *arbol, int valor);
void   borrar_Aux     (Nodo **referencia);
int    num_hijos      (Nodo *nodo);
Nodo  *obtener_Hijo   (Nodo *nodo);
void   copiar_Datos   (Nodo *Original, Nodo *Copia);
Nodo **maximo_Ref     (Nodo **referencia);

// Definiciones | Funciones //
Nodo *maximo              (Nodo *raiz);
Nodo *minimo              (Nodo *raiz);
void  recorrido_Preorden  (Nodo *raiz);
void  recorrido_Inorden   (Nodo *raiz);
void  recorrido_Postorden (Nodo *raiz);

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

// Funcion | Buscar //
Nodo *buscar (Arbol *arbol, int valor)
{
	Nodo **nodo = buscar_Ref(arbol, valor);
	
	if (nodo != NULL)
		return *nodo;
	else
		return NULL;
}

// Funcion Auxiliar | Buscar | Buscar Referencia //
Nodo **buscar_Ref (Arbol *arbol, int valor)
{
	if (arbol->raiz != NULL)
	{
		if (arbol->raiz->valor != valor)
			return buscar_Ref_Aux(arbol->raiz, valor);
		else
			return &arbol->raiz;
	}
	else
		return NULL;
}

// Funcion Auxiliar | Buscar | Auxiliar de Buscar Referencia //
Nodo **buscar_Ref_Aux (Nodo *raiz, int valor)
{
	int diferencia = valor - raiz->valor;
	
	if (diferencia < 0)
	{
		if (raiz->left != NULL)
		{
			if (raiz->left->valor == valor)
				return &raiz->left;
			else
				return buscar_Ref_Aux(raiz->left, valor);	
		}
		else
			return NULL;
	}
	
	if (diferencia > 0)
	{
		if (raiz->right != NULL)
		{
			if (raiz->right->valor == valor)
				return &raiz->right;
			else
				return buscar_Ref_Aux(raiz->right, valor);
		}
		else
			return NULL;
	}
	
	return NULL;
}

// Funcion | Borrar //
void borrar (Arbol *arbol, int valor)
{
	Nodo **referencia = buscar_Ref(arbol, valor);
	
	if (referencia != NULL)
		borrar_Aux(referencia);
}

// Funcion | Auxiliar de Borrar //
void borrar_Aux (Nodo **referencia)
{
	Nodo *nodo = *referencia;
	
	if (num_hijos(nodo) == 2)
	{
		Nodo **maximo = maximo_Ref(&nodo->left);
		copiar_Datos(*maximo, nodo);
		borrar_Aux(maximo);
	}
	else
	{
		Nodo *hijo = obtener_Hijo(nodo);
		*referencia = hijo;
		free(nodo);
	}
}

// Funcion Auxiliar | Borrar | Numero de Hijos //
int num_hijos (Nodo *nodo)
{
	int hijos = 0;
	
	if (nodo->left != NULL)
		hijos++;
		
	if (nodo->right != NULL)
		hijos++;
		
	return hijos;
}

// Funcion Auxiliar | Borrar | Obtener Hijo //
Nodo *obtener_Hijo (Nodo *nodo)
{
	if (nodo->left != NULL)
		return nodo->left;
	
	if (nodo->right != NULL)
		return nodo->right;
		
	return NULL;
}

// Funcion Auxiliar | Borrar | Copiar Datos //
void copiar_Datos (Nodo *Original, Nodo *Copia)
{
	Copia->valor = Original->valor;
}

// Funcion Auxiliar | Borrar | Referencia a Maximo //
Nodo **maximo_Ref (Nodo **referencia)
{
	Nodo *nodo = *referencia;
	
	if (nodo->right != NULL)
		return maximo_Ref(&nodo->right);
	else
		return referencia;
}

// Funcion | Maximo //
Nodo *maximo (Nodo *raiz)
{
	if (raiz->right != NULL)
		return maximo(raiz->right);
	else
		return raiz;
}

// Funcion | Minimo //
Nodo *minimo (Nodo *raiz)
{
	if (raiz->left != NULL)
		return minimo(raiz->left);
	else
		return raiz;
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
	Nodo *nodo;
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
		printf("\n 2) Borrar");
		printf("\n 3) Buscar");
		printf("\n 4) Maximo");
		printf("\n 5) Minimo");
		printf("\n 6) Salir");
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
				printf("\n Ingrese un valor a borrar: ");
				scanf("%d", &valor);
				borrar(arbol, valor);
			break;
			case '3':
				printf("\n Ingrese un valor a buscar: ");
				scanf("%d", &valor);
				nodo = buscar(arbol, valor);
				
				if (nodo == NULL)
					printf("\n Nodo no encontrado!");
				else
					printf("\n Nodo encontrado!");	
				
				getchar();	
			break;
			case '4':
				if (arbol->raiz != NULL)
				{
					nodo = maximo(arbol->raiz);
					printf("\n Maximo: %d", nodo->valor);
				}
				else
					printf("\n Maximo: N/a");
				getchar();
			break;
			case '5':
				if (arbol->raiz != NULL)
				{
					nodo = minimo(arbol->raiz);
					printf("\n Minimo: %d", nodo->valor);
				}
				else
					printf("\n Minimo: N/a");
				getchar();
			break;
			case '6':
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
