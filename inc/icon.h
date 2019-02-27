#ifndef ICON_H
#define ICON_H

GdkPixbuf* load_icon(const char*, int);
GList* load_icon_list(const char*);
GdkPixbuf* load_icon_with_emblem(const char*, int, const char*, int);

#endif
