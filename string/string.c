#include <stdio.h>

// ==========================================
// BÀI 1: TÍNH ĐỘ DÀI CHUỖI
// ==========================================
// Input: "Hello"
// Output: 5
size_t my_strlen(const char *str) {
    size_t count = 0;
    // Gợi ý:
    // 1. Kiểm tra xem *str có phải là '\0' không.
    // 2. Nếu không, tăng count lên 1, và tăng str lên 1 (str++).
    while (*str != '\0') {
        count++;
        str++;
    }
    return count;
}

// ==========================================
// BÀI 2: COPY CHUỖI (QUAN TRỌNG NHẤT)
// ==========================================
// Input: dest (rỗng), src ("Code")
// Output: dest chứa "Code"
char *my_strcpy(char *dest, const char *src) {
    // Chúng ta cần lưu lại địa chỉ ban đầu của dest để return
    char *original_dest = dest;

    // Gợi ý:
    // 1. Dùng vòng lặp while để chạy khi *src chưa phải là '\0'
    // 2. Trong lặp: Gán *dest = *src. Sau đó tăng cả dest++ và src++
    // 3. Hết lặp: Nhớ gán ký tự '\0' vào *dest hiện tại.

    // TODO: Viết code của bạn ở đây
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
     *dest = '\0';
     return original_dest;
}

// ==========================================
// BÀI 3: NỐI CHUỖI
// ==========================================
// Input: dest ("Hello"), src (" World")
// Output: dest thành "Hello World"
char *my_strcat(char *dest, const char *src) {
    char *original_dest = dest;

    // Gợi ý:
    // Bước 1: Phải đưa ngón tay dest chạy đến cuối chuỗi hiện tại (tìm ký tự '\0' của dest).
    // Bước 2: Khi dest đã đứng ở '\0', bắt đầu copy src đè lên vị trí đó (giống hệt logic của strcpy).

    // TODO: Viết code của bạn ở đây

    while (*dest != '\0') {
        dest++;
    }

    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }

    return original_dest;
}

// --- KHU VỰC KIỂM TRA (KHÔNG SỬA) ---
void run_test() {
    printf("=== BAT DAU TEST ===\n");

    // 1. Test strlen
    if (my_strlen("Hello") == 5 && my_strlen("") == 0)
        printf("[PASS] my_strlen\n");
    else
        printf("[FAIL] my_strlen\n");

    // 2. Test strcpy
    char buffer[100]; // Cấp phát 1 vùng nhớ 100 byte
    my_strcpy(buffer, "Backend");
    // Kiểm tra thủ công: độ dài phải là 7, và ký tự đầu là 'B'
    if (my_strlen(buffer) == 7 && buffer[0] == 'B' && buffer[7] == '\0')
        printf("[PASS] my_strcpy\n");
    else
        printf("[FAIL] my_strcpy: Ket qua la '%s'\n", buffer);

    // 3. Test strcat
    my_strcpy(buffer, "Hello"); // Reset buffer
    my_strcat(buffer, " World");
    if (my_strlen(buffer) == 11 && buffer[10] == 'd')
        printf("[PASS] my_strcat: Ket qua la '%s'\n", buffer);
    else
        printf("[FAIL] my_strcat: Ket qua la '%s'\n", buffer);

    printf("=== KET THUC TEST ===\n");
}

int main() {
    run_test();
    return 0;
}
