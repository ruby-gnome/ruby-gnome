/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrc.c -

  $Author: mutoh $
  $Date: 2003/05/18 17:02:41 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
static VALUE
rc_scanner_new(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_rc_scanner_new(), GTK_TYPE_GSCANNER);
}
*/

static VALUE
rc_get_style(self, widget)
    VALUE self, widget;
{
    GtkStyle* style = gtk_rc_get_style(GTK_WIDGET(RVAL2GOBJ(widget)));
    GType gtype = G_OBJECT_TYPE(style);
    const gchar* name = G_OBJECT_TYPE_NAME(style);
    if (! rbgobj_exist_class(gtype)){
        G_DEF_CLASS(gtype, (gchar*)name, mGtk);
    }
    return GOBJ2RVAL(gtk_rc_get_style(GTK_WIDGET(RVAL2GOBJ(widget))));
}

static VALUE
rc_get_style_by_paths(self, settings, widget_path, class_path, klass)
    VALUE self, settings, widget_path, class_path, klass;
{
    GtkStyle* style = gtk_rc_get_style_by_paths(GTK_SETTINGS(RVAL2GOBJ(settings)),
                                                NIL_P(widget_path) ? NULL : RVAL2CSTR(widget_path),
                                                NIL_P(class_path) ? NULL : RVAL2CSTR(class_path),
                                                CLASS2GTYPE(klass));
    GType gtype = G_OBJECT_TYPE(style);
    const gchar* name = G_OBJECT_TYPE_NAME(style);
    if (! rbgobj_exist_class(gtype)){
        G_DEF_CLASS(gtype, (gchar*)name, mGtk);
    }    
    return style ? GOBJ2RVAL(style) : Qnil;
}

static VALUE
rc_parse(self, filename)
    VALUE self, filename;
{
    gtk_rc_parse(RVAL2CSTR(filename));
    return filename;
}

static VALUE
rc_parse_string(self, rc_string)
    VALUE self, rc_string;
{
    gtk_rc_parse_string(RVAL2CSTR(rc_string));
    return rc_string;
}

static VALUE
rc_reparse_all(self)
    VALUE self;
{
    return gtk_rc_reparse_all() ? Qtrue : Qfalse;
}

static VALUE
rc_reparse_all_for_settings(self, settings, force_load)
    VALUE self, settings, force_load;
{
    return gtk_rc_reparse_all_for_settings(GTK_SETTINGS(RVAL2GOBJ(settings)), 
                                           RTEST(force_load)) ? Qtrue : Qfalse;
}

static VALUE
rc_add_default_file(self, filename)
    VALUE self, filename;
{
    gtk_rc_add_default_file(RVAL2CSTR(filename));
    return filename;
}

static VALUE
rc_get_default_files(self)
    VALUE self;
{
    gchar** files = gtk_rc_get_default_files();
    VALUE ary = rb_ary_new();
    while(*files){
        rb_ary_push(ary, CSTR2RVAL(*files));
        *files++;
    }
    return ary;
}

static VALUE
rc_set_default_files(self, filenames)
    VALUE self, filenames;
{
    int i;
    gchar** gfiles = g_new(gchar*, RARRAY(filenames)->len + 1);
    for (i = 0; i < RARRAY(filenames)->len; i++) {
        gfiles[i] = RVAL2CSTR(RARRAY(filenames)->ptr[i]);
    }
    gfiles[RARRAY(filenames)->len] = NULL;
    gtk_rc_set_default_files(gfiles);
    return filenames;
}
/*
guint       gtk_rc_parse_color              (GScanner *scanner,
                                             GdkColor *color);
guint       gtk_rc_parse_state              (GScanner *scanner,
                                             GtkStateType *state);
guint       gtk_rc_parse_priority           (GScanner *scanner,
                                             GtkPathPriorityType *priority);
*/

static VALUE
rc_find_module_in_path(self, module_file)
    VALUE self, module_file;
{
    return CSTR2RVAL(gtk_rc_find_module_in_path(RVAL2CSTR(module_file)));
}

/*
gchar*      gtk_rc_find_pixmap_in_path      (GtkSettings *settings,
                                             GScanner *scanner,
                                             const gchar *pixmap_file);
*/

static VALUE
rc_get_module_dir(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_rc_get_module_dir());
}

static VALUE
rc_get_im_module_path(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_rc_get_im_module_path());
}

static VALUE
rc_get_im_module_file(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_rc_get_im_module_file());
}

static VALUE
rc_get_theme_dir(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_rc_get_theme_dir());
}

void 
Init_gtk_rc()
{
    VALUE mRC = rb_define_module_under(mGtk, "RC");

    rb_define_module_function(mRC, "get_style", rc_get_style, 1);
    rb_define_module_function(mRC, "get_style_by_paths", rc_get_style_by_paths, 4);
    rb_define_module_function(mRC, "parse", rc_parse, 1);
    rb_define_module_function(mRC, "parse_string", rc_parse_string, 1);
    rb_define_module_function(mRC, "reparse_all", rc_reparse_all, 0);
    rb_define_module_function(mRC, "reparse_all_for_settings", rc_reparse_all_for_settings, 2);
    rb_define_module_function(mRC, "add_default_file", rc_add_default_file, 1);
    rb_define_module_function(mRC, "default_files", rc_get_default_files, 0);
    rb_define_module_function(mRC, "set_default_files", rc_set_default_files, 1);
    rb_define_module_function(mRC, "find_module_in_path", rc_find_module_in_path, 1);
    rb_define_module_function(mRC, "module_dir", rc_get_module_dir, 0);
    rb_define_module_function(mRC, "im_module_path", rc_get_im_module_path, 0);
    rb_define_module_function(mRC, "im_module_file", rc_get_im_module_file, 0);
    rb_define_module_function(mRC, "theme_dir", rc_get_theme_dir, 0);

    G_DEF_SETTERS(mRC);
}
