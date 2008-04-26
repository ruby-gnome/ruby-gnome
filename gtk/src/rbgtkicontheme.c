/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkicontheme.c -

  $Author: ggc $
  $Date: 2006/06/22 19:52:54 $

  Copyright (C) 2004,2005 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)
#define _SELF(i) GTK_ICON_THEME(RVAL2GOBJ(i))

#define RVAL2ICON_LOOKUP_FLAGS(flags) (RVAL2GFLAGS(flags, GTK_TYPE_ICON_LOOKUP_FLAGS))
#define ICON_INFO2RVAL(info) (BOXED2RVAL(info, GTK_TYPE_ICON_INFO))

static VALUE
it_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, gtk_icon_theme_new());
    return Qnil;
}

static VALUE
it_get_default(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_icon_theme_get_default());
}

static VALUE
it_get_for_screen(self, screen)
    VALUE self, screen;
{
    return GOBJ2RVAL(gtk_icon_theme_get_for_screen(GDK_SCREEN(RVAL2GOBJ(screen))));
}

static VALUE
it_set_screen(self, screen)
    VALUE self, screen;
{
    gtk_icon_theme_set_screen(_SELF(self), GDK_SCREEN(RVAL2GOBJ(screen)));
    return self;
}

static VALUE
it_set_search_path(self, paths)
    VALUE self, paths;
{
    gchar** gpaths;
    gint size, i;

    Check_Type(paths, T_ARRAY);
    
    size = RARRAY(paths)->len;
    gpaths = g_new(gchar*, size);

    for (i = 0; i < size; i++) { 
        gpaths[i] = (gchar*)(RARRAY(paths)->ptr[0]);
    }
    gtk_icon_theme_set_search_path(_SELF(self), (const gchar**)gpaths, size);
    g_free(gpaths);
    return self;
}

static VALUE
it_get_search_path(self)
    VALUE self;
{
    gchar** path;
    gint size, i;

    
    VALUE ret = rb_ary_new();
    gtk_icon_theme_get_search_path(_SELF(self), &path, &size);

    for (i = 0; i < size; i++){
        rb_ary_push(ret, CSTR2RVAL(path[i]));
    }
    g_strfreev(path);
    return ret;
}

static VALUE
it_append_search_path(self, path)
    VALUE self, path;
{
    gtk_icon_theme_append_search_path(_SELF(self), RVAL2CSTR(path));
    return self;
}

static VALUE
it_prepend_search_path(self, path)
    VALUE self, path;
{
    gtk_icon_theme_prepend_search_path(_SELF(self), RVAL2CSTR(path));
    return self;
}

static VALUE
it_set_custom_theme(self, theme_name)
    VALUE self, theme_name;
{
    gtk_icon_theme_set_custom_theme(_SELF(self), 
                                    NIL_P(theme_name) ? NULL : RVAL2CSTR(theme_name));
    return self;
}

static VALUE
it_has_icon(self, icon_name)
    VALUE self, icon_name;
{
    return CBOOL2RVAL(gtk_icon_theme_has_icon(_SELF(self), 
                                              RVAL2CSTR(icon_name)));
}

static VALUE
it_lookup_icon(self, icon_name, size, flags)
    VALUE self, icon_name, size, flags;
{
    GtkIconInfo* info;

    info = gtk_icon_theme_lookup_icon(_SELF(self),
				      RVAL2CSTR(icon_name),
				      NUM2INT(size),
				      RVAL2ICON_LOOKUP_FLAGS(flags));
    return ICON_INFO2RVAL(info);
}

static VALUE
it_load_icon(self, icon_name, size, flags)
    VALUE self, icon_name, size, flags;
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
it_list_icons(argc, argv, self)
    gint argc;
    VALUE* argv;
    VALUE self;
{
    VALUE context;
    GList* list;
    GList* old;
    VALUE ary = rb_ary_new();

    rb_scan_args(argc, argv, "01", &context);

    list = gtk_icon_theme_list_icons(_SELF(self), 
                                     NIL_P(context) ? NULL : RVAL2CSTR(context));
    old = list;
    while (list) {
        rb_ary_push(ary, CSTR2RVAL2(list->data));
        list = list->next;
    }
    g_list_free(old);
    return ary;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
it_get_icon_sizes(self, icon_name)
    VALUE self, icon_name;
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
it_get_example_icon_name(self)
    VALUE self;
{
    return CSTR2RVAL2(gtk_icon_theme_get_example_icon_name(_SELF(self)));
}

static VALUE
it_rescan_if_needed(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_icon_theme_rescan_if_needed(_SELF(self)));
}

static VALUE
it_s_add_builtin_icon(self, icon_name, size, pixbuf)
    VALUE self, icon_name, size, pixbuf;
{
    gtk_icon_theme_add_builtin_icon(RVAL2CSTR(icon_name), NUM2INT(size),
                                    GDK_PIXBUF(RVAL2GOBJ(pixbuf)));
    return self;
}

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
it_choose_icon(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_icon_names, rb_size, rb_flags;
    gint size;
    gchar **icon_names;
    GtkIconLookupFlags flags;
    GtkIconInfo *info;

    rb_scan_args(argc, argv, "21", &rb_icon_names, &rb_size, &rb_flags);

    if (RVAL2CBOOL(rb_obj_is_kind_of(rb_icon_names, rb_cString))) {
	icon_names = ALLOCA_N(gchar *, 2);
	icon_names[0] = RVAL2CSTR(rb_icon_names);
	icon_names[1] = NULL;
    }
    else if (RVAL2CBOOL(rb_obj_is_kind_of(rb_icon_names, rb_cArray))) {
	VALUE *elements;
	long i, len;

	len = RARRAY_LEN(rb_icon_names);
	icon_names = ALLOCA_N(gchar *, len + 1);
	elements = RARRAY_PTR(rb_icon_names);
	for (i = 0; i < len; i++) {
	    icon_names[i] = RVAL2CSTR(elements[i]);
	}
	icon_names[i] = NULL;
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
				      (const gchar **)icon_names,
				      size, flags);
    return ICON_INFO2RVAL(info);
}

static VALUE
it_list_contexts(VALUE self)
{
    return GLIST2ARY_STR_FREE(gtk_icon_theme_list_contexts(_SELF(self)));
}
#endif

#endif

void
Init_gtk_icon_theme()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE it = G_DEF_CLASS(GTK_TYPE_ICON_THEME, "IconTheme", mGtk);

    rb_define_method(it, "initialize", it_initialize, 0);
    rb_define_singleton_method(it, "default", it_get_default, 0);
    rb_define_singleton_method(it, "get_for_screen", it_get_for_screen, 1);
    rb_define_method(it, "set_screen", it_set_screen, 1);
    rb_define_method(it, "set_search_path", it_set_search_path, 1);
    rb_define_method(it, "search_path", it_get_search_path, 0);
    rb_define_method(it, "append_search_path", it_append_search_path, 1);
    rb_define_method(it, "prepend_search_path", it_prepend_search_path, 1);
    rb_define_method(it, "set_custom_theme", it_set_custom_theme, 1);
    rb_define_method(it, "has_icon?", it_has_icon, 1);
    rb_define_method(it, "lookup_icon", it_lookup_icon, 3);
    rb_define_method(it, "load_icon", it_load_icon, 3);
    rb_define_method(it, "icons", it_list_icons, -1);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(it, "get_icon_sizes", it_get_icon_sizes, 1);
#endif
    rb_define_method(it, "example_icon_name", it_get_example_icon_name, 0);
    rb_define_method(it, "rescan_if_needed", it_rescan_if_needed, 0);

    rb_define_singleton_method(it, "add_builtin_icon", it_s_add_builtin_icon, 3);

#if GTK_CHECK_VERSION(2, 12, 0)
    rb_define_method(it, "choose_icon", it_choose_icon, -1);
    rb_define_method(it, "contexts", it_list_contexts, 0);
#endif

    G_DEF_SETTERS(it);

    /* GtkIconThemeError */
    G_DEF_ERROR(GTK_ICON_THEME_ERROR, "IconThemeError", mGtk, rb_eRuntimeError,
                GTK_TYPE_ICON_THEME_ERROR);

    /* GtkIconLookupFlags */
    G_DEF_CLASS(GTK_TYPE_ICON_LOOKUP_FLAGS, "LookupFlags", it);
    G_DEF_CONSTANTS(it, GTK_TYPE_ICON_LOOKUP_FLAGS, "GTK_ICON_");

#endif
}
