#ifndef GFILE_CONFIG_H
#define GFILE_CONFIG_H

#include <glib-2.0/glib.h>

typedef struct GFileConfig {
    GList* files;
} *GFileConfig;

GFileConfig gfile_config_load();
void gfile_config_destroy(GFileConfig);

#endif
