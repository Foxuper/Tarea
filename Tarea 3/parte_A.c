#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Tipo de dato | Booleano //
typedef int bool;
#define true 1
#define false 0

// Definicion | Longitud de buffer //
#define BUFF_CADENA 100

// Tipo de dato | Valor //
typedef double Valor;

// Tipo de dato | Operador //
typedef enum Operador
{
	suma,        // '+'
	resta,       // '-'
	producto,    // '*'
	division,    // '/'
	potencia,    // '^'
	p_izquierdo, // '('
	p_derecho,   // ')'
	cantidad_op  // Cantidad de operadores
} Operador;

// Constante | Arreglo de operadores //
const char ct_operadores[cantidad_op] =
{
	'+',
	'-',
	'*',
	'/',
	'^',
	'(',
	')'
};

// Tipo de dato | Tipo (de [Elemento]) //
typedef enum Tipo
{
	valor,
	operador
} Tipo;

// Tipo de dato | Elemento (de una [Pila]) //
typedef struct Elemento
{
	Tipo tipo;
	void *dato;
	struct Elemento *siguiente;
} Elemento;

// Tipo de dato | Pila //
typedef struct Pila
{
	Elemento *tope;
} Pila;

// ----------------------------------------------------------------- //

// Definiciones | Funciones | Elemento //
Elemento *crear_Elemento  (Tipo tipo, void *dato);
void      borrar_Elemento (Elemento *E);

// Definiciones | Funciones | Pila //
Pila     *crear_Pila    (void);
bool      esta_vacia    (Pila *P);
void      apilar        (Pila *P, Elemento *E);
Elemento *desapilar     (Pila *P);
Elemento *mirar         (Pila *P);
void      invertir_Pila (Pila *(*P));
void      borrar_Pila   (Pila *P);

// Definiciones | Funciones Auxiliares //
bool     es_valor      (char c);
bool     es_operador   (char c);
Valor    leer_valor    (char *cadena, int *indice);
Operador leer_operador (char c);

// Definiciones | Funciones de Conversion //
Pila *cadena_a_pila (char *cadena);
char *pila_a_cadena (Pila *P);

// Definiciones | Funciones Auxiliares //
void vaciar_hasta_parentesis (Pila *Pila_Oper, Pila *Postfija);
void vaciar_operaciones (Pila *Pila_Oper, Pila *Postfija, Operador Op);

// Definiciones | Funciones Principales //
char *infija_a_postfija (char *cadena_infija);
Valor evaluar_infija    (char *cadena_infija);

// ----------------------------------------------------------------- //

// Funcion | Elemento | Crear //
Elemento *crear_Elemento (Tipo tipo, void *dato)
{
	Elemento *E = malloc(sizeof(Elemento));
	E->tipo = tipo;
	
	switch (tipo)
	{
		case valor:
			E->dato = malloc(sizeof(Valor));
			memcpy(E->dato, dato, sizeof(Valor));
		break;
		case operador:
			E->dato = malloc(sizeof(Operador));
			memcpy(E->dato, dato, sizeof(Operador));
		break;
	}
	
	return E;
}

// Funcion | Elemento | Borrar //
void borrar_Elemento (Elemento *E)
{
	free(E->dato);
	free(E);
}

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
void apilar (Pila *P, Elemento *E)
{
	E->siguiente = P->tope;
	P->tope = E;
}

// Funcion | Pila | Desapilar //
Elemento *desapilar (Pila *P)
{
	Elemento *E = P->tope;
	P->tope = P->tope->siguiente;
	return E;
}

// Funcion | Pila | Mirar //
Elemento *mirar (Pila *P)
{
	return P->tope;
}

// Funcion | Invertir Pila //
void invertir_Pila (Pila *(*P))
{
	Elemento *E;
	Pila *Actual = *P;
	Pila *Inversa = crear_Pila();
	
	while (!esta_vacia(Actual))
	{
		E = desapilar(Actual);
		apilar(Inversa, E);
	}
	
	borrar_Pila(Actual);
	*P = Inversa;
}

// Funcion | Pila | Borrar //
void borrar_Pila (Pila *P)
{
	Elemento *E;
	
	while (!esta_vacia(P))
	{
		E = desapilar(P);
		borrar_Elemento(E);
	}
	
	free(P);
}

// Funcion | Auxiliar | Compara si un caracter es un valor //
bool es_valor (char c)
{
	char valor = c - '0';
	
	if ((valor >= 0) && (valor <= 9))
		return true;
	else
		return false;
}

// Funcion | Auxiliar | Compara si un caracter es un operador //
bool es_operador (char c)
{
	Operador Op = suma;
	
	while (Op < cantidad_op)
	{
		if (c == ct_operadores[Op])
			return true;
		else
			Op++;
	}
	
	return false;
}

// Funcion | Auxiliar | Lee un valor //
Valor leer_valor (char *cadena, int *indice)
{
	Valor Val;
	int i_cadena = 0;
	int i_buffer = 0;
	char buffer[BUFF_CADENA];
	char c = cadena[i_cadena++];
	
	while (es_valor(c) || (c == '.'))
	{
		if (i_buffer == (BUFF_CADENA - 1))
		{
			printf("\n Error: Buffer insuficiente");
			break;
		}
		
		buffer[i_buffer++] = c;
		c = cadena[i_cadena++];	
	}
	
	*indice += i_cadena - 1;
	buffer[i_buffer] = '\0';
	sscanf(buffer, "%lg", &Val);
	
	return Val;
}

// Funcion | Auxiliar | Lee un operador //
Operador leer_operador (char c)
{
	Operador Op = suma;
	
	while (Op < cantidad_op)
	{
		if (c == ct_operadores[Op])
			return Op;
		else
			Op++;
	}
	
	return Op;
}

// Funcion | Convierte una cadena a una pila //
Pila *cadena_a_pila (char *cadena)
{
	int i = 0;
	char c = cadena[i++];
	Pila *P = crear_Pila();
	
	Valor Val;
	Operador Op;
	Elemento *E;
	
	while (c != '\0')
	{
		if (es_valor(c))
		{
			i = i - 1;
			Val = leer_valor(&cadena[i], &i);
			E = crear_Elemento(valor, &Val);
			apilar(P, E);
		}
		
		if (es_operador(c))
		{
			Op = leer_operador(c);
			E = crear_Elemento(operador, &Op);
			apilar(P, E);
		}
		
		c = cadena[i++];
	}
	
	invertir_Pila(&P);
	return P;
}

// Funcion | Convierte una pila a una cadena //
char *pila_a_cadena (Pila *P)
{
	int lon_buffer, longitud = 1;
	char *cadena = calloc(longitud, 1);
	char buffer[BUFF_CADENA];
	
	Valor Val;
	Operador Op;
	Elemento *E = P->tope;
	
	while (E != NULL)
	{
		switch (E->tipo)
		{
			case valor:
				memcpy(&Val, E->dato, sizeof(Valor));
				lon_buffer = snprintf(buffer, BUFF_CADENA, "%lg ", Val);
			break;
			case operador:
				memcpy(&Op, E->dato, sizeof(Operador));
				lon_buffer = snprintf(buffer, BUFF_CADENA, "%c ", ct_operadores[Op]);
			break;
		}
		
		if ((lon_buffer <= 0) || (lon_buffer >= BUFF_CADENA))
		{
			printf("\n Error: Buffer insuficiente");
			lon_buffer = BUFF_CADENA - 1;
			buffer[lon_buffer] = '\0';
		}
		
		longitud += lon_buffer;
		cadena = realloc(cadena, longitud);
		strcat(cadena, buffer);
		
		E = E->siguiente;
	}
	
	if (strlen(cadena) > 0)
	{
		longitud = longitud - 1;
		cadena[longitud - 1] = '\0';
		cadena = realloc(cadena, longitud);
	}
	
	return cadena;
}

// Funcion | Auxiliar | Vaciar pila hasta parentesis izquierdo //
void vaciar_hasta_parentesis (Pila *Pila_Oper, Pila *Postfija)
{
	Elemento *E;
	Operador Op;
	bool encontrado = false;
	
	while (!esta_vacia(Pila_Oper) && !encontrado)
	{
		E = desapilar(Pila_Oper);
		memcpy(&Op, E->dato, sizeof(Operador));
		
		if (Op == p_izquierdo)
		{
			borrar_Elemento(E);
			encontrado = true;
		}
		else
			apilar(Postfija, E);
	}
}

// Funcion | Auxiliar | Vaciar operaciones con mas prioridad //
void vaciar_operaciones (Pila *Pila_Oper, Pila *Postfija, Operador Op)
{
	Elemento *E;
	Operador Op_Actual;
	
	while (!esta_vacia(Pila_Oper))
	{
		E = mirar(Pila_Oper);
		memcpy(&Op_Actual, E->dato, sizeof(Operador));
		
		if ((Op_Actual >= Op) && (Op_Actual != p_izquierdo))
		{
			E = desapilar(Pila_Oper);
			apilar(Postfija, E);
		}
		else
			break;
	}
}

// Funcion | Convierte una cadena infija a una postfija //
char *infija_a_postfija (char *cadena_infija)
{
	Pila *Infija = cadena_a_pila(cadena_infija);
	Pila *Pila_Oper = crear_Pila();
	Pila *Postfija = crear_Pila();
	
	char *cadena_postfija;
	Elemento *E;
	Operador Op;
	
	while (!esta_vacia(Infija))
	{
		E = desapilar(Infija);
		
		switch (E->tipo)
		{
			case valor:
				apilar(Postfija, E);
			break;
			case operador:
				memcpy(&Op, E->dato, sizeof(Operador));
				
				switch (Op)
				{
					case p_izquierdo:
						apilar(Pila_Oper, E);
					break;
					case p_derecho:
						borrar_Elemento(E);
						vaciar_hasta_parentesis(Pila_Oper, Postfija);
					break;
					default:
						vaciar_operaciones(Pila_Oper, Postfija, Op);
						apilar(Pila_Oper, E);
					break;
				}
			break;
		}
	}
	
	while (!esta_vacia(Pila_Oper))
	{
		E = desapilar(Pila_Oper);
		apilar(Postfija, E);
	}
	
	invertir_Pila(&Postfija);
	cadena_postfija = pila_a_cadena(Postfija);
	
	borrar_Pila(Infija);
	borrar_Pila(Pila_Oper);
	borrar_Pila(Postfija);
	
	return cadena_postfija;
}

// Funcion | Evalua una cadena infija y regresa el resultado //
Valor evaluar_infija (char *cadena_infija)
{
	char *cadena_postfija = infija_a_postfija(cadena_infija);
	
	Pila *Postfija = cadena_a_pila(cadena_postfija);
	Pila *Resultado = crear_Pila();
	free(cadena_postfija);
	
	Valor Val_a, Val_b, Val_c;
	Elemento *E;
	Operador Op;
	
	while (!esta_vacia(Postfija))
	{
		E = desapilar(Postfija);
		
		switch (E->tipo)
		{
			case valor:
				apilar(Resultado, E);
			break;
			case operador:
				memcpy(&Op, E->dato, sizeof(Operador));
				
				borrar_Elemento(E);
				E = desapilar(Resultado);
				memcpy(&Val_b, E->dato, sizeof(Valor));
				
				borrar_Elemento(E);
				E = desapilar(Resultado);
				memcpy(&Val_a, E->dato, sizeof(Valor));
				
				switch (Op)
				{
					case suma:
						Val_c = Val_a + Val_b;
					break;
					case resta:
						Val_c = Val_a - Val_b;
					break;
					case producto:
						Val_c = Val_a * Val_b;
					break;
					case division:
						Val_c = Val_a / Val_b;
					break;
					case potencia:
						Val_c = pow(Val_a, Val_b);
					break;
					default:
						// Sin accion //
					break;
				}
				
				memcpy(E->dato, &Val_c, sizeof(Valor));
				apilar(Resultado, E);
			break;
		}
	}
	
	E = mirar(Resultado);
	memcpy(&Val_c, E->dato, sizeof(Valor));
	
	borrar_Pila(Resultado);
	borrar_Pila(Postfija);
	
	return Val_c;
}

// -------- MAIN -------- //
int main (void)
{
	char *postfija, ejemplo[] = "((5.4 + 11) / 2.3 + 8) - 7 ^ (1/2)";
	char cadena[BUFF_CADENA];
	
	printf("\n Notas:\n");
	printf("\n   - Se aceptan los siguientes caracteres:");
	printf("\n     '0' ... '9', '(', ')', '+', '-', '*', '/', '^', '.' y ' '");
	printf("\n   - La entrada no se revisa para errores de sintaxis");
	printf("\n   - Se aceptan numeros reales");
	printf("\n\n Presione una tecla para continuar . . .");
	getchar();
	
	printf("\n Ejemplo:\n");
	printf("\n   Eq =       %s", ejemplo);
	postfija = infija_a_postfija(ejemplo);
	printf("\n   Postfija = %s", postfija);
	free(postfija);
	
	printf("\n   Eq =       %g", evaluar_infija(ejemplo));
	printf("\n\n Presione una tecla para continuar . . .");
	getchar();
	
	printf("\n Ingrese una ecuacion infija:\n");
	printf("\n   Eq =       ");
	fgets(cadena, 200, stdin);
	
	postfija = infija_a_postfija(cadena);
	printf("   Postfija = %s", postfija);
	free(postfija);
	
	printf("\n   Eq =       %g", evaluar_infija(cadena));
	printf("\n\n Presione una tecla para salir . . .");
	getchar();

	return 0;
}
