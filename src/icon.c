#include <gtk/gtk.h>
#include "icon.h"

GList* load_icon_list(const char* icon_name) {
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

GdkPixbuf* load_icon(const char* icon_name, int size) {
    GtkIconTheme* theme;
    GtkIconLookupFlags flags = GTK_ICON_LOOKUP_FORCE_SIZE;

    theme = gtk_icon_theme_get_default();
    return gtk_icon_theme_load_icon(theme, icon_name, size, flags, NULL);
}

GdkPixbuf* load_icon_with_emblem(
    const char* icon_name, int icon_size,
    const char* emblem_name, int emblem_size) {

    GdkPixbuf* icon = load_icon(icon_name, icon_size);
    GdkPixbuf* emblem = load_icon(emblem_name, emblem_size);
    GdkPixbuf* composite = gdk_pixbuf_copy(icon);

    int offset = icon_size - emblem_size;

    gdk_pixbuf_composite(
        emblem,                     // src
        composite,                  // dst
        offset,                     // dst_x
        offset,                     // dst_y
        emblem_size,                // dst_width
        emblem_size,                // dst_height
        offset,                     // offset_x
        offset,                     // offset_y
        1.0,                        // scale_x
        1.0,                        // scale_y
        0,                          // interpolation mode (ignored?)
        255                         // alpha
    );

    g_object_unref(icon);
    g_object_unref(emblem);

    return composite;
}
