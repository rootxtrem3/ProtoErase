#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "destruct.h"
#include "security.h"
#include "config.h"

#define CONFIG_FILE "self_destruct.conf"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void self_destruct_sequence(Config *config) {
    char input_password[256];
    int attempts = config->max_attempts;
    
    printf("\n=== Self-Destruct System ===\n");
    printf("Target: %s\n", config->target_dir);
    printf("WARNING: This action is IRREVERSIBLE!\n\n");
    
    // Verify target exists
    if (!verify_directory_exists(config->target_dir)) {
        printf("❌ Error: Target directory does not exist or is not accessible: %s\n", 
               config->target_dir);
        printf("Please update the configuration with a valid directory.\n");
        return;
    }
    
    // Password verification loop
    while (attempts > 0) {
        printf("Enter password (%d attempts remaining): ", attempts);
        
        // Disable echo for password input
        system("stty -echo");
        if (scanf("%255s", input_password) != 1) {
            system("stty echo");
            printf("\nError reading input.\n");
            return;
        }
        system("stty echo");
        printf("\n");
        
        // Verify password
        if (secure_compare(input_password, config->password)) {
            printf("✓ Password accepted!\n");
            
            // Perform destruction
            if (perform_self_destruct(config)) {
                // Save config after successful destruction
                save_config_to_file(config, CONFIG_FILE);
            }
            
            secure_clear(input_password, sizeof(input_password));
            return;
        } else {
            printf("✗ Invalid password.\n");
            attempts--;
        }
        
        secure_clear(input_password, sizeof(input_password));
    }
    
    printf("Too many failed attempts. System locked.\n");
}

int main() {
    Config config;
    int choice;
    
    printf("=== Secure Directory Self-Destruct System ===\n");
    printf("Version: 2.0 | Multi-file Architecture\n\n");
    
    // Load or initialize configuration
    if (!load_config_from_file(&config, CONFIG_FILE)) {
        printf("No saved configuration found. Using defaults.\n");
        init_default_config(&config);
        save_config_to_file(&config, CONFIG_FILE);
    }
    
    // Main menu loop
    while (1) {
        printf("\n=== Main Menu ===\n");
        printf("Current Target: %s\n", config.target_dir);
        printf("1. Self-Destruct Sequence\n");
        printf("2. Configuration\n");
        printf("3. Display Current Settings\n");
        printf("4. Test Directory Existence\n");
        printf("5. Exit\n");
        printf("Choose option: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        
        switch (choice) {
            case 1:
                self_destruct_sequence(&config);
                break;
                
            case 2:
                if (update_config_interactive(&config)) {
                    save_config_to_file(&config, CONFIG_FILE);
                }
                break;
                
            case 3:
                display_config(&config);
                break;
                
            case 4:
                if (verify_directory_exists(config.target_dir)) {
                    printf("✓ Target directory exists: %s\n", config.target_dir);
                } else {
                    printf("✗ Target directory does not exist: %s\n", config.target_dir);
                }
                break;
                
            case 5:
                printf("Goodbye! Configuration saved.\n");
                save_config_to_file(&config, CONFIG_FILE);
                return 0;
                
            default:
                printf("Invalid option. Please choose 1-5.\n");
        }
    }
    
    return 0;
}