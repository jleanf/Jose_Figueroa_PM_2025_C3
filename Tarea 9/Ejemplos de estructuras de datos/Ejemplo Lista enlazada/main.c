#include <stdio.h>
#include <stdlib.h>

// Nodo de la lista
typedef struct Nodo
{
    int dato;
    struct Nodo* siguiente;
} Nodo;

// Crear un nuevo nodo
Nodo* crear_nodo(int valor)
{
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->dato = valor;
    nuevo->siguiente = NULL;
    return nuevo;
}

// Insertar al inicio
void insertar_inicio(Nodo** cabeza, int valor)
{
    Nodo* nuevo = crear_nodo(valor);
    nuevo->siguiente = *cabeza;
    *cabeza = nuevo;
}

// Imprimir la lista
void imprimir_lista(Nodo* cabeza)
{
    Nodo* actual = cabeza;
    while (actual != NULL)
    {
        printf("%i -> ", actual->dato);
        actual = actual->siguiente;
    }
    printf("NULL\n");
}

int main()
{
    Nodo* lista = NULL;

    int valor;
    printf("Inserta un valor:");
    scanf("%i",&valor);
    insertar_inicio(&lista, valor);
    printf("Inserta un valor:");
    scanf("%i",&valor);
    insertar_inicio(&lista, valor);
    printf("Inserta un valor:");
    scanf("%i",&valor);
    insertar_inicio(&lista, valor);

    imprimir_lista(lista);

    return 0;
}
