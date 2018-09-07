#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VACIO NULL

// Estructura | Libro //
// next -   Apuntador al siguiente libro
// titulo - Titulo del libro
// anio -   Año de publicacion del libro
typedef struct Libro
{	
	struct Libro *next;
	char *titulo;
	int anio;
} Libro;

// Tipo de Dato | Lista de (Libro) //
typedef Libro *Libros;

// Tipo de Dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Agregar Libro | Argumentos //
// libros - Apuntador a una lista de (Libro)
// titulo - Titulo del libro a agregar
// anio -   Año de publicacion del libro a agregar
void agregar_libro (Libros *libros, char *titulo, int anio)
{
	Libro *libro;
	
	// Se crea un nuevo (Libro) //
	libro = malloc(sizeof(Libro));
	libro->titulo = malloc(strlen(titulo) + 1);
	strcpy(libro->titulo, titulo);
	libro->anio = anio;
	libro->next = NULL;
	
	// La lista de (Libro) esta vacia //
	if ((*libros) != VACIO)
	{
		Libro *actual = (*libros);
		while (actual->next != NULL)
			actual = actual->next;
		actual->next = libro;
	}
	// La lista de (Libro) no esta vacia //
	else
		(*libros) = libro;
}

// Buscar Libro | Argumentos //
// libros - Apuntador a una lista de (Libro)
// titulo - Titulo del libro a buscar
// anio -   Año de publicacion del libro a buscar
bool buscar_libro (Libros libros, char *titulo, int anio)
{
	Libro *actual = libros;
	bool encontrado = false;
	
	while (actual != NULL)
	{
		if ((strcmp(actual->titulo, titulo) == 0) && (actual->anio == anio))
		{
			encontrado = true;
			break;
		}
		else
			actual = actual->next;
	}
	
	return encontrado;
}

int main (void)
{
	Libro *actual;
	bool encontrado;
	Libros libros = VACIO;
	
	// Se agregan cuatro libros //
	agregar_libro(&libros, "Libro A", 1111);
	agregar_libro(&libros, "Libro B", 2222);
	agregar_libro(&libros, "Libro C", 3333);
	agregar_libro(&libros, "Libro D", 4444);
	
	// Se imprimen los libros registrados //
	actual = libros;
	while (actual != NULL)
	{
		printf("\n Titulo: %s", actual->titulo);
		printf("\n Anio: %d\n", actual->anio);
		actual = actual->next;
	}
	
	// Se busca un libro y se muestra el resultado //
	encontrado = buscar_libro(libros, "Libro B", 2222);
	printf("\n Busqueda (Libro B, 2222) = ");
	if (encontrado)
		printf("Encontrado!");
	else
		printf("No Encontrado!");
	
	// Se busca un libro y se muestra el resultado //
	encontrado = buscar_libro(libros, "Libro E", 5555);
	printf("\n Busqueda (Libro E, 5555) = ");
		if (encontrado)
		printf("Encontrado!");
	else
		printf("No Encontrado!");
		
	getchar();
	
	// Se libera la memoria //
	actual = libros;
	while (actual != NULL)
	{
		libros = libros->next;
		free(actual->titulo);
		free(actual);
		actual = libros;
	}
	
	return 0;
}

