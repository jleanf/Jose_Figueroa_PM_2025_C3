#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char nombre[20];
    float precio;
} producto;

void anadirProducto(FILE *);
producto *cargarInv(FILE *, int *);
void imprimirInv(FILE *);

int main()
{
    FILE *archinv;
    archinv = fopen("inventario.dat", "a+b");
    if (!archinv) {
        perror("Error al abrir inventario.dat");
        return 1;
    }

    int res;

    printf("Desea anadir un producto nuevo al inventario? (0-No 1-Si)\n");
    scanf("%d", &res);
    if(res)
    {
        anadirProducto(archinv);
    }

    printf("Desea imprimir el inventario? (0-No 1-Si)\n");
    scanf("%d", &res);
    if(res)
    {
        imprimirInv(archinv);
    }

    fclose(archinv);
    return 0;
}

void anadirProducto(FILE *archinv)
{
    producto newprod;
    printf("Nombre: ");
    scanf("%s", newprod.nombre);
    printf("Precio: ");
    scanf("%f", &newprod.precio);

    fwrite(&newprod, sizeof(producto), 1, archinv);
}

producto *cargarInv(FILE *archinv, int *count)
{
    *count = 0;
    if (!archinv) return NULL;

    if (fseek(archinv, 0, SEEK_END) != 0) return NULL;
    long filesize = ftell(archinv);
    if (filesize <= 0) {
        rewind(archinv);
        return NULL;
    }

    long records = filesize / sizeof(producto);
    if (records <= 0) {
        rewind(archinv);
        return NULL;
    }

    producto *inv = malloc(records * sizeof(producto));
    if (!inv) {
        perror("malloc");
        rewind(archinv);
        return NULL;
    }

    rewind(archinv);
    size_t read = fread(inv, sizeof(producto), (size_t)records, archinv);
    if (read == 0) {
        free(inv);
        rewind(archinv);
        return NULL;
    }

    *count = (int)read;
    return inv;
}

void imprimirInv(FILE *archinv)
{
    producto *inv = NULL;
    int n = 0;

    inv = cargarInv(archinv, &n);
    if (!inv || n == 0) {
        printf("El inventario esta vacio.\n");
        free(inv);
        return;
    }

    printf("Inventario (%d productos):\n", n);
    for (int i = 0; i < n; ++i) {
        printf("%d) Nombre: %s\tPrecio: %.2f\n", i+1, inv[i].nombre, inv[i].precio);
    }

    free(inv);
}
