#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- DEFINITIONS ---

typedef struct Node {
    void *data; // Generic data
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct DLL {
    Node *head;
    Node *tail;
    int size;
} DLL;

// Định nghĩa kiểu hàm callback cho gọn
typedef void (*PrintFn)(void *);
typedef void (*FreeFn)(void *);

// --- IMPLEMENTATION ---

Node *create_node(void *value) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("Unable to allocate new_node");
        exit(EXIT_FAILURE);
    }

    new_node->data = value;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

// TODO: Tạo một List rỗng
DLL *create_dll() {
    DLL *list = malloc(sizeof(DLL));
    if (list == NULL) {
        perror("Unable to allocate DLL");
        exit(EXIT_FAILURE);
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void append(DLL *list, void *data) {
    if (list == NULL) {
        fprintf(stderr, "Error: List pointer cannot be NULL.\n");
        return; // Trả về 0 (false) thay vì exit
    }

    // Check 2: Valid data
    if (data == NULL) {
        fprintf(stderr, "Error: Data cannot be NULL.\n");
        return;
    }

    Node *new_node = create_node(data);
    Node *current_tail = list->tail;
    if (current_tail == NULL) {
        list->tail = new_node;
        list->head = new_node;
        list->size = 1;

        return;
    }

    current_tail->next = new_node;
    new_node->prev = current_tail;
    list->tail = new_node;
    list->size++;

    return;
}

void print_list(DLL *list, PrintFn print_fn) {
    if (list == NULL) {
        fprintf(stderr, "Error: List pointer cannot be NULL\n");

        return;
    }

    Node *current_head = list->head;
    if (current_head == NULL) {
        printf("List is empty");

        return;
    }

    while (current_head != NULL) {
        print_fn(current_head->data);
        current_head = current_head->next;
    }

    return;
}

void destroy_list(DLL *list, FreeFn free_fn) {
    if (list == NULL) {
        fprintf(stderr, "Error: List pointer cannot be NULL\n");

        return;
    }

    if (list->head == NULL) {

        return;
    }

    Node *current = list->head;
    while (current != NULL) {
        Node *next_node = current->next;

        if (free_fn != NULL) {
            free_fn(current->data);
        }

        free(current);
        current = next_node;
    }

    free(list);

    return;
}

// --- USER DEFINED FUNCTIONS (Callback) ---
// Giả sử ta lưu kiểu "Person" trong list

typedef struct Person {
    char name[32];
    int age;
} Person;

// Helper tạo Person
Person *create_person(const char *name, int age) {
    Person *p = malloc(sizeof(Person));
    strcpy(p->name, name);
    p->age = age;
    return p;
}

// Hàm này sẽ được truyền vào print_list
void print_person(void *data) {
    Person *p = (Person *)data; // Ép kiểu từ void* về Person*
    printf("[Name: %s, Age: %d] <-> ", p->name, p->age);
}

// Hàm này sẽ được truyền vào destroy_list
void free_person(void *data) {
    free(data); // Free vùng nhớ của struct Person
}

// --- MAIN ---

int main() {
    printf("=== Generic Doubly Linked List ===\n");

    DLL *my_list = create_dll();

    // Thêm dữ liệu (Heap)
    append(my_list, create_person("Alice", 25));
    append(my_list, create_person("Bob", 30));
    append(my_list, create_person("Charlie", 22));

    printf("List Size: %d\n", my_list->size);
    printf("Content: ");

    // Truyền hàm print_person vào đây (Callback)
    print_list(my_list, print_person);
    printf("NULL\n");

    printf("\n=== Destroying List ===\n");
    // Truyền hàm free_person vào để dọn dẹp data
    destroy_list(my_list, free_person);

    printf("Clean exit.\n");
    return 0;
}
