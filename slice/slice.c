#include <stdio.h>
#include <stdlib.h> // Thư viện chứa malloc, free, realloc

// Định nghĩa cấu trúc Vector (giống struct Slice header trong Go)
typedef struct {
    int *data;      // Con trỏ trỏ tới dữ liệu trên Heap (mảng int)
    size_t len;    // Số lượng phần tử hiện tại (len)
    size_t cap; // Sức chứa hiện tại (cap)
} IntVector;

// 1. Khởi tạo Vector
// Cấp phát capacity ban đầu (ví dụ 4 slot)
void vector_init(IntVector *vec) {
    vec->len = 0;
    vec->cap = 4; // Mặc định ban đầu chứa được 4 số
    // MALLOC: Xin hệ điều hành 4 ô đất (mỗi ô to bằng kích thước int)
    vec->data = (int *)malloc(vec->cap * sizeof(int));

    // Kiểm tra xem OS có cho đất không (đề phòng hết RAM)
    if (vec->data == NULL) {
        printf("Panic: Out of memory!\n");
        exit(1);
    }
}

// 2. Thêm phần tử vào cuối (Mô phỏng hàm append của Go)
// Đây là hàm KHÓ NHẤT - Boss Level
void vector_push_back(IntVector *vec, int value) {
    // Bước 1: Kiểm tra xem nhà còn chỗ không (size == capacity)?
    if (vec->len == vec->cap) {
        // HẾT CHỖ RỒI! PHẢI MỞ RỘNG NHÀ (RESIZE)

        // Logic: Nhân đôi sức chứa (Double capacity)
        // Tại sao nhân đôi? Để giảm số lần phải xin cấp phát lại.
        size_t new_capacity = vec->cap * 2;

        // TODO 1: Dùng hàm realloc() để thay đổi kích thước vùng nhớ vec->data.
        // Cú pháp: int *new_data = realloc(ptr_cũ, kích_thước_mới_tính_bằng_byte);
        // Nhớ cập nhật vec->data và vec->capacity sau khi realloc thành công.
        int *newData = realloc(vec->data, new_capacity * sizeof(int));
        printf("--> Resize: Tang capacity tu %zu len %zu\n", vec->cap, new_capacity);

        vec->cap = new_capacity;
        vec->data = newData;
    }

    // Bước 2: Nhét giá trị vào vị trí tiếp theo và tăng size
    // TODO 2: Gán value vào mảng data tại chỉ số vec->size, sau đó tăng vec->size lên.
    vec->data[vec->len] = value;

    vec->len++;
}

// 3. Lấy giá trị tại index (Mô phỏng vec[i])
int vector_get(IntVector *vec, size_t index) {
    if (index >= vec->len) {
        printf("Panic: Index out of bounds!\n");
        exit(1);
    }

    // TODO 3: Return giá trị tại vị trí index
    return vec->data[index];
}

// 4. Giải phóng bộ nhớ (Destructor)
// Go có Garbage Collector (GC) tự làm cái này. C thì KHÔNG.
// Bạn phải tự dọn rác.
void vector_free(IntVector *vec) {
    // TODO 4: Dùng hàm free() để trả lại vùng nhớ vec->data cho OS.
    vec->len = 0;
    vec->cap = 0;

    free(vec->data);
}

// --- TEST ZONE ---
int main() {
    IntVector v;
    vector_init(&v);

    printf("--- Start Pushing ---\n");
    // Thử push 10 phần tử để xem nó resize thế nào
    for (int i = 0; i < 10; i++) {
        vector_push_back(&v, i * 10);
        printf("Pushed %d (Size: %zu, Cap: %zu)\n", i * 10, v.len, v.cap);
    }

    printf("\n--- Reading Data ---\n");
    printf("Item at index 5: %d\n", vector_get(&v, 5));

    printf("\n--- Cleaning up ---\n");
    vector_free(&v);
    printf("Vector freed.\n");

    return 0;
}
