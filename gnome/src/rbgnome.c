/* $Id: rbgnome.c,v 1.1 2002/05/19 12:39:24 mutoh Exp $ */

/* Gnome module for Ruby/Gnome
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

static VALUE
get_gnome_type(gnobj)
     GtkObject* gnobj;
{
    VALUE klass;

    if (0);
    else if GTK_IS_CLOCK(gnobj) klass = gClock;
    else if GTK_IS_DIAL(gnobj) klass = gDial;
    else if GTK_IS_PIXMAP_MENU_ITEM(gnobj) klass = gPixmapMenuItem;
    else if GNOME_IS_ABOUT(gnobj) klass = gnoAbout;
    else if GNOME_IS_ANIMATOR(gnobj) klass = gnoAnimator;
    else if GNOME_IS_APP(gnobj) klass = gnoApp;
    else if GNOME_IS_APPBAR(gnobj) klass = gnoAppBar;
    else if GNOME_IS_CALCULATOR(gnobj) klass = gnoCalculator;
    else if GNOME_IS_CANVAS(gnobj) klass = gnoCanvas;
    else if GNOME_IS_CANVAS_ITEM(gnobj) klass = gnoCanvasItem;
    else if GNOME_IS_CANVAS_GROUP(gnobj) klass = gnoCanvasGroup;
    else if GNOME_IS_CLIENT(gnobj) klass = gnoClient;
    else if GNOME_IS_COLOR_PICKER(gnobj) klass = gnoColorPicker;
    else if GNOME_IS_DATE_EDIT(gnobj) klass = gnoDateEdit;
    else if GNOME_IS_DENTRY_EDIT(gnobj) klass = gnoDEntryEdit;
    else if GNOME_IS_DIALOG(gnobj) klass = gnoDialog;
    else if GNOME_IS_DOCK(gnobj) klass = gnoDock;
    else if GNOME_IS_DOCK_BAND(gnobj) klass = gnoDockBand;
    else if GNOME_IS_DOCK_ITEM(gnobj) klass = gnoDockItem;
    else if GNOME_IS_DOCK_LAYOUT(gnobj) klass = gnoDockLayout;
    else if GNOME_IS_ENTRY(gnobj) klass = gnoEntry;
    else if GNOME_IS_FILE_ENTRY(gnobj) klass = gnoFileEntry;
    else if GNOME_IS_FONT_PICKER(gnobj) klass = gnoFontPicker;
    else if GNOME_IS_HREF(gnobj) klass = gnoHRef;
    else if GNOME_IS_ICON_ENTRY(gnobj) klass = gnoIconEntry;
    else if GNOME_IS_ICON_LIST(gnobj) klass = gnoIconList;
    else if GNOME_IS_ICON_SELECTION(gnobj) klass = gnoIconEntry;
    else if GNOME_IS_LESS(gnobj) klass = gnoLess;
    else if GNOME_IS_MESSAGE_BOX(gnobj) klass = gnoMessageBox;
    else if GNOME_IS_NUMBER_ENTRY(gnobj) klass = gnoNumberEntry;
    else if GNOME_IS_PAPER_SELECTOR(gnobj) klass = gnoPaperSelector;
    else if GNOME_IS_PIXMAP(gnobj) klass = gnoPixmap;
    else if GNOME_IS_PIXMAP_ENTRY(gnobj) klass = gnoPixmapEntry;
    else if GNOME_IS_PROPERTY_BOX(gnobj) klass = gnoPropertyBox;
    else if GNOME_IS_PROC_BAR(gnobj) klass = gnoProcBar;
    else if GNOME_IS_SCORES(gnobj) klass = gnoScores;
    else if GNOME_IS_SPELL(gnobj) klass = gnoSpell;
    else if GNOME_IS_STOCK(gnobj) klass = gnoStock;
    else klass = get_gtk_type(gnobj);

    return klass;
}

VALUE
make_gnobject_auto_type(gnobj)
    GtkObject* gnobj;
{
    return make_gobject(get_gnome_type(gnobj), gnobj);
}

static VALUE
gnome_s_is_program_in_path(self, program)
    VALUE self, program;
{
    VALUE res;
    char* path = gnome_is_program_in_path(STR2CSTR(program));
    if (path) {
        res = rb_str_new2(path);
        g_free(path);
    } else {
        res = Qnil;
    }
    return res;
}

VALUE mGnome;

void
Init_gnome()
{
    int argc, i;
    char **argv;

    argc = RARRAY(rb_argv)->len;
    argv = ALLOCA_N(char*,argc+1);
    argv[0] = STR2CSTR(rb_progname);
    for (i=0;i<argc;++i) {
        if (TYPE(RARRAY(rb_argv)->ptr[i]) == T_STRING) {
            argv[i+1] = RSTRING(RARRAY(rb_argv)->ptr[i])->ptr;
        } else {
            argv[i+1] = "";
        }
    }
    ++argc;
    program_invocation_name = 0;
    gnome_init("Ruby/GNOME", "0.0", argc, argv);

    mGnome = rb_define_module("Gnome");

    rb_define_module_function(mGnome, "is_program_in_path",
                              gnome_s_is_program_in_path, 1);

    /* GnomePreferencesType */
    rb_define_const(mGnome, "REFERENCES_NEVER",
		    INT2FIX(GNOME_PREFERENCES_NEVER));
    rb_define_const(mGnome, "REFERENCES_USER",
		    INT2FIX(GNOME_PREFERENCES_USER));
    rb_define_const(mGnome, "REFERENCES_ALWAYS",
		    INT2FIX(GNOME_PREFERENCES_ALWAYS));

    Init_gtk_dial();
    Init_gtk_clock();
    Init_gtk_pixmap_menu_item();

    Init_gnome_animator();
    Init_gnome_app();
    Init_gnome_appbar();
    Init_gnome_app_helper();
    Init_gnome_calculator();
    Init_gnome_canvas();
    Init_gnome_canvas_item();
    Init_gnome_client();
    Init_gnome_color_picker();
    Init_gnome_dateedit();
    Init_gnome_dentry_edit();
    Init_gnome_desktop_entry();
    Init_gnome_dialog();
    Init_gnome_dock();
    Init_gnome_dock_band();
    Init_gnome_dock_item();
    Init_gnome_dock_layout();
    Init_gnome_entry();
    Init_gnome_file_entry();
    Init_gnome_font_picker();
    Init_gnome_href();
    Init_gnome_icon_entry();
    Init_gnome_icon_selection();
    Init_gnome_less();
    Init_gnome_messagebox();
    Init_gnome_number_entry();
    Init_gnome_paper_selector();
    Init_gnome_pixmap();
    Init_gnome_pixmap_entry();
    Init_gnome_proc_bar();
    Init_gnome_scores();
    Init_gnome_spell();
    Init_gnome_stock();

    Init_gnome_config();
    Init_gnome_i18n();
    Init_gnome_uidefs();
    Init_gnome_url();
    Init_gnome_util();
}
