/* 
 * Copyright (C) 2002-2005, 2009 Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _RBGNOME_H_included
#define _RBGNOME_H_included

#define ENABLE_NLS 1

#include <sys/time.h>
#include <gnome.h>

#undef PATH_SEP
#undef _

#include "rbgtk.h"
#ifdef HAVE_RUBY_IO_H
#  include <ruby/io.h>
#else
#  include <rubyio.h>
#endif
#include <libgnome/libgnometypebuiltins.h>
#include "rbgnomeversion.h"

#define SET_STR_AND_GFREE(obj, str) do { \
    if ((str) != NULL) { \
        (obj) = rb_str_new2(str); \
        g_free(str); \
    } else { \
        (obj) = Qnil; \
    } \
} while(0)

struct timeval rb_time_timeval(VALUE);

VALUE rbgno_poptoption_raw_to_arary(struct poptOption *);
VALUE rbgno_poptoption_array_to_obj(VALUE);
void rbgno_poptoption_arg_to_hash(VALUE, struct poptOption *);


enum RBUIInfoType {
    RBUI_MENUS,
    RBUI_TOOLBAR,
    RBUI_MENU_HINTS,
    RBUI_POPUP_MENU,
};
VALUE rbgno_ary_to_ui_info(VALUE, enum RBUIInfoType);

void Init_gnome_program(VALUE);
void Init_gnome_about(VALUE);
void Init_gnome_app(VALUE);
void Init_gnome_appbar(VALUE);
void Init_gnome_app_helper(VALUE);
void Init_gnome_client(VALUE);
void Init_gnome_color_picker(VALUE);
void Init_gnome_config(VALUE);
void Init_gnome_dateedit(VALUE);
void Init_gnome_druid(VALUE);
void Init_gnome_druid_page(VALUE);
void Init_gnome_druid_page_edge(VALUE);
void Init_gnome_druid_page_standard(VALUE);
void Init_gnome_entry(VALUE);
void Init_gnome_file_entry(VALUE);
void Init_gnome_font_picker(VALUE);
void Init_gnome_help(VALUE);
void Init_gnome_href(VALUE);
void Init_gnome_i18n(VALUE);
void Init_gnome_icon_entry(VALUE);
void Init_gnome_icon_item(VALUE);
void Init_gnome_icon_list(VALUE);
void Init_gnome_icon_selection(VALUE);
void Init_gnome_pixmap_entry(VALUE);
void Init_gnome_popup_menu(VALUE);
void Init_gnome_score(VALUE);
void Init_gnome_scores(VALUE);
void Init_gnome_stock_icons(VALUE);
void Init_gnome_uidefs(VALUE);
void Init_gnome_url(VALUE);
void Init_gnome_util(VALUE);
void Init_gnome_popt(VALUE);
void Init_gnome_gconf(VALUE);
void Init_gnome_sound(VALUE);
void Init_gnome_trigger(VALUE);

#endif /* ! _RBGNOME_H_included */
