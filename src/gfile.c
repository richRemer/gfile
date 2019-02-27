#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "gfile-app.h"

extern unsigned char gfile_css[];
extern unsigned int gfile_css_len;

static void init_styles() {
    GtkCssProvider* styles;
    GdkScreen* screen;
    GdkDisplay* display;

    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    styles = gtk_css_provider_new();

    gtk_css_provider_load_from_data(styles, gfile_css, gfile_css_len, NULL);
    gtk_style_context_add_provider_for_screen(
        screen,
        GTK_STYLE_PROVIDER(styles),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(styles);
}

static GList* load_icon_list(const char* icon_name) {
  GList* icon_list = NULL;
  GdkPixbuf* icon;
  GtkIconTheme* theme;
  gint* sizes;

  theme = gtk_icon_theme_get_default();
  sizes = gtk_icon_theme_get_icon_sizes(theme, icon_name);

  for (gint* size = sizes; *size; ++size) {
    icon = gtk_icon_theme_load_icon(theme, icon_name, *size, 0, NULL);
    icon_list = g_list_prepend(icon_list, icon);
  }

  g_free(sizes);

  return icon_list;
}

static GdkPixbuf* load_icon(const char* icon_name, int size) {
    GtkIconTheme* theme;
    theme = gtk_icon_theme_get_default();
    return gtk_icon_theme_load_icon(theme, icon_name, size, 0, NULL);
}

static void add_style_class(GtkWidget* widget, const char* style_class) {
    GtkStyleContext* styles;
    styles = gtk_widget_get_style_context(widget);
    gtk_style_context_add_class(styles, style_class);
}

int main(int argc, char* argv[]) {
    GFileApp app = create_gfile_app();
    GtkWidget* window;
    GtkWidget* window_box;
    GtkWidget* header;
    GtkWidget* header_box;
    GtkWidget* path_left_button;
    GtkWidget* path_root_button;
    GtkWidget* path_home_button;
    GtkWidget* path_right_button;
    GtkWidget* path_other_buttons[1] = {NULL};
    GtkWidget* file_view;
    GtkListStore* files;
    GtkTreeIter item;
    GList* icon_list;
    GdkPixbuf* up_icon;
    GdkPixbuf* folder_icon;
    GdkPixbuf* text_icon;

    gtk_init(&argc, &argv);
    init_styles();

    icon_list = load_icon_list("system-file-manager");

    path_left_button = gtk_button_new_with_label("◀");
    path_root_button = gtk_button_new_from_icon_name("drive-harddisk", GTK_ICON_SIZE_BUTTON);
    path_other_buttons[0] = gtk_button_new_with_label("home");
    path_home_button = gtk_button_new_from_icon_name("go-home", GTK_ICON_SIZE_BUTTON);
    path_right_button = gtk_button_new_with_label("▶");

    gtk_widget_set_sensitive(path_left_button, FALSE);
    gtk_widget_set_sensitive(path_right_button, FALSE);
    gtk_widget_set_visible(path_root_button, FALSE);
    gtk_widget_set_visible(path_other_buttons[0], FALSE);

    header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    add_style_class(header_box, GTK_STYLE_CLASS_LINKED);
    gtk_box_pack_start(GTK_BOX(header_box), path_left_button, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(header_box), path_root_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(header_box), path_other_buttons[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(header_box), path_home_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(header_box), path_right_button, FALSE, TRUE, 0);

    header = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header), gfile_app_get_basename(app));
    gtk_header_bar_set_has_subtitle(GTK_HEADER_BAR(header), FALSE);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), header_box);

    up_icon = load_icon("go-up", 48);
    folder_icon = load_icon("folder", 48);
    text_icon = load_icon("text-x-generic", 48);

    files = gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING, GDK_TYPE_PIXBUF);
    gtk_list_store_append(files, &item);
    gtk_list_store_set(files, &item, 0, 0, 1, "", 2, up_icon, -1);
    gtk_list_store_append(files, &item);
    gtk_list_store_set(files, &item, 0, 1, 1, "Documents", 2, folder_icon, -1);
    gtk_list_store_append(files, &item);
    gtk_list_store_set(files, &item, 0, 2, 1, "foo.txt", 2, text_icon, -1);

    file_view = gtk_icon_view_new_with_model(GTK_TREE_MODEL(files));
    gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(file_view), GTK_SELECTION_MULTIPLE);
    gtk_icon_view_set_item_width(GTK_ICON_VIEW(file_view), 64);
    gtk_icon_view_set_text_column(GTK_ICON_VIEW(file_view), 1);
    gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(file_view), 2);

    window_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(window_box), file_view, TRUE, TRUE, 0);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Files");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
    gtk_window_set_icon_list(GTK_WINDOW(window), icon_list);
    gtk_window_set_titlebar(GTK_WINDOW(window), header);
    gtk_container_add(GTK_CONTAINER(window), window_box);
    g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);

    gtk_widget_show_all(window);
    gtk_main();
    destroy_gfile_app(app);

    return 0;
}
