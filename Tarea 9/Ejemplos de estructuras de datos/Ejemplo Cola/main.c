#include <stdio.h>
#include <stdlib.h>

#define MAX 5

int main(void)
{
    int queue[MAX];
    int head = 0; // índice del primer elemento
    int tail = 0; // índice donde insertar el siguiente
    int count = 0;
    int x;

    printf("COLA (FIFO). Introduce %i numeros:\n", MAX);
    for (int i = 0; i < MAX; ++i)
    {
        if (scanf("%d", &x) != 1)
        {
            fprintf(stderr, "Entrada invalida\n");
            return 1;
        }
        if (count == MAX)
        {
            fprintf(stderr, "Queue full\n");
            return 1;
        }
        queue[tail] = x;
        tail = (tail + 1) % MAX;
        count++;
    }

    printf("Salida de la cola (orden de dequeue):\n");
    while (count > 0)
    {
        int val = queue[head];
        head = (head + 1) % MAX;
        count--;
        printf("%d\n", val);
    }

    return 0;
}
