#ifndef CONFIG_H
#define CONFIG_H

#include "destruct.h"

// Configuration management functions
void init_default_config(Config *config);
int save_config_to_file(const Config *config, const char *filename);
int load_config_from_file(Config *config, const char *filename);
void display_config(const Config *config);
int update_config_interactive(Config *config);

#endif