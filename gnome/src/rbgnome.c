/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome.c,v 1.23 2003/11/08 18:49:45 mutoh Exp $ */

/* GNOME module for Ruby/GNOME
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2001      Neil Conway <neilconway@rogers.com>
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

#include "rbgnome.h"

void
Init_gnome2()
{
    VALUE mGnome = rb_define_module("Gnome");

    /* GnomePreferencesType */
    G_DEF_CLASS(GNOME_TYPE_PREFERENCES_TYPE, "PreferencesType", mGnome);
    G_DEF_CONSTANTS(mGnome, GNOME_TYPE_PREFERENCES_TYPE, "GNOME_");

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
    Init_gnome_popup_menu(mGnome);
    Init_gnome_score(mGnome);
    Init_gnome_scores(mGnome);
    Init_gnome_stock_icons(mGnome);
    Init_gnome_uidefs(mGnome);
    Init_gnome_url(mGnome);
    Init_gnome_util(mGnome);
    Init_gnome_popt(mGnome);
}
