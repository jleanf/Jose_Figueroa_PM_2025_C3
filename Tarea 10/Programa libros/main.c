#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATAFILE "libros.txt"
#define TEXTOMAX 2048

typedef struct Libro
{
    int ano;
    char *autor;
    char *titulo;
    char *descripcion;
    double precio;
    int codigo;
    struct Libro *siguiente;
} Libro;

/* prototipos */
Libro *cargar_libros(void);
void guardar_libros(Libro *cabeza);
void liberar_lista(Libro *cabeza);
void imprimir_resumen(Libro *cabeza);
void agregar_libro(Libro **cabeza);
void eliminar_libro(Libro **cabeza);
void buscar_libro(Libro *cabeza);
char *dup_cadena(const char *s);

int main(void)
{
    Libro *cabeza = cargar_libros();
    int opcion;

    while (1)
    {
        printf("\n=== LISTA DE LIBROS GUARDADOS ===\n");
        imprimir_resumen(cabeza);
        printf("\nOpciones:\n");
        printf("1 - Agregar libro\n");
        printf("2 - Eliminar libro\n");
        printf("3 - Buscar libro\n");
        printf("0 - Salir\n");
        printf("Selecciona una opcion: ");
        if (scanf("%d", &opcion) != 1) opcion = -1;
        /* consumir resto de la linea */
        getchar();

        if (opcion == 1)
        {
            agregar_libro(&cabeza);
            guardar_libros(cabeza);
        }
        else if (opcion == 2)
        {
            eliminar_libro(&cabeza);
            guardar_libros(cabeza);
        }
        else if (opcion == 3)
        {
            buscar_libro(cabeza);
        }
        else if (opcion == 0)
        {
            guardar_libros(cabeza);
            liberar_lista(cabeza);
            printf("Hasta luego.\n");
            break;
        }
        else
        {
            printf("Opcion no valida.\n");
        }
    }
    return 0;
}

/* duplica cadena en memoria dinámica */
char *dup_cadena(const char *s)
{
    size_t n = strlen(s);
    char *r = malloc(n + 1);
    strcpy(r, s);
    return r;
}

/* cargar libros desde archivo de texto simple */
Libro *cargar_libros(void)
{
    FILE *f = fopen(DATAFILE, "r");
    if (!f) return NULL;
    char linea[TEXTOMAX];
    Libro *cabeza = NULL;
    Libro *ultimo = NULL;

    while (fgets(linea, TEXTOMAX, f))
    {
        /* quitar posible '\n' final */
        size_t L = strlen(linea);
        if (L > 0 && linea[L-1] == '\n') linea[L-1] = '\0';

        /* separar por '|' */
        char *p = strtok(linea, "|");
        if (!p) continue;
        int codigo = atoi(p);

        p = strtok(NULL, "|");
        if (!p) continue;
        int ano = atoi(p);

        p = strtok(NULL, "|");
        if (!p) continue;
        double precio = atof(p);

        p = strtok(NULL, "|");
        if (!p) continue;
        char *autor = p;

        p = strtok(NULL, "|");
        if (!p) continue;
        char *titulo = p;

        p = strtok(NULL, ""); /* descripcion (resto) */
        char *descripcion = p ? p : "";

        /* crear nodo */
        Libro *node = malloc(sizeof(Libro));
        node->codigo = codigo;
        node->ano = ano;
        node->precio = precio;
        node->autor = dup_cadena(autor);
        node->titulo = dup_cadena(titulo);
        node->descripcion = dup_cadena(descripcion);
        node->siguiente = NULL;

        if (!cabeza) cabeza = node;
        else ultimo->siguiente = node;
        ultimo = node;
    }

    fclose(f);
    return cabeza;
}

/* guardar lista completa sobreescribiendo archivo */
void guardar_libros(Libro *cabeza)
{
    FILE *f = fopen(DATAFILE, "w");
    if (!f)
    {
        printf("No se pudo abrir archivo para guardar.\n");
        return;
    }
    for (Libro *p = cabeza; p; p = p->siguiente)
    {
        /* formateo: codigo|ano|precio|autor|titulo|descripcion\n */
        fprintf(f, "%d|%d|%.2f|%s|%s|%s\n",
                p->codigo, p->ano, p->precio,
                p->autor, p->titulo, p->descripcion);
    }
    fclose(f);
}

/* liberar memoria de la lista */
void liberar_lista(Libro *cabeza)
{
    while (cabeza)
    {
        Libro *t = cabeza;
        cabeza = cabeza->siguiente;
        free(t->autor);
        free(t->titulo);
        free(t->descripcion);
        free(t);
    }
}

/* imprimir resumen (titulo, autor, ano) */
void imprimir_resumen(Libro *cabeza)
{
    if (!cabeza)
    {
        printf("No hay libros guardados.\n");
        return;
    }
    int i = 1;
    for (Libro *p = cabeza; p; p = p->siguiente, ++i)
    {
        printf("%d) \"%s\", %s (%d)\n", i, p->titulo, p->autor, p->ano);
    }
}

/* agregar libro (lectura simple con scanf) */
void agregar_libro(Libro **cabeza)
{
    char buf[TEXTOMAX];
    printf("\n--- Agregar libro ---\n");
    printf("Titulo: ");
    scanf(" %[^\n]", buf);
    char *titulo = dup_cadena(buf);

    printf("Autor: ");
    scanf(" %[^\n]", buf);
    char *autor = dup_cadena(buf);

    printf("ano: ");
    int ano;
    scanf("%d", &ano);
    getchar();

    printf("Codigo (numero): ");
    int codigo;
    scanf("%d", &codigo);
    getchar();

    printf("Precio (ej: 12.50): ");
    double precio;
    scanf("%lf", &precio);
    getchar();

    printf("Descripcion: ");
    scanf(" %[^\n]", buf);
    char *descripcion = dup_cadena(buf);

    Libro *n = malloc(sizeof(Libro));
    n->titulo = titulo;
    n->autor = autor;
    n->ano = ano;
    n->codigo = codigo;
    n->precio = precio;
    n->descripcion = descripcion;
    n->siguiente = NULL;

    /* anexar al final */
    if (*cabeza == NULL) *cabeza = n;
    else
    {
        Libro *p = *cabeza;
        while (p->siguiente) p = p->siguiente;
        p->siguiente = n;
    }
    printf("Libro agregado.\n");
}

/* eliminar libro por numero en lista */
void eliminar_libro(Libro **cabeza)
{
    if (*cabeza == NULL)
    {
        printf("No hay libros para eliminar.\n");
        return;
    }
    imprimir_resumen(*cabeza);
    printf("Seleccione el numero del libro a eliminar (0 cancelar): ");
    int sel;
    scanf("%d", &sel);
    getchar();
    if (sel <= 0)
    {
        printf("Operacion cancelada.\n");
        return;
    }

    int i = 1;
    Libro *p = *cabeza;
    Libro *prev = NULL;
    while (p && i < sel)
    {
        prev = p;
        p = p->siguiente;
        ++i;
    }
    if (!p)
    {
        printf("Numero invalido.\n");
        return;
    }
    if (prev == NULL) *cabeza = p->siguiente;
    else prev->siguiente = p->siguiente;

    free(p->autor);
    free(p->titulo);
    free(p->descripcion);
    free(p);
    printf("Libro eliminado.\n");
}

/* buscar por autor (subcadena) o por titulo (subcadena) */
void buscar_libro(Libro *cabeza)
{
    if (!cabeza)
    {
        printf("No hay libros guardados.\n");
        return;
    }
    printf("\n--- Buscar libro ---\n");
    printf("1 - Por autor\n2 - Por titulo\n0 - Cancelar\n");
    printf("Selecciona: ");
    int op;
    scanf("%d", &op);
    getchar();
    if (op == 0)
    {
        printf("Busqueda cancelada.\n");
        return;
    }

    Libro **matches = NULL;
    int mcount = 0;
    char buf[TEXTOMAX];

    if (op == 1)
    {
        printf("Introduce el autor del libro: ");
        scanf(" %[^\n]", buf);
        for (Libro *p = cabeza; p; p = p->siguiente)
        {
            if (strstr(p->autor, buf) != NULL)
            {
                matches = realloc(matches, sizeof(Libro*) * (mcount + 1));
                matches[mcount++] = p;
            }
        }
    }
    else if (op == 2)
    {
        printf("Introduce el titulo del libro: ");
        scanf(" %[^\n]", buf);
        for (Libro *p = cabeza; p; p = p->siguiente)
        {
            if (strstr(p->titulo, buf) != NULL)
            {
                matches = realloc(matches, sizeof(Libro*) * (mcount + 1));
                matches[mcount++] = p;
            }
        }
    }
    else
    {
        printf("Opcion no valida.\n");
        return;
    }

    if (mcount == 0)
    {
        printf("No se encontraron coincidencias.\n");
        free(matches);
        return;
    }

    printf("Resultados:\n");
    for (int i = 0; i < mcount; ++i)
    {
        printf("%d) \"%s\", %s (%d)\n", i+1,
               matches[i]->titulo, matches[i]->autor, matches[i]->ano);
    }
    printf("Seleccione un libro (numero) para ver detalles, 0 para cancelar: ");
    int sel;
    scanf("%d", &sel);
    getchar();
    if (sel <= 0 || sel > mcount)
    {
        printf("Operacion cancelada.\n");
        free(matches);
        return;
    }
    Libro *e = matches[sel - 1];
    printf("\n--- Detalle del libro ---\n");
    printf("Titulo: %s\n", e->titulo);
    printf("Autor: %s\n", e->autor);
    printf("Ano: %d\n", e->ano);
    printf("Codigo: %d\n", e->codigo);
    printf("Precio: %.2f\n", e->precio);
    printf("Descripcion: %s\n", e->descripcion);
    printf("-------------------------\n");
    free(matches);
}
