#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *name;
    struct Node **children;
    int n_children;
    int capacity;
} Node;

Node* create_node(const char *name)
{
    Node *n = (Node*)malloc(sizeof(Node));
    n->name = strdup(name);
    n->children = NULL;
    n->n_children = 0;
    n->capacity = 0;
    return n;
}

void add_child(Node *parent, Node *child)
{
    if (parent->n_children == parent->capacity)
    {
        int newcap = parent->capacity == 0 ? 2 : parent->capacity * 2;
        parent->children = (Node**)realloc(parent->children, newcap * sizeof(Node*));
        parent->capacity = newcap;
    }
    parent->children[parent->n_children++] = child;
}

void print_tree(Node *raiz, int depth)
{
    if (!raiz) return;
    for (int i = 0; i < depth; ++i) printf("\t"); // imprime un '\t' por cada nivel
    printf("%s\n", raiz->name);
    for (int i = 0; i < raiz->n_children; ++i)
    {
        print_tree(raiz->children[i], depth + 1);
    }
}

void free_tree(Node *raiz)
{
    if (!raiz) return;
    for (int i = 0; i < raiz->n_children; ++i) free_tree(raiz->children[i]);
    free(raiz->children);
    free(raiz->name);
    free(raiz);
}

int main(void)
{
    Node *raiz = create_node("raiz");

    Node *a = create_node("A");
    Node *b = create_node("B");
    Node *c = create_node("C");
    add_child(raiz, a);
    add_child(raiz, b);
    add_child(raiz, c);

    add_child(a, create_node("A1"));
    add_child(a, create_node("A2"));

    Node *b1 = create_node("B1");
    add_child(b, b1);
    add_child(b1, create_node("B1a"));

    add_child(c, create_node("C1"));

    print_tree(raiz, 0);

    free_tree(raiz);
    return 0;
}
