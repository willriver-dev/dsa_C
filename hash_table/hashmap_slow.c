#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node **buckets;
    int size;
    int count;
} HashTable;

static Node *create_node(const char *key, int value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
        return NULL;
    new_node->key = strdup(key);
    if (!new_node->key) {
        free(new_node);
        return NULL;
    }
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

unsigned int hash(const char *key, int size) {
    unsigned long int hash_value = 5381;
    int c;
    while ((c = *key++)) {
        hash_value = ((hash_value << 5) + hash_value) + c;
    }
    return hash_value % size;
}

HashTable *create_hashmap(int size) {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (!ht)
        return NULL;
    ht->size = size;
    ht->count = 0;
    ht->buckets = (Node **)calloc(size, sizeof(Node *));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }
    return ht;
}
void ht_insert(HashTable *table, const char *key, int value) {
    if (table == NULL || key == NULL)
        return;
    unsigned int index = hash(key, table->size);
    Node *current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }
    Node *new_node = create_node(key, value);
    if (!new_node)
        return;
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->count++;
}
int ht_search(HashTable *table, const char *key) {
    if (table == NULL || key == NULL)
        return -1;
    unsigned int index = hash(key, table->size);
    Node *current = table->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return -1;
}
static void free_linked_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        if (temp->key)
            free(temp->key);
        free(temp);
    }
}
void ht_free(HashTable *table) {
    if (table == NULL)
        return;
    if (table->buckets != NULL) {
        for (int i = 0; i < table->size; i++) {
            free_linked_list(table->buckets[i]);
        }
        free(table->buckets);
    }
    free(table);
}
void generate_key(char *buffer, int index) { sprintf(buffer, "user_%d", index); }
void write_html_report(const char *filename, double time_spent, size_t size, size_t count) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Cannot open report file");
        return;
    }
    fprintf(f, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(f, "<style>\n");
    fprintf(f, "body { font-family: sans-serif; padding: 20px; }\n");
    fprintf(
        f,
        ".box { border: 1px solid #ccc; padding: 20px; border-radius: 8px; max-width: 400px; }\n");
    fprintf(f, "h2 { color: #333; }\n");
    fprintf(f, ".metric { display: flex; justify-content: space-between; border-bottom: 1px solid "
               "#eee; padding: 10px 0; }\n");
    fprintf(f, ".val { font-weight: bold; color: #007bff; }\n");
    fprintf(f, ".slow { color: red; } .fast { color: green; }\n");
    fprintf(f, "</style>\n</head>\n<body>\n");
    fprintf(f, "<div class='box'>\n");
    fprintf(f, "<h2>📊 Benchmark Report</h2>\n");
    const char *status_class = (time_spent > 1.0) ? "slow" : "fast";
    const char *status_text = (time_spent > 1.0) ? "BAD (Slow)" : "EXCELLENT (Fast)";
    fprintf(f, "<div class='metric'><span>Time:</span> <span class='val %s'>%.4fs</span></div>\n",
            status_class, time_spent);
    fprintf(f, "<div class='metric'><span>Buckets:</span> <span class='val'>%zu</span></div>\n",
            size);
    fprintf(f, "<div class='metric'><span>Elements:</span> <span class='val'>%zu</span></div>\n",
            count);
    fprintf(f,
            "<div class='metric'><span>Load Factor:</span> <span class='val'>%.2f</span></div>\n",
            (double)count / size);
    fprintf(f, "<div class='metric'><span>Status:</span> <span class='val %s'>%s</span></div>\n",
            status_class, status_text);
    fprintf(f, "</div>\n</body>\n</html>\n");
    fclose(f);
    printf(">> Report saved to %s\n", filename);
}
int main() {
    const int NUM_ELEMENTS = 100000;
    const int MAP_SIZE = 16;
    printf("=== BENCHMARK: FIXED SIZE HASH MAP (SLOW) ===\n");
    printf("Inserting %d elements into %d buckets...\n", NUM_ELEMENTS, MAP_SIZE);
    HashTable *map = create_hashmap(MAP_SIZE);
    if (!map)
        return 1;
    char key_buf[64];
    clock_t start = clock();
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        generate_key(key_buf, i);
        ht_insert(map, key_buf, i);
        if (i % 10000 == 0) {
            printf(".");
            fflush(stdout);
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    write_html_report("benchmark_result.html", time_spent, map->size, map->count);
    ht_free(map);
    return 0;
}
