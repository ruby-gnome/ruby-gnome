/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE cIconTheme
#define _SELF(i) GTK_ICON_THEME(RVAL2GOBJ(i))

#define RVAL2ICON_LOOKUP_FLAGS(flags) (RVAL2GFLAGS(flags, GTK_TYPE_ICON_LOOKUP_FLAGS))
#define ICON_INFO2RVAL(info) (BOXED2RVAL(info, GTK_TYPE_ICON_INFO))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_icon_theme_new());
    return Qnil;
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gtk_icon_theme_get_default());
}

static VALUE
rg_s_get_for_screen(G_GNUC_UNUSED VALUE self, VALUE screen)
{
    return GOBJ2RVAL(gtk_icon_theme_get_for_screen(GDK_SCREEN(RVAL2GOBJ(screen))));
}

static VALUE
rg_set_screen(VALUE self, VALUE screen)
{
    gtk_icon_theme_set_screen(_SELF(self), GDK_SCREEN(RVAL2GOBJ(screen)));
    return self;
}

static VALUE
rg_set_search_path(VALUE self, VALUE rbpath)
{
    GtkIconTheme *theme = _SELF(self);
    long n;
    const gchar **path = RVAL2STRS(rbpath, n);

    gtk_icon_theme_set_search_path(theme, path, n);

    g_free(path);

    return self;
}

static VALUE
rg_search_path(VALUE self)
{
    gchar **path;

    gtk_icon_theme_get_search_path(_SELF(self), &path, NULL);

    return STRV2RVAL_FREE(path);
}

static VALUE
rg_append_search_path(VALUE self, VALUE path)
{
    gtk_icon_theme_append_search_path(_SELF(self), RVAL2CSTR(path));
    return self;
}

static VALUE
rg_prepend_search_path(VALUE self, VALUE path)
{
    gtk_icon_theme_prepend_search_path(_SELF(self), RVAL2CSTR(path));
    return self;
}

static VALUE
rg_set_custom_theme(VALUE self, VALUE theme_name)
{
    gtk_icon_theme_set_custom_theme(_SELF(self), 
                                    NIL_P(theme_name) ? NULL : RVAL2CSTR(theme_name));
    return self;
}

static VALUE
rg_has_icon_p(VALUE self, VALUE icon_name)
{
    return CBOOL2RVAL(gtk_icon_theme_has_icon(_SELF(self), 
                                              RVAL2CSTR(icon_name)));
}

static VALUE
rg_lookup_icon(VALUE self, VALUE icon_name, VALUE size, VALUE flags)
{
    GtkIconInfo* info;

    info = gtk_icon_theme_lookup_icon(_SELF(self),
                                      RVAL2CSTR(icon_name),
                                      NUM2INT(size),
                                      RVAL2ICON_LOOKUP_FLAGS(flags));
    return ICON_INFO2RVAL(info);
}

static VALUE
rg_load_icon(VALUE self, VALUE icon_name, VALUE size, VALUE flags)
{
    GError *error = NULL;
    GdkPixbuf *pixbuf;

    pixbuf = gtk_icon_theme_load_icon(_SELF(self),
                                      RVAL2CSTR(icon_name),
                                      NUM2INT(size),
                                      RVAL2ICON_LOOKUP_FLAGS(flags),
                                      &error);
    if (!pixbuf)
    RAISE_GERROR(error);
    return GOBJ2RVAL(pixbuf);
}

static VALUE
rg_icons(int argc, VALUE *argv, VALUE self)
{
    VALUE context;
    GList *icons;

    rb_scan_args(argc, argv, "01", &context);

    icons = gtk_icon_theme_list_icons(_SELF(self),
                                      RVAL2CSTR_ACCEPT_NIL(context));
    return GLIST2ARY_STR_FREE(icons);
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_get_icon_sizes(VALUE self, VALUE icon_name)
{
    VALUE ary = rb_ary_new();

    gint* sizes = gtk_icon_theme_get_icon_sizes(_SELF(self), RVAL2CSTR(icon_name));
    gint* tmp_sizes = sizes;
    while (*tmp_sizes) {
        rb_ary_push(ary, INT2NUM(*tmp_sizes));
        tmp_sizes++;
    }
    g_free(sizes);
    return ary;
}
#endif

static VALUE
rg_example_icon_name(VALUE self)
{
    return CSTR2RVAL_FREE(gtk_icon_theme_get_example_icon_name(_SELF(self)));
}

static VALUE
rg_rescan_if_needed(VALUE self)
{
    return CBOOL2RVAL(gtk_icon_theme_rescan_if_needed(_SELF(self)));
}

static VALUE
rg_s_add_builtin_icon(VALUE self, VALUE icon_name, VALUE size, VALUE pixbuf)
{
    gtk_icon_theme_add_builtin_icon(RVAL2CSTR(icon_name), NUM2INT(size),
                                    GDK_PIXBUF(RVAL2GOBJ(pixbuf)));
    return self;
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
rg_choose_icon(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_icon_names, rb_size, rb_flags;
    gint size;
    const gchar **icon_names;
    GtkIconLookupFlags flags;
    GtkIconInfo *info;

    rb_scan_args(argc, argv, "21", &rb_icon_names, &rb_size, &rb_flags);

    if (RVAL2CBOOL(rb_obj_is_kind_of(rb_icon_names, rb_cString))) {
        icon_names = ALLOCA_N(const gchar *, 2);
        icon_names[0] = RVAL2CSTR(rb_icon_names);
        icon_names[1] = NULL;
    }
    else if (RVAL2CBOOL(rb_obj_is_kind_of(rb_icon_names, rb_cArray))) {
        icon_names = RVAL2STRV(rb_icon_names);
    }
    else {
        rb_raise(rb_eArgError,
             "expected (name, size), (name, size, flags), "
             "([name1, name2, ...], size) or "
             "([name1, name2, ...], size, flags): %s",
             RBG_INSPECT(rb_ary_new4(argc, argv)));
    }

    size = NUM2INT(rb_size);

    if (NIL_P(rb_flags)) {
        flags = GTK_ICON_LOOKUP_GENERIC_FALLBACK;
    }
    else {
        flags = RVAL2ICON_LOOKUP_FLAGS(rb_flags);
    }

    info = gtk_icon_theme_choose_icon(_SELF(self),
                                      icon_names,
                                      size, flags);
    return ICON_INFO2RVAL(info);
}

static VALUE
rg_contexts(VALUE self)
{
    return GLIST2ARY_STR_FREE(gtk_icon_theme_list_contexts(_SELF(self)));
}
#endif

#endif

void
Init_gtk_icon_theme(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ICON_THEME, "IconTheme", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_SMETHOD(default, 0);
    RG_DEF_SMETHOD(get_for_screen, 1);
    RG_DEF_METHOD(set_screen, 1);
    RG_DEF_METHOD(set_search_path, 1);
    RG_DEF_METHOD(search_path, 0);
    RG_DEF_METHOD(append_search_path, 1);
    RG_DEF_METHOD(prepend_search_path, 1);
    RG_DEF_METHOD(set_custom_theme, 1);
    RG_DEF_METHOD_P(has_icon, 1);
    RG_DEF_METHOD(lookup_icon, 3);
    RG_DEF_METHOD(load_icon, 3);
    RG_DEF_METHOD(icons, -1);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(get_icon_sizes, 1);
#endif
    RG_DEF_METHOD(example_icon_name, 0);
    RG_DEF_METHOD(rescan_if_needed, 0);

    RG_DEF_SMETHOD(add_builtin_icon, 3);

#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD(choose_icon, -1);
    RG_DEF_METHOD(contexts, 0);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkIconThemeError */
    G_DEF_ERROR(GTK_ICON_THEME_ERROR, "IconThemeError", mGtk, rb_eRuntimeError,
                GTK_TYPE_ICON_THEME_ERROR);

    /* GtkIconLookupFlags */
    G_DEF_CLASS(GTK_TYPE_ICON_LOOKUP_FLAGS, "LookupFlags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_ICON_LOOKUP_FLAGS, "GTK_ICON_");

#endif
}
