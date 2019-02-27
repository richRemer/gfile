#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include "gfile-app.h"
#include "gfile-error.h"

GFileApp create_gfile_app() {
    GFileApp app;
    char* cwd;

    allocate(app = malloc(sizeof(struct GFileApp)));
    allocate(cwd = getcwd(NULL, 0));

    if (!gfile_app_change_path(app, cwd)) {
        raise();
    }

    return app;

    catch();

    free(app);
    free(cwd);

    return NULL;
}

bool gfile_app_change_path(GFileApp app, const char* path) {
    char* new_path;
    char* new_base;

    if (path == NULL) {
        return false;
    }

    allocate(new_path = strdup(path));
    allocate(new_base = strdup(basename(new_path)));

    free((char*)app->path);
    free((char*)app->basename);

    app->path = new_path;
    app->basename = new_base;

    return true;

    catch();

    free(new_path);
    free(new_base);

    return false;
}

const char* gfile_app_get_path(GFileApp app) {
    return app->path;
}

const char* gfile_app_get_basename(GFileApp app) {
    return app->basename;
}

void destroy_gfile_app(GFileApp app) {
    free((char*)app->path);
    free((char*)app->basename);
    free(app);
}
