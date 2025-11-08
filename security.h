#ifndef SECURITY_H
#define SECURITY_H

#include <stdlib.h>

#define MAX_PATH 1024  // Moved from destruct.h to security.h

// Security function declarations
int secure_compare(const char *a, const char *b);
void secure_clear(void *data, size_t len);
int validate_password(const char *password);
int validate_directory(const char *path);

#endif