#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Entrada de la lista encadenada (colisión por chaining) */
typedef struct Entry
{
    char *key;
    int value;
    struct Entry *next;
} Entry;

/* Tabla hash */
typedef struct HashTable
{
    int size;        // número de buckets
    Entry **buckets; // array de punteros a Entry (listas enlazadas)
} HashTable;

/* strdup portable */
static char *xstrdup(const char *s)
{
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *r = malloc(n);
    if (r) memcpy(r, s, n);
    return r;
}

/* Función hash: djb2 (simple y buena práctica para strings) */
static unsigned long hash_djb2(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

/* Crear tabla */
HashTable* ht_create(int size)
{
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->size = size;
    ht->buckets = calloc(size, sizeof(Entry*));
    if (!ht->buckets)
    {
        free(ht);
        return NULL;
    }
    return ht;
}

/* Buscar entrada por clave; devuelve puntero a Entry o NULL */
Entry* ht_find_entry(HashTable *ht, const char *key)
{
    unsigned long h = hash_djb2(key) % (unsigned long)ht->size;
    Entry *e = ht->buckets[h];
    while (e)
    {
        if (strcmp(e->key, key) == 0) return e;
        e = e->next;
    }
    return NULL;
}

/* Obtener valor; devuelve true si existe */
bool ht_get(HashTable *ht, const char *key, int *out_value)
{
    Entry *e = ht_find_entry(ht, key);
    if (!e) return false;
    if (out_value) *out_value = e->value;
    return true;
}

/* Insertar o actualizar (head insert en la lista del bucket) */
bool ht_put(HashTable *ht, const char *key, int value)
{
    if (!ht || !key) return false;
    Entry *existing = ht_find_entry(ht, key);
    if (existing)
    {
        existing->value = value; // actualiza
        return true;
    }
    unsigned long h = hash_djb2(key) % (unsigned long)ht->size;
    Entry *e = malloc(sizeof(Entry));
    if (!e) return false;
    e->key = xstrdup(key);
    e->value = value;
    e->next = ht->buckets[h];
    ht->buckets[h] = e;
    return true;
}

/* Eliminar clave; devuelve true si se borró */
bool ht_remove(HashTable *ht, const char *key)
{
    if (!ht || !key) return false;
    unsigned long h = hash_djb2(key) % (unsigned long)ht->size;
    Entry *e = ht->buckets[h];
    Entry *prev = NULL;
    while (e)
    {
        if (strcmp(e->key, key) == 0)
        {
            if (prev) prev->next = e->next;
            else ht->buckets[h] = e->next;
            free(e->key);
            free(e);
            return true;
        }
        prev = e;
        e = e->next;
    }
    return false;
}

/* Imprimir tabla (para depuración) */
void ht_print(HashTable *ht)
{
    if (!ht) return;
    for (int i = 0; i < ht->size; ++i)
    {
        printf("%d:", i);
        for (Entry *e = ht->buckets[i]; e; e = e->next)
        {
            printf(" -> (%s:%d)", e->key, e->value);
        }
        printf("\n");
    }
}

/* Liberar toda la memoria */
void ht_free(HashTable *ht)
{
    if (!ht) return;
    for (int i = 0; i < ht->size; ++i)
    {
        Entry *e = ht->buckets[i];
        while (e)
        {
            Entry *tmp = e;
            e = e->next;
            free(tmp->key);
            free(tmp);
        }
    }
    free(ht->buckets);
    free(ht);
}

/* Ejemplo de uso */
int main(void)
{
    HashTable *ht = ht_create(8); // 8 buckets
    ht_put(ht, "uno", 1);
    ht_put(ht, "dos", 2);
    ht_put(ht, "tres", 3);
    ht_put(ht, "dos", 22); // actualiza

    ht_print(ht);

    int v;
    if (ht_get(ht, "dos", &v)) printf("valor de 'dos' = %d\n", v);
    else printf("'dos' no encontrado\n");

    ht_remove(ht, "uno");
    ht_print(ht);

    ht_free(ht);
    return 0;
}
