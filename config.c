#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "security.h"

// Initialize with default configuration
void init_default_config(Config *config) {
    // Use snprintf instead of strncpy for safer string copying
    snprintf(config->password, sizeof(config->password), "%s", "selfdestruct123");
    snprintf(config->target_dir, sizeof(config->target_dir), "%s", "/tmp/test_destruct");
    config->max_attempts = 3;
}

// Save configuration to file
int save_config_to_file(const Config *config, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        return 0;
    }
    
    fprintf(file, "%s\n", config->password);
    fprintf(file, "%s\n", config->target_dir);
    fprintf(file, "%d\n", config->max_attempts);
    
    fclose(file);
    return 1;
}

// Load configuration from file
int load_config_from_file(Config *config, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return 0;
    }
    
    char line[512];
    
    // Read password
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }
    line[strcspn(line, "\n")] = 0;
    
    // Safer string copying with length validation
    if (strlen(line) >= sizeof(config->password)) {
        fclose(file);
        return 0; // Password too long
    }
    strcpy(config->password, line); // Safe because we checked length
    
    // Read target directory
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }
    line[strcspn(line, "\n")] = 0;
    
    // Safer string copying with length validation
    if (strlen(line) >= sizeof(config->target_dir)) {
        fclose(file);
        return 0; // Path too long
    }
    strcpy(config->target_dir, line); // Safe because we checked length
    
    // Read max attempts
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }
    config->max_attempts = atoi(line);
    
    // Validate max_attempts
    if (config->max_attempts < 1 || config->max_attempts > 10) {
        config->max_attempts = 3; // Reset to default if invalid
    }
    
    fclose(file);
    return 1;
}

// Display current configuration
void display_config(const Config *config) {
    printf("\n=== Current Configuration ===\n");
    printf("Target Directory: %s\n", config->target_dir);
    printf("Max Attempts: %d\n", config->max_attempts);
    printf("Password: ********\n");
}

// Interactive configuration update
int update_config_interactive(Config *config) {
    char new_password[256];
    char new_directory[MAX_PATH];
    
    printf("\n=== Update Configuration ===\n");
    
    // Get new password
    printf("Enter new password: ");
    if (scanf("%255s", new_password) != 1) {
        printf("Error reading password.\n");
        return 0;
    }
    
    // Validate password
    if (!validate_password(new_password)) {
        return 0;
    }
    
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // Get new directory
    printf("Enter target directory: ");
    if (fgets(new_directory, sizeof(new_directory), stdin) == NULL) {
        printf("Error reading directory.\n");
        return 0;
    }
    new_directory[strcspn(new_directory, "\n")] = 0;
    
    // Validate directory
    if (!validate_directory(new_directory)) {
        return 0;
    }
    
    // Update configuration with safe string copying
    strcpy(config->password, new_password); // Safe because new_password is limited to 255 chars
    strcpy(config->target_dir, new_directory); // Safe because validate_directory checks length
    
    // Clear sensitive data
    secure_clear(new_password, sizeof(new_password));
    
    printf("✓ Configuration updated successfully!\n");
    return 1;
}