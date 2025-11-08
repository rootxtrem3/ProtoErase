#ifndef DESTRUCT_H
#define DESTRUCT_H

#include <time.h>
#include "security.h"  // Include security.h to get MAX_PATH

// Structure to hold configuration
typedef struct {
    char password[256];
    char target_dir[MAX_PATH];
    int max_attempts;
} Config;

// Function declarations
int remove_directory(const char *path);
int verify_directory_exists(const char *path);
int perform_self_destruct(const Config *config);

#endif