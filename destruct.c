#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "destruct.h"
#include "security.h"

// Recursively remove directory and all contents
int remove_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return -1;
    }

    struct dirent *entry;
    char full_path[MAX_PATH];
    int result = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Build full path
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        // Get file info
        struct stat statbuf;
        if (stat(full_path, &statbuf) == -1) {  // Changed from lstat to stat
            continue;
        }
        
        if (S_ISDIR(statbuf.st_mode)) {
            // Recursively remove subdirectory
            if (remove_directory(full_path) == -1) {
                result = -1;
            }
        } else {
            // Remove file
            if (unlink(full_path) == -1) {
                result = -1;
            }
        }
    }
    
    closedir(dir);
    
    // Remove the now-empty directory
    if (rmdir(path) == -1) {
        result = -1;
    }
    
    return result;
}

// Verify target directory exists and is accessible
int verify_directory_exists(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        return 0; // Doesn't exist or not accessible
    }
    return S_ISDIR(st.st_mode); // Returns 1 if it's a directory
}

// Perform the self-destruct sequence
int perform_self_destruct(const Config *config) {
    printf("Initiating self-destruct sequence...\n");
    
    // Countdown
    for (int i = 5; i > 0; i--) {
        printf("%d... ", i);
        fflush(stdout);
        sleep(1);
    }
    printf("DESTROYING!\n");
    
    // Remove directory
    if (remove_directory(config->target_dir) == 0) {
        printf("✓ Directory successfully destroyed: %s\n", config->target_dir);
        
        // Create log file
        FILE *log = fopen("/tmp/self_destruct.log", "a");
        if (log) {
            time_t now = time(NULL);
            fprintf(log, "Self-destruct completed at %ld for: %s\n", 
                    (long)now, config->target_dir);
            fclose(log);
        }
        return 1; // Success
    } else {
        printf("✗ Error: Failed to destroy directory: %s\n", config->target_dir);
        return 0; // Failure
    }
}