/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define RG_TARGET_NAMESPACE mRC

/*
static VALUE
rc_scanner_new(VALUE self)
{
    return BOXED2RVAL(gtk_rc_scanner_new(), GTK_TYPE_GSCANNER);
}
*/

static VALUE
rg_m_get_style(G_GNUC_UNUSED VALUE self, VALUE widget)
{
    GtkStyle* style = gtk_rc_get_style(GTK_WIDGET(RVAL2GOBJ(widget)));
    GType gtype = G_OBJECT_TYPE(style);
    const gchar* name = G_OBJECT_TYPE_NAME(style);
    if (! rb_const_defined_at(mGtk, rb_intern(name))){
        G_DEF_CLASS(gtype, (gchar*)name, mGtk);
    }
    return GOBJ2RVAL(gtk_rc_get_style(GTK_WIDGET(RVAL2GOBJ(widget))));
}

static VALUE
rg_m_get_style_by_paths(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE settings, widget_path, class_path, klass;
    GtkStyle* style;
    GType gtype;
    const gchar* name;

    rb_scan_args(argc, argv, "13", &settings, &widget_path, &class_path, &klass);

    style = gtk_rc_get_style_by_paths(GTK_SETTINGS(RVAL2GOBJ(settings)),
                                      NIL_P(widget_path) ? NULL : RVAL2CSTR(widget_path),
                                      NIL_P(class_path) ? NULL : RVAL2CSTR(class_path),
                                      NIL_P(klass) ? G_TYPE_NONE : CLASS2GTYPE(klass));

    if (style){
        gtype = G_OBJECT_TYPE(style);
        name = G_OBJECT_TYPE_NAME(style);
        if (! rb_const_defined_at(mGtk, rb_intern(name))){
            G_DEF_CLASS(gtype, (gchar*)name, mGtk);
        }
        return GOBJ2RVAL(style);
    }
    return Qnil;
}

static VALUE
rg_m_parse(G_GNUC_UNUSED VALUE self, VALUE filename)
{
    gtk_rc_parse(RVAL2CSTR(filename));
    return filename;
}

static VALUE
rg_m_parse_string(G_GNUC_UNUSED VALUE self, VALUE rc_string)
{
    gtk_rc_parse_string(RVAL2CSTR(rc_string));
    return rc_string;
}

static VALUE
rg_m_reparse_all(G_GNUC_UNUSED VALUE self)
{
    return CBOOL2RVAL(gtk_rc_reparse_all());
}

static VALUE
rg_m_reparse_all_for_settings(G_GNUC_UNUSED VALUE self, VALUE settings, VALUE force_load)
{
    return CBOOL2RVAL(gtk_rc_reparse_all_for_settings(GTK_SETTINGS(RVAL2GOBJ(settings)), 
                                                      RVAL2CBOOL(force_load)));
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_m_reset_styles(G_GNUC_UNUSED VALUE self, VALUE settings)
{
    gtk_rc_reset_styles(GTK_SETTINGS(RVAL2GOBJ(settings)));
    return settings;
}
#endif

static VALUE
rg_m_add_default_file(G_GNUC_UNUSED VALUE self, VALUE filename)
{
    gtk_rc_add_default_file(RVAL2CSTR(filename));
    return filename;
}

static VALUE
rg_m_default_files(G_GNUC_UNUSED VALUE self)
{
    gchar** files = gtk_rc_get_default_files();
    VALUE ary = rb_ary_new();
    while(*files){
        rb_ary_push(ary, CSTR2RVAL(*files));
        files++;
    }
    return ary;
}

static VALUE
rg_m_set_default_files(G_GNUC_UNUSED VALUE self, VALUE rbfilenames)
{
    gchar **filenames = (gchar **)RVAL2STRV(rbfilenames);

    gtk_rc_set_default_files(filenames);

    g_free(filenames);

    return rbfilenames;
}
/*
guint       gtk_rc_parse_color              (GScanner *scanner,
                                             GdkColor *color);
guint       gtk_rc_parse_color_full         (GScanner *scanner,
                                             GtkRcStyle *style,
                                             GdkColor *color);
guint       gtk_rc_parse_state              (GScanner *scanner,
                                             GtkStateType *state);
guint       gtk_rc_parse_priority           (GScanner *scanner,
                                             GtkPathPriorityType *priority);
*/

static VALUE
rg_m_find_module_in_path(G_GNUC_UNUSED VALUE self, VALUE module_file)
{
    return CSTR2RVAL_FREE(gtk_rc_find_module_in_path(RVAL2CSTR(module_file)));
}

/*
gchar*      gtk_rc_find_pixmap_in_path      (GtkSettings *settings,
                                             GScanner *scanner,
                                             const gchar *pixmap_file);
*/

static VALUE
rg_m_module_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL_FREE(gtk_rc_get_module_dir());
}

static VALUE
rg_m_im_module_path(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(gtk_rc_get_im_module_path());
}

static VALUE
rg_m_im_module_file(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL(gtk_rc_get_im_module_file());
}

static VALUE
rg_m_theme_dir(G_GNUC_UNUSED VALUE self)
{
    return CSTR2RVAL_FREE(gtk_rc_get_theme_dir());
}

void 
Init_gtk_rc(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGtk, "RC");

    RG_DEF_MODFUNC(get_style, 1);
    RG_DEF_MODFUNC(get_style_by_paths, -1);
    RG_DEF_MODFUNC(parse, 1);
    RG_DEF_MODFUNC(parse_string, 1);
    RG_DEF_MODFUNC(reparse_all, 0);
    RG_DEF_MODFUNC(reparse_all_for_settings, 2);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_MODFUNC(reset_styles, 1);
#endif
    RG_DEF_MODFUNC(add_default_file, 1);
    RG_DEF_MODFUNC(default_files, 0);
    RG_DEF_MODFUNC(set_default_files, 1);
    RG_DEF_MODFUNC(find_module_in_path, 1);
    RG_DEF_MODFUNC(module_dir, 0);
    RG_DEF_MODFUNC(im_module_path, 0);
    RG_DEF_MODFUNC(im_module_file, 0);
    RG_DEF_MODFUNC(theme_dir, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkRcFlags */
    G_DEF_CLASS(GTK_TYPE_RC_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_RC_FLAGS, "GTK_RC_");

    /* GtkRcTokenType */
    G_DEF_CLASS(GTK_TYPE_RC_TOKEN_TYPE, "TokenType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_RC_TOKEN_TYPE, "GTK_RC_");
}
