/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome.c,v 1.19 2002/10/28 16:32:56 tkubo Exp $ */

/* GNOME module for Ruby/GNOME
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"
#ifndef EXCLUDE_RUBY_BONOBO
#include "rbbonobo.h"
#endif

void
Init_gnome2()
{
    VALUE mGnome = rb_define_module("Gnome");
#ifndef EXCLUDE_RUBY_BONOBO
    VALUE mBonobo = rb_define_module("Bonobo");

    Init_bonobo_dock(mBonobo);
    Init_bonobo_dock_layout(mBonobo);
    Init_bonobo_dock_item(mBonobo);
    Init_bonobo_dock_band(mBonobo);
    Init_bonobo_i18n(mBonobo);
#endif

    /* GnomePreferencesType */
    rb_define_const(mGnome, "PREFERENCES_NEVER", INT2FIX(GNOME_PREFERENCES_NEVER));
    rb_define_const(mGnome, "PREFERENCES_USER", INT2FIX(GNOME_PREFERENCES_USER));
    rb_define_const(mGnome, "PREFERENCES_ALWAYS", INT2FIX(GNOME_PREFERENCES_ALWAYS));

    Init_gnome_program(mGnome);
    Init_gnome_about(mGnome);
    Init_gnome_app(mGnome);
    Init_gnome_appbar(mGnome);
    Init_gnome_app_helper(mGnome);
    Init_gnome_client(mGnome);
    Init_gnome_color_picker(mGnome);
    Init_gnome_config(mGnome);
    Init_gnome_dateedit(mGnome);
    Init_gnome_druid(mGnome);
    Init_gnome_druid_page(mGnome);
    Init_gnome_druid_page_edge(mGnome);
    Init_gnome_druid_page_standard(mGnome);
    Init_gnome_entry(mGnome);
    Init_gnome_file_entry(mGnome);
    Init_gnome_font_picker(mGnome);
    Init_gnome_help(mGnome);
    Init_gnome_href(mGnome);
    Init_gnome_i18n(mGnome);
    Init_gnome_icon_entry(mGnome);
    Init_gnome_icon_item(mGnome);
    Init_gnome_icon_list(mGnome);
    Init_gnome_icon_selection(mGnome);
    Init_gnome_pixmap_entry(mGnome);
    Init_gnome_score(mGnome);
    Init_gnome_scores(mGnome);
    Init_gnome_stock_icons(mGnome);
    Init_gnome_uidefs(mGnome);
    Init_gnome_url(mGnome);
    Init_gnome_util(mGnome);
    Init_gnome_popt(mGnome);
}
