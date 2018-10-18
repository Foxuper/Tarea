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
	int nodos_left;
	struct Nodo *left;
	struct Nodo *right;
} Nodo;

// Tipo de dato | Cola //
typedef struct Arbol
{
	Nodo *raiz;
	int nodos;
} Arbol;

// Definiciones | Funciones de Memoria //
Nodo  *crear_Nodo       (int valor);
Arbol *crear_Arbol      (void);
void   borrar_Arbol     (Arbol *arbol);
void   borrar_Arbol_Aux (Nodo *raiz);

// Definiciones | Funciones Auxiliares //
int   altura          (Nodo *raiz);
Nodo *rotar_izquierda (Nodo *raiz);
Nodo *rotar_derecha   (Nodo *raiz);
int   obtener_balance (Nodo *raiz);

// Definiciones | Funcion Insertar AVL //
void insertar     (Arbol *arbol, int valor);
void insertar_Aux (Arbol *arbol, Nodo **referencia, Nodo *nodo);

// Definiciones | Funcion Borrar AVL //
void   borrar         (Arbol *arbol, int valor);
void   borrar_Aux     (Arbol *arbol, Nodo **referencia, int valor);
void   borrar_nodo    (Nodo **referencia);
int    num_hijos      (Nodo *nodo);
Nodo  *obtener_Hijo   (Nodo *nodo);
void   copiar_Datos   (Nodo *Original, Nodo *Copia);
Nodo **maximo_Ref     (Nodo **referencia);

// Definiciones | Funciones //
double mediana             (Arbol *arbol);
int    obtener_valor       (Nodo *raiz, int indice);
int    contar_nodos        (Nodo *raiz);
void   recorrido_Preorden  (Nodo *raiz);
void   recorrido_Inorden   (Nodo *raiz);
void   recorrido_Postorden (Nodo *raiz);

// Funcion de Memoria | Crear Nodo //
Nodo *crear_Nodo (int valor)
{
	Nodo *nodo = malloc(sizeof(Nodo));
	nodo->valor = valor;
	nodo->left = NULL;
	nodo->right = NULL;
	nodo->nodos_left = 0;
	return nodo;
}

// Funcion de Memoria | Crear Arbol //
Arbol *crear_Arbol (void)
{
	Arbol *arbol = malloc(sizeof(Arbol));
	arbol->raiz = NULL;
	arbol->nodos = 0;
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

// Funcion Auxiliar | Altura //
int altura (Nodo *raiz)
{
	if (raiz != NULL)
	{
		int altura_left = altura(raiz->left);
		int altura_right = altura(raiz->right);
		
		if (altura_left > altura_right)
			return altura_left + 1;
		else
			return altura_right + 1;
	}
	else
		return 0;
}

// Funcion Auxiliar | Rotar Izquierda //
Nodo *rotar_izquierda (Nodo *raiz)
{
	Nodo *pivote = raiz->right;
	raiz->right = pivote->left;
	pivote->left = raiz;
	return pivote;
}

// Funcion Auxiliar | Rotar Derecha //
Nodo *rotar_derecha (Nodo *raiz)
{
	Nodo *pivote = raiz->left;
	raiz->left = pivote->right;
	pivote->right = raiz;
	return pivote;
}

// Funcion Auxiliar | Obtener Balance //
int obtener_balance (Nodo *raiz)
{
	if (raiz != NULL)
		return altura(raiz->left) - altura(raiz->right);
	else
		return 0;
}

// Funcion | Insertar //
void insertar (Arbol *arbol, int valor)
{
	Nodo *nodo = crear_Nodo(valor);
	
	if (arbol->raiz == NULL)
	{
		arbol->raiz = nodo;
		arbol->nodos++;
	}
	else
		insertar_Aux(arbol, &arbol->raiz, nodo);	
}

// Funcion | Auxiliar de Insertar //
void insertar_Aux (Arbol *arbol, Nodo **referencia, Nodo *nodo)
{
	Nodo *raiz = *referencia;
	int diferencia = nodo->valor - raiz->valor;
	
	// ----- Insercion del Nodo ----- //
	if (diferencia < 0)
	{
		if (raiz->left == NULL)
		{
			raiz->left = nodo;
			arbol->nodos++;
		}
		else
			insertar_Aux(arbol, &raiz->left, nodo); // Recursividad //	
	}
	
	if (diferencia > 0)
	{
		if (raiz->right == NULL)
		{
			raiz->right = nodo;
			arbol->nodos++;
		}
		else
			insertar_Aux(arbol, &raiz->right, nodo); // Recursividad //	
	}
	
	// ----- Rebalanceo Recursivo ----- //
	int balance = obtener_balance(raiz);
	
	if (balance < -1)
	{
		if (nodo->valor < raiz->right->valor)
		{
			raiz->right = rotar_derecha(raiz->right);
			*referencia = rotar_izquierda(raiz);
		}
		else if (nodo->valor > raiz->right->valor)
			*referencia = rotar_izquierda(raiz);
	}
	
	if (balance > 1)
	{
		if (nodo->valor > raiz->left->valor)
		{
			raiz->left = rotar_izquierda(raiz->left);
			*referencia = rotar_derecha(raiz);
		}
		else if (nodo->valor < raiz->left->valor)
			*referencia = rotar_derecha(raiz);
	}
	
	// Cuenta de nodos izquierdos //
	raiz = *referencia;
	if (raiz != NULL)
		raiz->nodos_left = contar_nodos(raiz->left);
}

// Funcion | Borrar //
void borrar (Arbol *arbol, int valor)
{
	if (arbol->raiz != NULL)
		borrar_Aux(arbol, &arbol->raiz, valor);	
}

// Funcion | Auxiliar de Borrar //
void borrar_Aux (Arbol *arbol, Nodo **referencia, int valor)
{
	Nodo *raiz = *referencia;
	int diferencia = valor - raiz->valor;
	
	// ----- Busqueda y eliminacion del nodo ----- //
	if ((diferencia < 0) && (raiz->left != NULL))
	{
		if (raiz->left->valor == valor)
		{
			borrar_nodo(&raiz->left);
			arbol->nodos--;
		}
		else
			borrar_Aux(arbol, &raiz->left, valor); // Recursividad //
	}
	
	if ((diferencia > 0) && (raiz->right != NULL))
	{
		if (raiz->right->valor == valor)
		{
			borrar_nodo(&raiz->right);
			arbol->nodos--;
		}
		else
			borrar_Aux(arbol, &raiz->right, valor); // Recursividad //
	}
	
	if (diferencia == 0)
	{
		borrar_nodo(referencia);
		raiz = *referencia;
		arbol->nodos--;
	}
	
	// ----- Rebalanceo Recursivo ----- //
	int balance = obtener_balance(raiz);
	
	if (balance < -1)
	{
		if (obtener_balance(raiz->right) > 0)
		{
			raiz->right = rotar_derecha(raiz->right);
			*referencia = rotar_izquierda(raiz);
		}
		else
			*referencia = rotar_izquierda(raiz);
	}
	
	if (balance > 1)
	{
		if (obtener_balance(raiz->left) < 0)
		{
			raiz->left = rotar_izquierda(raiz->left);
			*referencia = rotar_derecha(raiz);
		}
		else
			*referencia = rotar_derecha(raiz);
	}
	
	// ----- Cuenta de nodos izquierdos ----- //
	raiz = *referencia;
	if (raiz != NULL)
		raiz->nodos_left = contar_nodos(raiz->left);
}

// Funcion Auxiliar | Borrar | Borrar Nodo //
void borrar_nodo (Nodo **referencia)
{
	Nodo *nodo = *referencia;
	
	if (num_hijos(nodo) == 2)
	{
		Nodo **maximo = maximo_Ref(&nodo->left);
		copiar_Datos(*maximo, nodo);
		borrar_nodo(maximo);
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

// Funcion | Mediana //
double mediana (Arbol *arbol)
{
	int mitad = arbol->nodos / 2;
	
	if ((mitad * 2) == arbol->nodos)
	{
		int a = obtener_valor(arbol->raiz, mitad);
		int b = obtener_valor(arbol->raiz, mitad + 1);
		
		return (a + b) / 2.0;
	}
	else
		return obtener_valor(arbol->raiz, mitad + 1);
}

// Funcion | Obtener Valor //
int obtener_valor (Nodo *raiz, int indice)
{
	if (raiz != NULL)
	{
		int mitad = raiz->nodos_left + 1;
	
		if (indice < mitad)
			return obtener_valor(raiz->left, indice);
		else if (indice > mitad)
			return obtener_valor(raiz->right, indice - mitad);
		else
			return raiz->valor;
	}
	else
		return 0;
}

// Funcion | Contar Nodos //
int contar_nodos (Nodo *raiz)
{
	if (raiz != NULL)
	{
		int nodos = 1;
		
		nodos += contar_nodos(raiz->left);
		nodos += contar_nodos(raiz->right);
		
		return nodos;
	}
	else
		return 0;
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
		printf("\n 2) Borrar");
		printf("\n 3) Mediana");
		printf("\n 4) Salir");
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
				printf("\n Mediana: %lg", mediana(arbol));
				getchar();
			break;
			case '4':
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
