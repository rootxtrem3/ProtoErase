#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "security.h"

// Constant-time password comparison to prevent timing attacks
int secure_compare(const char *a, const char *b) {
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    
    // Immediate failure if lengths don't match
    if (len_a != len_b) {
        return 0;
    }
    
    // Constant-time comparison
    int result = 0;
    for (size_t i = 0; i < len_a; i++) {
        result |= a[i] ^ b[i];
    }
    
    return result == 0;
}

// Securely clear sensitive data from memory
void secure_clear(void *data, size_t len) {
    if (data == NULL) return;
    
    volatile unsigned char *p = (volatile unsigned char *)data;
    while (len--) {
        *p++ = 0;
    }
}

// Validate password meets minimum requirements
int validate_password(const char *password) {
    size_t len = strlen(password);
    
    if (len < 4) {
        printf("Error: Password must be at least 4 characters\n");
        return 0;
    }
    
    if (len > 255) {
        printf("Error: Password too long\n");
        return 0;
    }
    
    return 1;
}

// Validate directory path is reasonable
int validate_directory(const char *path) {
    if (path == NULL || strlen(path) == 0) {
        printf("Error: Directory path cannot be empty\n");
        return 0;
    }
    
    if (strlen(path) >= MAX_PATH) {
        printf("Error: Directory path too long\n");
        return 0;
    }
    
    // Prevent deletion of critical system directories
    const char *protected_paths[] = {
        "/", "/bin", "/etc", "/lib", "/root", "/sys", "/proc", 
        "/usr", "/var", "/boot", "/sbin", "/dev", NULL
    };
    
    for (int i = 0; protected_paths[i] != NULL; i++) {
        if (strcmp(path, protected_paths[i]) == 0) {
            printf("Error: Cannot target protected system directory: %s\n", path);
            return 0;
        }
    }
    
    return 1;
}