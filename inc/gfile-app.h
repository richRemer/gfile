#ifndef GFILE_APP_H
#define GFILE_APP_H

#include <stdbool.h>

typedef struct GFileApp {
    const char* path;
    const char* basename;
} *GFileApp;

GFileApp create_gfile_app();
bool gfile_app_change_path(GFileApp, const char*);
const char* gfile_app_get_path(GFileApp);
const char* gfile_app_get_basename(GFileApp);
void destroy_gfile_app(GFileApp);

#endif
