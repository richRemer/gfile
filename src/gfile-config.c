#include "gfile-config.h"
#include "gfile-error.h"

static GKeyFile* gfile_config_load_key_file(const gchar*);
static const gchar* config_file = "gfile/gfile.config";

GFileConfig gfile_config_load() {
    GFileConfig config;
    GKeyFile* key_file = NULL;
    GList* files = NULL;
    const gchar* dir;

    allocate(config = calloc(1, sizeof(struct GFileConfig)));

    for (const gchar* const* ptr = g_get_system_config_dirs(); *ptr; ptr++) {
        g_print("loading %s\n", *ptr);
        key_file = gfile_config_load_key_file(*ptr);

        if (key_file) {
            g_print("apending %s\n", *ptr);
            files = g_list_append(files, key_file);
        } else {
            g_print("skipping %s\n", *ptr);
        }
    }

    dir = g_get_user_config_dir();
    g_print("loading %s\n", dir);
    key_file = gfile_config_load_key_file(dir);

    if (key_file) {
        g_print("appending %s\n", dir);
        files = g_list_append(files, key_file);
    }

    config->files = files;

    return config;

    catch();

    free(config);

    return NULL;
}

static GKeyFile* gfile_config_load_key_file(const gchar* dir) {
    GKeyFile* key_file = NULL;
    GError* error = NULL;
    gchar* path = NULL;
    
    //g_print("allocation for %s\n", dir);
    allocate(key_file = g_key_file_new());
    //g_print("allocation for %s/%s\n", dir, config_file);
    allocate(path = g_strconcat(dir, "/", config_file));
    g_print("allocated\n");

    if (!g_key_file_load_from_file(key_file, path, G_KEY_FILE_NONE, &error)) {
        g_key_file_free(key_file);
        key_file = NULL;

        if (g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
            g_print("NOENT\n");
            g_info("Info: %s", error->message);
            g_clear_error(&error);
        } else {
            g_critical("Error: %s", error->message);
            g_error_free(error);
        }
    }
    
    free(path);
    
    return key_file;

    catch();

    free(path);
    g_key_file_free(key_file);

    return NULL;
}

void gfile_config_destroy(GFileConfig config) {
    GList* item;

    for (GList* item = config->files; item; item = item->next) {
        g_key_file_free(item->data);
    }

    g_list_free(g_steal_pointer(&config->files));
    free(config);
}
