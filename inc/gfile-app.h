#ifndef GFILE_APP_H
#define GFILE_APP_H

#include <stdbool.h>

typedef struct GFileApp {
    const char* path;
    const char* basename;
} *GFileApp;

GFileApp gfile_app_new();
bool gfile_app_change_path(GFileApp, const char*);
const char* gfile_app_get_path(GFileApp);
const char* gfile_app_get_basename(GFileApp);
void gfile_app_destroy(GFileApp);

#endif
