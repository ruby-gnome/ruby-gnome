#ifndef _RBGNOME_H_included
#define _RBGNOME_H_included

#define ENABLE_NLS 1

#include <sys/time.h>
#include <gnome.h>

#undef PATH_SEP
#undef _

#include "rbgtk.h"
#include <rubyio.h>

#define SET_STR_AND_GFREE(obj, str) do { \
    if ((str) != NULL) { \
        (obj) = rb_str_new2(str); \
        g_free(str); \
    } else { \
        (obj) = Qnil; \
    } \
} while(0)

struct timeval rb_time_timeval(VALUE);

void Init_bonobo_dock(VALUE);
void Init_bonobo_dock_layout(VALUE);
void Init_bonobo_dock_item(VALUE);
void Init_bonobo_dock_band(VALUE);

void Init_gnome_about(VALUE);
void Init_gnome_app_helper(VALUE, VALUE);
void Init_gnome_app(VALUE);
void Init_gnome_appbar(VALUE);
void Init_gnome_client(VALUE);
void Init_gnome_color_picker(VALUE);
void Init_gnome_config(VALUE);
void Init_gnome_dateedit(VALUE);
void Init_gnome_entry(VALUE);
void Init_gnome_file_entry(VALUE);
void Init_gnome_font_picker(VALUE);
void Init_gnome_href(VALUE);
void Init_gnome_i18n(VALUE);
void Init_gnome_icon_entry(VALUE);
void Init_gnome_icon_item(VALUE);
void Init_gnome_icon_list(VALUE);
void Init_gnome_icon_selection(VALUE);
void Init_gnome_pixmap_entry(VALUE);
void Init_gnome_program(VALUE);
void Init_gnome_scores(VALUE);
void Init_gnome_uidefs(VALUE);
void Init_gnome_url(VALUE);
void Init_gnome_util(VALUE);

#endif /* ! _RBGNOME_H_included */
