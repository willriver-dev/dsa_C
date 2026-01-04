#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR_THRESHOLD 0.75
#define HASH_SEED 5381

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node **buckets;
    size_t size;
    size_t count;
} HashTable;

static size_t hash_djb2(const char *key, size_t capacity);
static bool ht_resize(HashTable *table);
static Node *create_node(const char *key, int value);
static void free_bucket(Node *head);
void ht_destroy(HashTable *table);

HashTable *ht_create(size_t capacity) {
    if (capacity == 0)
        capacity = INITIAL_CAPACITY;
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (!ht)
        return NULL;
    ht->size = capacity;
    ht->count = 0;
    ht->buckets = (Node **)calloc(capacity, sizeof(Node *));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }
    return ht;
}

bool ht_insert(HashTable *table, const char *key, int value) {
    if (!table || !key)
        return false;

    if ((double)table->count / (double)table->size >= LOAD_FACTOR_THRESHOLD) {
        ht_resize(table);
    }

    size_t index = hash_djb2(key, table->size);
    Node *current = table->buckets[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return true;
        }
        current = current->next;
    }

    Node *new_node = create_node(key, value);
    if (!new_node)
        return false;

    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->count++;

    return true;
}

bool ht_search(const HashTable *table, const char *key, int *out_value) {
    if (!table || !key)
        return false;
    size_t index = hash_djb2(key, table->size);
    Node *current = table->buckets[index];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (out_value)
                *out_value = current->value;
            return true;
        }
        current = current->next;
    }
    return false;
}

void ht_destroy(HashTable *table) {
    if (!table)
        return;
    if (table->buckets) {
        for (size_t i = 0; i < table->size; i++) {
            free_bucket(table->buckets[i]);
        }
        free(table->buckets);
    }
    free(table);
}

static size_t hash_djb2(const char *key, size_t capacity) {
    size_t hash = HASH_SEED;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

static Node *create_node(const char *key, int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node)
        return NULL;
    node->key = strdup(key);
    if (!node->key) {
        free(node);
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

static void free_bucket(Node *head) {
    while (head) {
        Node *temp = head;
        head = head->next;
        free(temp->key);
        free(temp);
    }
}

static bool ht_resize(HashTable *table) {
    size_t new_size = table->size * 2;
    Node **new_buckets = (Node **)calloc(new_size, sizeof(Node *));
    if (!new_buckets)
        return false;

    for (size_t i = 0; i < table->size; i++) {
        Node *current = table->buckets[i];
        while (current) {
            Node *next = current->next;
            size_t new_index = hash_djb2(current->key, new_size);
            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;
            current = next;
        }
    }
    free(table->buckets);
    table->buckets = new_buckets;
    table->size = new_size;
    return true;
}

void generate_key(char *buffer, int index) { sprintf(buffer, "user_%d", index); }

int main() {
    const int NUM_ELEMENTS = 100000;

    HashTable *map = ht_create(16);
    if (!map)
        return 1;

    char key_buf[64];
    clock_t start = clock();

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        generate_key(key_buf, i);
        ht_insert(map, key_buf, i);
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("[DATA] Time: %.6f\n", time_spent);
    printf("[DATA] Count: %zu\n", map->count);
    printf("[DATA] Size: %zu\n", map->size);

    ht_destroy(map);
    return 0;
}
