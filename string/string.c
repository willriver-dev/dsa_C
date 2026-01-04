#include <stdio.h>






size_t my_strlen(const char *str) {
    size_t count = 0;
    
    
    
    while (*str != '\0') {
        count++;
        str++;
    }
    return count;
}






char *my_strcpy(char *dest, const char *src) {
    
    char *original_dest = dest;

    
    
    
    

    
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
     *dest = '\0';
     return original_dest;
}






char *my_strcat(char *dest, const char *src) {
    char *original_dest = dest;

    
    
    

    

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


void run_test() {
    printf("=== BAT DAU TEST ===\n");

    
    if (my_strlen("Hello") == 5 && my_strlen("") == 0)
        printf("[PASS] my_strlen\n");
    else
        printf("[FAIL] my_strlen\n");

    
    char buffer[100]; 
    my_strcpy(buffer, "Backend");
    
    if (my_strlen(buffer) == 7 && buffer[0] == 'B' && buffer[7] == '\0')
        printf("[PASS] my_strcpy\n");
    else
        printf("[FAIL] my_strcpy: Ket qua la '%s'\n", buffer);

    
    my_strcpy(buffer, "Hello"); 
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
