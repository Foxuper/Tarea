#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tipo de dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Tipo de dato | Libro //
typedef struct Libro
{
	char *titulo;
	int anio;
	struct Libro *siguiente;
	struct Libro *anterior;
} Libro;

// Tipo de dato | Libros //
typedef struct Libros
{
	Libro *inicio;
	Libro *final;
} Libros;

// -------- Funciones Auxiliares | Definiciones -------- //
Libros *crear_Libros (void);
void borrar_Libros (Libros *L);
void imprimir_Libros (Libros *L);
bool es_igual (Libro *libro, char *titulo, int anio);

// -------- CRUD | Definiciones -------- //
void crear_Libro (Libros *L, char *titulo, int anio);
Libro *obtener_Libro (Libros *L, char *titulo, int anio);
void modificar_Libro (Libros *L, char *titulo, int anio, char *n_titulo, int n_anio);
void borrar_Libro (Libros *L, char *titulo, int anio);

// Funcion Auxiliar | Crea una lista de libros //
Libros *crear_Libros (void)
{
	Libros *L = malloc(sizeof(Libros));
	L->inicio = NULL;
	L->final = NULL;
	return L;
}

// Funcion Auxiliar | Borra una lista de libros //
void borrar_Libros (Libros *L)
{
	Libro *actual = L->inicio;
	while (actual != NULL)
	{
		L->inicio = L->inicio->siguiente;
		free(actual->titulo);
		free(actual);
		actual = L->inicio;
	}
	free(L);
}

// Funcion Auxiliar | Imprime una lista de libros //
void imprimir_Libros (Libros *L)
{
	Libro *actual = L->inicio;
	printf("\n -------- Libros --------");
	
	if (actual != NULL)
	{
		while (actual != NULL)
		{
			printf("\n Titulo: %s, Anio: %d", actual->titulo, actual->anio);
			actual = actual->siguiente;
		}
	}	
	else
		printf("\n Vacio");
		
	printf("\n ------------------------\n");
}

// -------- Funcion auxiliar | Compara un libro con los datos de otro -------- //
bool es_igual (Libro *libro, char *titulo, int anio)
{
	if ((strcmp(libro->titulo, titulo) == 0) && (libro->anio == anio))
		return true;
	else
		return false;
}

// -------- CRUD | Crear -------- //
void crear_Libro (Libros *L, char *titulo, int anio)
{
	if (obtener_Libro(L, titulo, anio) == NULL)
	{
		// -------- Se crea un nuevo libro -------- //
		Libro *nuevo = malloc(sizeof(Libro));
		nuevo->titulo = malloc(strlen(titulo) + 1);
		strcpy(nuevo->titulo, titulo);
		nuevo->anio = anio;
		
		// Caso 1 | La lista esta vacia //
		if (L->inicio == NULL)
		{
			nuevo->siguiente = NULL;
			nuevo->anterior = NULL;
			L->inicio = nuevo;
			L->final = nuevo;
		}
		// Caso 2 | La lista no esta vacia //
		else
		{
			nuevo->siguiente = NULL;
			nuevo->anterior = L->final;
			L->final->siguiente = nuevo;
			L->final = nuevo;
		}
	}
}

// -------- CRUD | Obtener -------- //
Libro *obtener_Libro (Libros *L, char *titulo, int anio)
{
	// Caso 1 | La lista no esta vacia //
	if (L->inicio != NULL)
	{
		Libro *inicio = L->inicio;
		Libro *final = L->final;
		
		// -------- Se busca el libro por ambos lados de la lista -------- //
		while (inicio != final)
		{
			if (es_igual(inicio, titulo, anio))
				return inicio;
			else if (es_igual(final, titulo, anio))
				return final;
			
			if (inicio->siguiente != final)
			{
				inicio = inicio->siguiente;
				final = final->anterior;
			}
			else
				break;
		}
		
		// -------- Se compara el nodo impar -------- //
		if (inicio == final)
		{
			if (es_igual(inicio, titulo, anio))
				return inicio;
		}
		
		// -------- El libro no existe -------- //
		return NULL;
	}
	
	// Caso 2 | La lista esta vacia //
	return NULL;
}

// -------- CRUD | Modificar -------- //
void modificar_Libro (Libros *L, char *titulo, int anio, char *n_titulo, int n_anio)
{
	Libro *actual = obtener_Libro(L, titulo, anio);
	Libro *modificacion = obtener_Libro(L, n_titulo, n_anio);
	
	if ((actual != NULL) && (modificacion == NULL))
	{
		actual->titulo = realloc(actual->titulo, strlen(n_titulo) + 1);
		strcpy(actual->titulo, n_titulo);
		actual->anio = n_anio;
	}
}

// -------- CRUD | Borrar -------- //
void borrar_Libro (Libros *L, char *titulo, int anio)
{
	Libro *actual = obtener_Libro(L, titulo, anio);
	
	if (actual != NULL)
	{
		// Caso 1 | El libro esta en el inicio //
		if (actual == L->inicio)
		{
			L->inicio =L->inicio->siguiente;
			L->inicio->anterior = NULL;
		}
		// Caso 2 | El libro esta en el final //
		else if (actual == L->final)
		{
			L->final = L->final->anterior;
			L->final->siguiente = NULL;
		}
		// Caso 3 | El libro esta entre el inicio y el final //
		else
		{
			Libro *siguiente = actual->siguiente;
			Libro *anterior = actual->anterior;
			siguiente->anterior = anterior;
			anterior->siguiente = siguiente;
		}
		
		free(actual->titulo);
		free(actual);
	}
}

// -------- MAIN -------- //
int main (void)
{
	// Se crea una lista de libros //
	Libros *L = crear_Libros();
	
	// Se imprime la lista vacia //
	imprimir_Libros(L);
	
	// CRUD | Crear | Se crean tres libros en la lista //
	printf("\n - Crear   (Libro 1)");
	printf("\n - Crear   (Libro 2)");
	printf("\n - Crear   (Libro 3)\n");
	crear_Libro(L, "Libro 1", 1);
	crear_Libro(L, "Libro 2", 2);
	crear_Libro(L, "Libro 3", 3);
	
	// Se imprime la lista antes de los cambios //
	imprimir_Libros(L);
	
	// CRUD | Obtener | Se obtiene el apuntador al (Libro 1) y se imprime //
	Libro *obtenido = obtener_Libro(L, "Libro 1", 1);
	printf("\n - Obtener   (Libro 1) -> ");
	printf("Titulo: %s, Anio: %d", obtenido->titulo, obtenido->anio);
	
	// CRUD | Modificar | Se modifica el (Libro 3) //
	modificar_Libro(L, "Libro 3", 3, "Libro 0", 0);
	printf("\n - Modificar (Libro 3, Libro 0)");
	
	// CRUD | Borrar | Se borra el (Libro 2) //
	borrar_Libro(L, "Libro 2", 2);
	printf("\n - Borrar    (Libro 2)\n");
	
	// Se imprime la lista despues de los cambios //
	imprimir_Libros(L);
	
	// Se borra la lista de libros //
	borrar_Libros(L);
	
	getchar();
	return 0;
}

