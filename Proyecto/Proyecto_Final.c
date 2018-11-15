#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>

// Tipo de dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Definiciones //
#define LINEAS 16     // Lineas maximas por nodo
#define BUFFER 64     // Longitud maxima de texto
#define ESPERA 100    // Milisegundos
#define KILOBYTE 1024
#define ARCHIVO_UNO  "Datos_Uno.txt"
#define ARCHIVO_DOS  "Datos_Dos.txt"
#define ARCHIVO_TRES "Datos_Tres.txt"

// Tipo de dato | Pila //
typedef struct Pila
{
	char texto[BUFFER];
	int indice;
} Pila;

// Tipo de dato | Registro //
typedef struct Registro
{
	int edad;
	char nombre[BUFFER];
	char ocupacion[BUFFER];
} Registro;

// Tipo de dato | Valor//
typedef struct Valor
{
	int llave;          // Llave
	int cantidad;       // Cantidad de lineas
	int lineas[LINEAS]; // Lineas de datos
} Valor;

// Tipo de dato | Nodo //
typedef struct Nodo
{
	Valor *valores;      // Valores
	struct Nodo **hijos; // Hijos
	bool es_hoja;        // Es hoja
	struct Nodo *padre;  // Padre
	int n_llaves;        // Numero de llaves
} Nodo;

// Tipo de dato | Arbol //
typedef struct Arbol
{
	Nodo *raiz;
	int grado;
} Arbol;

// Variables estaticas de memoria //
static int inicio, fin, n_lineas;
static int n_registros = 10 * KILOBYTE / sizeof(Registro);
static Registro memoria[10 * KILOBYTE / sizeof(Registro)];

// Definiciones | Funciones de Nodo y Arbol //
Nodo  *crear_Nodo       (int grado, bool es_hoja);
void   borrar_Nodo      (Nodo *nodo);
Arbol *crear_Arbol      (int grado);
void   vaciar_arbol     (Arbol *arbol);
void   borrar_Arbol     (Arbol *arbol);
void   borrar_Arbol_aux (Nodo *nodo, int grado);

// Definiciones | Funciones de Impresion de Arbol //
void imprimir_Arbol        (Arbol *arbol);
void imprimir_Arbol_aux    (Pila *pila, Nodo *nodo, int grado);
void imprimir_Valor_lineas (Valor valor);
void push_Pila             (Pila *pila, char caracter);
void pop_Pila              (Pila *pila);

// Definiciones | Funciones de Insercion en Arbol //
void  insertar_Arbol (Arbol *arbol, Valor valor);
Nodo *buscar_Nodo    (Nodo *nodo, int llave);
void  agregar_Valor  (Nodo *nodo, Valor valor);
void  ajustar_Nodo   (Arbol *arbol, Nodo *nodo);
bool  existe_Llave   (Nodo *nodo, int llave);
int   buscar_Indice  (Nodo *nodo, int llave);
void  agregar_Linea  (Valor *valor, int linea);

// Definiciones | Funciones de Registro //
void imprimir_Registro (Registro *registro);
void generar_Registro (Registro *registro, char *nombre);

// Definiciones | Funciones Auxiliares //
int hashear_Texto (char *texto);
int contar_Lineas (FILE *archivo);

// Definiciones | Funciones de indice //
void      cargar_Ram      (int linea, char *nombre_archivo);
void      cargar_Indice   (Arbol *arbol, char *nombre_archivo);
Registro *buscar_Registro (Arbol *arbol, char *nombre_archivo, char *nombre);

// Definicion | Funcion de busqueda forzada //
Registro *forzar_Busqueda (char *nombre_archivo, char *nombre);

// Definiciones | Funciones de interaccion //
void pausa            (void);
void titulo           (void);
void leer_Entrada     (char *buffer, int longitud, bool numero);
int  menu_Principal   (bool indexado, char *nombre_archivo);
bool comprobar_Nombre (char *nombre_archivo);
bool cambiar_Archivo  (Arbol *arbol, char *nombre_archivo);

// Funcion | Crear Nodo //
Nodo *crear_Nodo (int grado, bool es_hoja)
{
	Nodo *nodo = malloc(sizeof(Nodo));
	nodo->valores = calloc(grado, sizeof(Valor));
	nodo->hijos = calloc((grado + 1), sizeof(Nodo *));
	nodo->es_hoja = es_hoja;
	nodo->padre = NULL;
	nodo->n_llaves = 0;
	
	return nodo;
}

// Funcion | Borrar Nodo //
void borrar_Nodo (Nodo *nodo)
{
	free(nodo->valores);
	free(nodo->hijos);
	free(nodo);
}

// Funcion | Crear Arbol //
Arbol *crear_Arbol (int grado)
{
	Arbol *arbol = malloc(sizeof(Arbol));
	
	arbol->grado = grado;
	arbol->raiz = NULL;
	
	return arbol;
}

// Funcion | Vaciar Arbol //
void vaciar_Arbol (Arbol *arbol)
{
	if (arbol->raiz != NULL)
	{
		borrar_Arbol_aux(arbol->raiz, arbol->grado);
		arbol->raiz = NULL;
	}
}

// Funcion | Borrar Arbol //
void borrar_Arbol (Arbol *arbol)
{
	vaciar_Arbol(arbol);
	free(arbol);
}

// Funcion | Auxiliar de Borrar Arbol //
void borrar_Arbol_aux (Nodo *nodo, int grado)
{
	int i;
	if (!nodo->es_hoja)
	{
		for (i = 0; i < nodo->n_llaves + 1; i++)
		   	borrar_Arbol_aux(nodo->hijos[i], grado);
	}
	borrar_Nodo(nodo);
}

// Funcion | Imprimir Arbol //
void imprimir_Arbol (Arbol *arbol)
{
	if (arbol->raiz != NULL)
	{
		Pila pila;
		pila.indice = 0;
		pila.texto[0] = '\0';
		imprimir_Arbol_aux(&pila, arbol->raiz, arbol->grado);
	}
	else
		printf("\t Estado: Indice vacio\n");
		
	printf("\n ------------------- Indice ------------------");
}

// Funcion | Auxiliar de Imprimir Arbol //
void imprimir_Arbol_aux (Pila *pila, Nodo *nodo, int grado)
{
	int i;
	Valor *valores = nodo->valores;
	
	if (!nodo->es_hoja)
	{
		printf("[");
		for (i = 0; i < (nodo->n_llaves - 1); i++)
			printf("%d, ", valores[i].llave);
		printf("%d]\n", valores[i].llave);
	}
	else
	{
		printf("[");
		for (i = 0; i < (nodo->n_llaves - 1); i++)
		{
			printf("%d > ", valores[i].llave);
			imprimir_Valor_lineas(valores[i]);
			printf(" | ");
		}
		printf("%d > ", valores[i].llave);
		imprimir_Valor_lineas(valores[i]);
		printf("]\n");
	}
	
	if (!nodo->es_hoja)
	{
		for (i = 0; i < (grado + 1); i++)
		{
		    Nodo *hijo = nodo->hijos[i];
		    if (i < nodo->n_llaves)
		    	printf("%s %c%c%c", pila->texto, 195, 196, 196);
		    if (i == nodo->n_llaves)
		    	printf("%s %c%c%c", pila->texto, 192, 196, 196);
		    
		    if (i < nodo->n_llaves)
		    	push_Pila(pila, (char) 179);
		    else
		    	push_Pila(pila, ' ');
		    
		    if (i < (nodo->n_llaves + 1))
		    	imprimir_Arbol_aux(pila, hijo, grado);
		    pop_Pila(pila);
		}
	}
}

// Funcion | Imprimir lineas de Valor //
void imprimir_Valor_lineas (Valor valor)
{
	if (valor.cantidad > 0)
	{
		int i;
		for (i = 0; i < (valor.cantidad - 1); i++)
			printf("%d, ", valor.lineas[i]);
		printf("%d", valor.lineas[i]);
	}
}

// Funcion | Push de Pila //
void push_Pila (Pila *pila, char caracter)
{
    pila->texto[pila->indice++] = ' ';
    pila->texto[pila->indice++] = caracter;
    pila->texto[pila->indice++] = ' ';
    pila->texto[pila->indice++] = ' ';
    pila->texto[pila->indice] = '\0';
}

// Funcion | Pop de Pila //
void pop_Pila (Pila *pila)
{
	pila->indice -= 4;
    pila->texto[pila->indice] = '\0';
}

// Funcion | Insertar //
void insertar_Arbol (Arbol *arbol, Valor valor)
{
	valor.cantidad = 1;
	
	if (arbol->raiz == NULL)
	{
		arbol->raiz = crear_Nodo(arbol->grado, true);
		arbol->raiz->valores[0] = valor;
		arbol->raiz->n_llaves = 1;
	}
	else
	{
		int llave = valor.llave;
		Nodo *hoja = buscar_Nodo(arbol->raiz, llave);
		
		if (!existe_Llave(hoja, llave))
		{
			agregar_Valor(hoja, valor);
			ajustar_Nodo(arbol, hoja);
		}
		else
		{
			int linea = valor.lineas[0];
			int indice = buscar_Indice(hoja, llave);
			agregar_Linea(&hoja->valores[indice], linea);
		}
	}
}

// Funcion | Buscar Nodo Hoja //
Nodo *buscar_Nodo (Nodo *nodo, int llave)
{
	if (nodo->es_hoja)
		return nodo;
	
	int i;
	int primero = 0;
	int ultimo = nodo->n_llaves - 1;
	
	Valor *valores = nodo->valores;
	Nodo **hijos = nodo->hijos;
	
	if (llave < valores[primero].llave)
		return buscar_Nodo(hijos[primero], llave);
	
	if (llave >= valores[ultimo].llave)
		return buscar_Nodo(hijos[ultimo + 1], llave);
	
	for (i = primero; i < ultimo; i++)
	{
		if ((valores[i].llave <= llave) && (llave < valores[i + 1].llave))
			return buscar_Nodo(hijos[i + 1], llave);
	}
	
	return NULL;
}

// Funcion | Agregar Valor //
void agregar_Valor (Nodo *nodo, Valor valor)
{
	if (!nodo->es_hoja)
		valor.cantidad = 0;
	
	int indice = nodo->n_llaves - 1;
	Valor *valores = nodo->valores;
	
	while ((indice >= 0) && (valor.llave < valores[indice].llave))
	{
		valores[indice + 1] = valores[indice];
		indice = indice - 1;
	}
	
	valores[indice + 1] = valor;
	nodo->n_llaves++;
}

// Funcion | Ajustar Nodo //
void ajustar_Nodo (Arbol *arbol, Nodo *nodo)
{
	int grado = arbol->grado;
	
	if (nodo->n_llaves > (grado - 1))
	{
		int i, media = grado / 2;
		Valor valor = nodo->valores[media];
		
		Nodo *nuevo = crear_Nodo(grado, nodo->es_hoja);
		nuevo->padre = nodo->padre;
		nodo->n_llaves = media;
		valor.cantidad = 0;
		
		if (nodo->es_hoja)
		{
			for (i = 0; i < (grado - media); i++)
				nuevo->valores[i] = nodo->valores[i + media];
			nuevo->n_llaves = grado - media;
		}
		else
		{
			if ((grado % 2) == 0)
			{
				for (i = 0; i < (media - 1); i++)
					nuevo->valores[i] = nodo->valores[i + media + 1];
				nuevo->n_llaves = media - 1;
			}	
			else
			{
				for (i = 0; i < media; i++)
					nuevo->valores[i] = nodo->valores[i + media + 1];
				nuevo->n_llaves = media;
			}
		}
		
		if (!nodo->es_hoja)
		{
			media = nuevo->n_llaves + 1;
			
			for (i = 0; i < media; i++)
			{
				if ((grado % 2) == 0)
					nuevo->hijos[i] = nodo->hijos[i + media + 1];
				else
					nuevo->hijos[i] = nodo->hijos[i + media];	
				nuevo->hijos[i]->padre = nuevo;
			}
		}
		
		if (nodo->padre != NULL)
		{
			Nodo *padre = nodo->padre;
			
			Nodo **hijos = padre->hijos;
			int indice = padre->n_llaves;
			
			while ((indice >= 0) && (nodo != hijos[indice]))
			{
				hijos[indice + 1] = hijos[indice];
				indice = indice - 1;
			}
			
			hijos[indice + 1] = nuevo;
			
			agregar_Valor(padre, valor);
			ajustar_Nodo(arbol, padre);
		}
		else
		{
			Nodo *raiz = crear_Nodo(grado, false);
			raiz->valores[0] = valor;
			raiz->hijos[0] = nodo;
			raiz->hijos[1] = nuevo;
			raiz->n_llaves++;
			nodo->padre = raiz;
			nuevo->padre = raiz;
			arbol->raiz = raiz;
		}
	}
}

// Funcion | Verifica si existe una Llave //
bool existe_Llave (Nodo *nodo, int llave)
{
	int i;
	for (i = 0; i < nodo->n_llaves; i++)
	{
		if (llave == nodo->valores[i].llave)
			return true;
	}
	return false;
}

// Funcion | Buscar indice de Llave //
int buscar_Indice (Nodo *nodo, int llave)
{
	int i;
	for (i = 0; i < nodo->n_llaves; i++)
	{
		if (llave == nodo->valores[i].llave)
			break;
	}
	return i;
}

// Funcion | Agregar linea a Valor //
void agregar_Linea (Valor *valor, int linea)
{
	if (valor->cantidad < LINEAS)
	{
		valor->lineas[valor->cantidad] = linea;
		valor->cantidad++;
	}
	else
		printf("\n\t Error: Limite de hash multiple alcanzado");
}

// Funcion | Imprimir Registro //
void imprimir_Registro (Registro *registro)
{
	if (registro != NULL)
	{
		printf("\n  Nombre ----- %s", registro->nombre);
		printf("\n  Ocupacion -- %s", registro->ocupacion);
		printf("\n  Edad-------- %d", registro->edad);
	}
	else
		printf("\n\t Error: Registro no encontrado");
}

// Funcion | Genera los datos faltantes de un Registro //
void generar_Registro (Registro *registro, char *nombre)
{
	int longitud = strlen(nombre);
	registro->edad = (int) nombre[0] / 2;
	
	int i, j = 0;
	strcpy(registro->nombre, nombre);
	
	for (i = 0; i < longitud; i = i + 2)
		registro->ocupacion[j++] = registro->nombre[i];
	
	registro->ocupacion[j] = '\0';
}

// Funcion | Generar hash a partir de texto //
int hashear_Texto (char *texto)
{
	unsigned int i, valor = 0;
	unsigned int longitud = strlen(texto);
	
	for (i = 0; i < longitud; i++)
		valor += texto[i] * (longitud - i);
	
	return valor;
}

// Funcion | Contar lineas de un Archivo //
int contar_Lineas (FILE *archivo)
{
	int lineas = 0;
	char buffer[BUFFER];
	
	while (feof(archivo) == 0)
	{
		fgets(buffer, BUFFER, archivo);
		lineas = lineas + 1;
	}
	
    rewind(archivo);
    
    return lineas;
}

// Funcion | Cargar base de datos en la RAM //
void cargar_Ram (int linea, char *nombre_archivo)
{
	FILE *archivo = fopen(nombre_archivo, "r");
	char buffer[BUFFER];
	int i, cuenta = 0;
	
	n_lineas = contar_Lineas(archivo) - 1;
	inicio = -1;
	fin = -1;
	
	for (i = 0; i < linea; i++)
		fgets(buffer, BUFFER, archivo);
	
	while ((i < n_lineas) && (cuenta < n_registros))
	{
		fgets(buffer, BUFFER, archivo);
		buffer[strlen(buffer) - 1] = '\0';
		generar_Registro(&memoria[cuenta], buffer);
		cuenta = cuenta + 1;
		i = i + 1;
	}
	
	printf("\t Estado: Se cargaron %d registros a la RAM\n", cuenta);
	
	inicio = linea;
	fin = inicio + cuenta - 1;
	
	fclose(archivo);
	
	struct timespec tiempo;
	tiempo.tv_sec = 0;
	tiempo.tv_nsec = 1000000 * ESPERA;
	nanosleep(&tiempo, NULL);
}

// Funcion | Cargar el indice de la base de datos en un Arbol //
void cargar_Indice (Arbol *arbol, char *nombre_archivo)
{
	char buffer[BUFFER];
	int i, linea = 0;
	Valor valor;
	
	vaciar_Arbol(arbol);
	FILE *archivo = fopen(nombre_archivo, "r");
	
	n_lineas = contar_Lineas(archivo) - 1;
	inicio = -1;
	fin = -1;
	
	for (i = 0; i < n_lineas; i++)
	{
		fgets(buffer, BUFFER, archivo);
		buffer[strlen(buffer) - 1] = '\0';
		
		valor.lineas[0] = linea++;
		valor.llave = hashear_Texto(buffer);
		insertar_Arbol(arbol, valor);
	}
	
	fclose(archivo);
}

// Funcion | Busca un registro en el indice //
Registro *buscar_Registro (Arbol *arbol, char *nombre_archivo, char *nombre)
{
	int i, j, linea, llave = hashear_Texto(nombre);
	Nodo *nodo = buscar_Nodo(arbol->raiz, llave);
	
	printf("\t Hash de nombre: %d", llave);
	printf("\n\t Estado: Buscando registro...\n");
	
	for (i = 0; i < nodo->n_llaves; i++)
	{
		if (llave == nodo->valores[i].llave)
		{
			for (j = 0; j < nodo->valores[i].cantidad; j++)
			{
				linea = nodo->valores[i].lineas[j];
				
				if ((linea < inicio) || (linea > fin))
					cargar_Ram(linea, nombre_archivo);
				
				if (strcmp(nombre, memoria[linea - inicio].nombre) == 0)
					return &memoria[linea - inicio];
				
				printf("\n\t Estado: Hash multiple detectado\n");
			}
		}
	}
	
	return NULL;
}

// Funcion | Busca un registro por fuerza bruta //
Registro *forzar_Busqueda (char *nombre_archivo, char *nombre)
{
	int linea = 0;
	
	printf("\n\t Estado: Buscando registro...\n");
	cargar_Ram(linea, nombre_archivo);
	
	while (linea < n_lineas)
	{
		if (linea > fin)
			cargar_Ram(linea, nombre_archivo);
			
		if (strcmp(nombre, memoria[linea - inicio].nombre) == 0)
			return &memoria[linea - inicio];
			
		linea = linea + 1;
	}
	
	return NULL;
}

// Funcion | Pausa //
void pausa (void)
{
	printf("\n\n\t Presiona una tecla para continuar...");
	getch();
}

// Funcion | Titulo //
void titulo (void)
{
	system("cls");
	printf(" ---------------------------------------------");
	printf("\n Proyecto Final - Un billon de datos indexados ");
	printf("\n ---------------------------------------------\n\n");
}

// Funcion | Lee una entrada de texto o numerica //
void leer_Entrada (char *buffer, int longitud, bool numero)
{
	char tecla;
	int indice = 0;
	bool leer = true;
	
	buffer[indice] = '\0';
	
	while (leer)
	{
		tecla = getch();
		
		switch (tecla)
		{
			case '\r':
				if (indice > 0)
					leer = false;
			break;
			
			case '\b':
				if (indice > 0)
				{
					buffer[--indice] = '\0';
					printf("\b \b");
				}
			break;
			
			default:
				if ((numero && isdigit(tecla)) || !numero)
				{
					if (indice < (longitud - 1))
					{
						buffer[indice++] = tecla;
						buffer[indice] = '\0';
						putchar(tecla);
					}
				}
			break;
		}
	}
}

// Funcion | Menu Principal //
int menu_Principal (bool indexado, char *nombre_archivo)
{
	int opcion;
	char buffer[2];
	
	while (true)
	{
		titulo();
		
		printf("  Archivo de datos --- \"%s\"\n", nombre_archivo);
		if (indexado)
			printf("  Indexado ---------- Indice cargado\n");
		else
			printf("  Indexado ----------- Indice vacio\n");
		
		printf("\n  1) Indexar");
		printf("\n  2) Cambiar Archivo");
		printf("\n  3) Buscar Registro (Indice)");
		printf("\n  4) Buscar Registro (Fuerza Bruta)");
		printf("\n  5) Imprimir Indice");
		printf("\n  6) Salir");
		printf("\n\n ---------------------------------------------\n\n");
		printf("  Opcion: ");
		leer_Entrada(buffer, 2, true);
		opcion = strtol(buffer, NULL, 10);
		
		if ((opcion < 1) || (opcion > 6))
		{
			printf("\n\n\t Error: Opcion invalida [%d]", opcion);
			pausa();
		}
		else
			return opcion;
	}
}

bool comprobar_Nombre (char *nombre_archivo)
{
	FILE *archivo = fopen(nombre_archivo, "r");
	if (archivo == NULL)
	{
		printf("\n\n\t Error: El archivo \"%s\"no existe", nombre_archivo);
		pausa();
		return false;
	}
	else
	{
		fclose(archivo);
		return true;
	}
}

bool cambiar_Archivo (Arbol *arbol, char *nombre_archivo)
{
	int opcion;
	char buffer[BUFFER];
	bool cambio_archivo = false;
	
	while (true)
	{
		titulo();
		printf("  Archivo de datos --- \"%s\"\n", nombre_archivo);
		printf("\n  1) Cargar archivo 1");
		printf("\n  2) Cargar archivo 2");
		printf("\n  3) Cargar archivo 3");
		printf("\n  4) Ingresar nombre");
		printf("\n  5) Regresar");
		printf("\n\n ---------------------------------------------\n\n");
		printf("  Opcion: ");
		leer_Entrada(buffer, 2, true);
		opcion = strtol(buffer, NULL, 10);
		
		switch (opcion)
		{
			case 1:
				if (comprobar_Nombre(ARCHIVO_UNO))
				{
					strcpy(nombre_archivo, ARCHIVO_UNO);
					cambio_archivo = true;
					vaciar_Arbol(arbol);
				}
			break;
			case 2:
				if (comprobar_Nombre(ARCHIVO_DOS))
				{
					strcpy(nombre_archivo, ARCHIVO_DOS);
					cambio_archivo = true;
					vaciar_Arbol(arbol);
				}
			break;
			case 3:
				if (comprobar_Nombre(ARCHIVO_TRES))
				{
					strcpy(nombre_archivo, ARCHIVO_TRES);
					cambio_archivo = true;
					vaciar_Arbol(arbol);
				}
			break;
			case 4:
				titulo();
				printf("  Nuevo nombre: ");
				leer_Entrada(buffer, BUFFER, false);
				
				if (comprobar_Nombre(buffer))
				{
					strcpy(nombre_archivo, buffer);
					cambio_archivo = true;
					vaciar_Arbol(arbol);
				}
			break;
			case 5:
				return cambio_archivo;
			break;
			default:
				printf("\n\n\t Error: Opcion invalida [%d]", opcion);
				pausa();
			break;
		}
	}
}

// Funcion | Main //
int main (void)
{
	bool salir = false;
	bool indexado = false;
	Arbol *arbol = crear_Arbol(3);
	char nombre_archivo[BUFFER];
	char buffer[BUFFER];
	Registro *registro;
	
	strcpy(nombre_archivo, ARCHIVO_UNO);
	
	while (!salir)
	{
		switch (menu_Principal(indexado, nombre_archivo))
		{
			case 1:
				titulo();
				printf("\t Estado: Indexando el archivo...\n");
				cargar_Indice(arbol, nombre_archivo);
				indexado = true;
				printf("\n\t Estado: Indexacion completada\n");
				printf("\n ---------------------------------------------");
				pausa();
			break;
			case 2:
				if (cambiar_Archivo(arbol, nombre_archivo))
					indexado = false;
			break;
			case 3:
				titulo();
				if (indexado)
				{
					printf("  Nombre a buscar: ");
					leer_Entrada(buffer, BUFFER, false);
					titulo();
					registro = buscar_Registro(arbol, nombre_archivo, buffer);
					imprimir_Registro(registro);
				}
				else
				{
					printf("\t Estado: Indice vacio\n");
					printf("\n ------------------- Indice ------------------");
				}
				pausa();
			break;
			case 4:
				titulo();
				printf("  Nombre a buscar: ");
				leer_Entrada(buffer, BUFFER, false);
				titulo();
				registro = forzar_Busqueda(nombre_archivo, buffer);
				imprimir_Registro(registro);
				pausa();
			break;
			case 5:
				titulo();
				imprimir_Arbol(arbol);
				pausa();
			break;
			case 6:
				salir = true;
			break;
		}
	}
	
	borrar_Arbol(arbol);
	return 0;
}
