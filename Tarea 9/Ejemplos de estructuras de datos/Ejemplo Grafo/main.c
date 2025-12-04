#include <stdio.h>
#include <stdlib.h>

/* Nodo de la lista de adyacencia */
typedef struct AdjNode
{
    int dest;
    struct AdjNode* next;
} AdjNode;

/* Grafo: número de vértices y array de punteros a listas de adyacencia */
typedef struct Graph
{
    int V;
    AdjNode** adj; // adj[v] es la cabeza de la lista de vecinos de v
} Graph;

/* Crear un nuevo nodo de adyacencia */
AdjNode* newAdjNode(int dest)
{
    AdjNode* node = (AdjNode*)malloc(sizeof(AdjNode));
    if (!node)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    node->dest = dest;
    node->next = NULL;
    return node;
}

/* Crear grafo con V vértices */
Graph* createGraph(int V)
{
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if (!g)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    g->V = V;
    g->adj = (AdjNode**)malloc(V * sizeof(AdjNode*));
    if (!g->adj)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < V; ++i) g->adj[i] = NULL;
    return g;
}

/* Añadir arista al grafo.
   Si undirected = 1 -> añade en ambos sentidos (grafo no dirigido).
   Si undirected = 0 -> añade solo src -> dest (grafo dirigido). */
void addEdge(Graph* g, int src, int dest, int undirected)
{
    if (!g || src < 0 || src >= g->V || dest < 0 || dest >= g->V) return;

    /* insertar al inicio en src */
    AdjNode* node = newAdjNode(dest);
    node->next = g->adj[src];
    g->adj[src] = node;

    if (undirected)
    {
        AdjNode* rev = newAdjNode(src);
        rev->next = g->adj[dest];
        g->adj[dest] = rev;
    }
}

/* Imprimir grafo */
void printGraph(const Graph* g)
{
    if (!g) return;
    for (int v = 0; v < g->V; ++v)
    {
        printf("%d ->", v);
        printf("(");
        for (AdjNode* p = g->adj[v]; p != NULL; p = p->next)
        {
            printf("-%d", p->dest);
        }
        printf("-)");
        printf("\n");
    }
}

/* Liberar memoria del grafo */
void freeGraph(Graph* g)
{
    if (!g) return;
    for (int v = 0; v < g->V; ++v)
    {
        AdjNode* p = g->adj[v];
        while (p)
        {
            AdjNode* tmp = p;
            p = p->next;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

/* Ejemplo de uso */
int main(void)
{
    int V = 5;
    Graph* g = createGraph(V);

    /* Crear algunas aristas (no dirigido) */
    addEdge(g, 0, 1, 1);
    addEdge(g, 0, 4, 1);
    addEdge(g, 1, 2, 1);
    addEdge(g, 1, 3, 1);
    addEdge(g, 1, 4, 1);
    addEdge(g, 2, 3, 1);
    addEdge(g, 3, 4, 1);

    printf("Grafo (lista de adyacencia):\n");
    printGraph(g);

    freeGraph(g);
    return 0;
}
