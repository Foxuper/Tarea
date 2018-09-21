#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Tipo de dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Definicion | Longitud de buffer //
#define BUFF_CADENA 100

// Tipo de dato | Caracter (de una [Pila]) //
typedef struct Caracter
{
	char caracter;
	struct Caracter *siguiente;
} Caracter;

// Tipo de dato | Pila //
typedef struct Pila
{
	Caracter *tope;
} Pila;

// ------------------------------------- //

// Definiciones | Funciones | Pila //
Pila *crear_Pila  (void);
void  apilar      (Pila *P, char caracter);
char  desapilar   (Pila *P);
void  borrar_Pila (Pila *P);

// Definicion | Funcion Principal //
bool es_palindromo (char *cadena);

// ------------------------------------- //

// Funcion | Pila | Crear //
Pila *crear_Pila (void)
{
	Pila *P = malloc(sizeof(Pila));
	P->tope = NULL;
	return P;
}

// Funcion | Pila | Esta Vacia //
bool esta_vacia (Pila *P)
{
	if (P->tope == NULL)
		return true;
	else
		return false;
}

// Funcion | Pila | Apilar //
void apilar (Pila *P, char caracter)
{
	Caracter *C = malloc(sizeof(Caracter));
	C->caracter = caracter;
	C->siguiente = P->tope;
	P->tope = C;
}

// Funcion | Pila | Desapilar //
char desapilar (Pila *P)
{
	char caracter = '\0';
	
	if (!esta_vacia(P))
	{
		Caracter *C = P->tope;
		P->tope = P->tope->siguiente;
		caracter = C->caracter;
		free(C);
	}
	
	return caracter;
}

// Funcion | Pila | Borrar //
void borrar_Pila (Pila *P)
{
	while (!esta_vacia(P))
		desapilar(P);
	
	free(P);
}

// Funcion | Es Palindromo //
bool es_palindromo (char *cadena)
{
	int i, j, longitud;
	Pila *P = crear_Pila();
	char normal[BUFF_CADENA];
	char inversa[BUFF_CADENA];
	
	j = 0;
	longitud = strlen(cadena);
	cadena[longitud - 1] = '\0';
	
	for (i = 0; i < longitud; i++)
	{
		if (cadena[i] != ' ')
			normal[j++] = tolower(cadena[i]);
	}
	
	normal[j] = '\0';
	longitud = strlen(normal);
	
	for (i = 0; i < longitud; i++)
		apilar(P, normal[i]);
	
	i = 0;
	while (!esta_vacia(P))
		inversa[i++] = desapilar(P);
	
	inversa[i] = '\0';
	borrar_Pila(P);
	
	if (strcmp(normal, inversa) == 0)
		return true;
	else
		return false;
}

// -------- MAIN -------- //
int main (void)
{
	char cadena[BUFF_CADENA];
	
	printf("\n Ingrese una cadena: ");
	fgets(cadena, BUFF_CADENA, stdin);
	
	if (es_palindromo(cadena))
		printf("\n   La cadena SI es palindromo");
	else
		printf("\n   La cadena NO es palindromo");
	
	printf("\n\n Presione una tecla para salir . . .");
	getchar();
	
	return 0;
}
