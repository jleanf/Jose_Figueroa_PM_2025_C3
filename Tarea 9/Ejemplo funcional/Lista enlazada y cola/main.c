#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *desc;
    struct Node *next;
} Node;

typedef struct Queue
{
    Node *head;
    Node *tail;
    int size;
} Queue;

static char *xstrdup(const char *s)
{
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *r = malloc(n);
    if (!r) return NULL;
    memcpy(r, s, n);
    return r;
}

Queue* create_queue(void)
{
    Queue *q = malloc(sizeof(Queue));
    if (!q)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

void enqueue(Queue *q, const char *s)
{
    Node *n = malloc(sizeof(Node));
    if (!n)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    n->desc = xstrdup(s);
    n->next = NULL;
    if (q->tail) q->tail->next = n;
    else q->head = n;
    q->tail = n;
    q->size++;
}

int is_empty(const Queue *q)
{
    return q->head == NULL;
}

char* peek(const Queue *q)
{
    if (is_empty(q)) return NULL;
    return q->head->desc;
}

void dequeue_and_free(Queue *q)
{
    if (is_empty(q)) return;
    Node *n = q->head;
    q->head = n->next;
    if (q->head == NULL) q->tail = NULL;
    free(n->desc);
    free(n);
    q->size--;
}

void free_queue(Queue *q)
{
    while (!is_empty(q)) dequeue_and_free(q);
    free(q);
}

/* lee una linea de stdin y remueve el '\n' final si existe */
void read_line(char *buf, size_t cap)
{
    if (!fgets(buf, (int)cap, stdin))
    {
        buf[0] = '\0';
        return;
    }
    size_t L = strlen(buf);
    if (L > 0 && buf[L-1] == '\n') buf[L-1] = '\0';
}

void menu(void)
{
    printf("\n--- SISTEMA DE PEDIDOS ---\n");
    printf("1) Mostrar primer pedido pendiente\n");
    printf("2) Introducir pedido\n");
    printf("3) Pedido realizado (eliminar primer pedido)\n");
    printf("4) Salir\n");
    printf("Elige una opcion: ");
}

int main(void)
{
    Queue *q = create_queue();
    char input[256];

    while (1)
    {
        menu();
        read_line(input, sizeof(input));
        int opt = atoi(input);

        switch (opt)
        {
        case 1:
        {
            char *p = peek(q);
            if (p) printf("Primer pedido pendiente: %s\n", p);
            else printf("No hay pedidos\n");
            break;
        }
        case 2:
        {
            printf("Introduce la descripcion del pedido: ");
            read_line(input, sizeof(input));
            if (input[0] == '\0')
            {
                printf("Pedido vacio, no se anadio.\n");
            }
            else
            {
                enqueue(q, input);
                printf("Pedido anadido.\n");
            }
            break;
        }
        case 3:
        {
            if (is_empty(q))
            {
                printf("No hay pedidos para marcar como realizados.\n");
            }
            else
            {
                printf("Pedido realizado (eliminando): %s\n", peek(q));
                dequeue_and_free(q);
                if (is_empty(q)) printf("No hay mas pedidos.\n");
                else printf("Siguiente pedido pendiente: %s\n", peek(q));
            }
            break;
        }
        case 4:
        {
            free_queue(q);
            printf("Saliendo. Adios.\n");
            return 0;
        }
        default:
            printf("Opcion no valida. Elige 1-4.\n");
        }
    }

    return 0;
}
