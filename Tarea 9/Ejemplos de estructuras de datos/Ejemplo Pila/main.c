#include <stdio.h>
#include <stdlib.h>

#define MAX 5

int main(void) {
    int stack[MAX];
    int top = 0; // índice de inserción (también número de elementos)
    int x;

    printf("PILA. Introduce %i numeros:\n", MAX);
    for (int i = 0; i < MAX; ++i) {
        if (scanf("%d", &x) != 1) { fprintf(stderr, "Entrada inválida\n"); return 1; }
        // push
        if (top >= MAX) { fprintf(stderr, "Stack overflow\n"); return 1; }
        stack[top++] = x;
    }

    printf("Salida de la pila (orden de pop):\n");
    while (top > 0) {
        // pop
        int val = stack[--top];
        printf("%d\n", val);
    }

    return 0;
}
