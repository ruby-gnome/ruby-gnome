/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************
 
  rbgtkfilechooser.c -
 
  $Author: mutoh $
  $Date: 2006/06/17 06:59:32 $
 
  Copyright (C) 2005,2006 Ruby-GNOME2 Project Team
  Copyright (C) 2004 Seiya Nishizawa, Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) GTK_FILE_CHOOSER(RVAL2GOBJ(self))

/* For error handling */
#define GTK_FILE_SYSTEM_ENABLE_UNSUPPORTED
#include <gtk/gtkfilesystem.h>

static VALUE
gslist2ary_free(list)
    GSList* list;
{
    GSList* l;
    VALUE ary = rb_ary_new();
    for (l = list; l != NULL; l = g_slist_next(l)) {
        rb_ary_push(ary, CSTR2RVAL2(l->data));
    }
    g_slist_free(list);
    return ary;
}

static VALUE
fcho_set_action(self, action)
    VALUE self, action;
{
    gtk_file_chooser_set_action(
        _SELF(self),
        RVAL2GENUM(action, GTK_TYPE_FILE_CHOOSER_ACTION));
    return self;
}

static VALUE
fcho_get_action(self)
    VALUE self;
{
    return GENUM2RVAL(gtk_file_chooser_get_action(_SELF(self)), GTK_TYPE_FILE_CHOOSER_ACTION);
}

/*  They are defined as properties
    gtk_file_chooser_set_local_only(_SELF(self), RTEST(local));
    gtk_file_chooser_get_local_only(_SELF(self));
    gtk_file_chooser_set_select_multiple(_SELF(self), RTEST(multiple));
    gtk_file_chooser_get_select_multiple(_SELF(self));
*/

/*  Defined as Property
void        gtk_file_chooser_set_show_hidden
                                            (GtkFileChooser *chooser,
                                             gboolean show_hidden);
gboolean    gtk_file_chooser_get_show_hidden
                                            (GtkFileChooser *chooser);
*/

static VALUE
fcho_set_current_name(self, name)
    VALUE self, name;
{
    gtk_file_chooser_set_current_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
fcho_get_filename(self)
    VALUE self;
{
    return CSTR2RVAL2(gtk_file_chooser_get_filename(_SELF(self)));
}

static VALUE
fcho_set_filename(self, name)
    VALUE self, name;
{
    gboolean ret = gtk_file_chooser_set_filename(_SELF(self), RVAL2CSTR(name));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set filename");
    return self;
}

static VALUE
fcho_select_filename(self, filename)
    VALUE self, filename;
{
    return CBOOL2RVAL(gtk_file_chooser_select_filename(_SELF(self), RVAL2CSTR(filename)));
}

static VALUE
fcho_unselect_filename(self, filename)
    VALUE self, filename;
{
    gtk_file_chooser_unselect_filename(_SELF(self), RVAL2CSTR(filename));
    return self;
}

static VALUE
fcho_select_all(self)
    VALUE self;
{
    gtk_file_chooser_select_all(_SELF(self));
    return self;
}

static VALUE
fcho_unselect_all(self)
    VALUE self;
{
    gtk_file_chooser_unselect_all(_SELF(self));
    return self;
}

static VALUE
fcho_get_filenames(self)
    VALUE self;
{
    return gslist2ary_free(gtk_file_chooser_get_filenames(_SELF(self)));
}

static VALUE
fcho_set_current_folder(self, filename)
    VALUE self, filename;
{
    gboolean ret = gtk_file_chooser_set_current_folder(_SELF(self), RVAL2CSTR(filename));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set current folder");
    return self;
}

static VALUE
fcho_get_current_folder(self)
    VALUE self;
{
    return CSTR2RVAL2(gtk_file_chooser_get_current_folder(_SELF(self)));
}

static VALUE
fcho_get_uri(self)
    VALUE self;
{
    return CSTR2RVAL2(gtk_file_chooser_get_uri(_SELF(self)));
}

static VALUE
fcho_set_uri(self, uri)
    VALUE self, uri;
{
    gboolean ret = gtk_file_chooser_set_uri(_SELF(self), RVAL2CSTR(uri));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set uri");
    return self;
}

static VALUE
fcho_select_uri(self, uri)
    VALUE self, uri;
{
    return CBOOL2RVAL(gtk_file_chooser_select_uri(_SELF(self), RVAL2CSTR(uri)));
}

static VALUE
fcho_unselect_uri(self, uri)
    VALUE self, uri;
{
    gtk_file_chooser_unselect_uri(_SELF(self), RVAL2CSTR(uri));
    return self;
}

static VALUE
fcho_get_uris(self)
    VALUE self;
{
    return gslist2ary_free(gtk_file_chooser_get_uris(_SELF(self)));
}

static VALUE
fcho_set_current_folder_uri(self, uri)
    VALUE self, uri;
{
    gboolean ret = gtk_file_chooser_set_current_folder_uri(_SELF(self), RVAL2CSTR(uri));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set current folder uri");
    return self;
}

static VALUE
fcho_get_current_folder_uri(self)
    VALUE self;
{
    return CSTR2RVAL2(gtk_file_chooser_get_current_folder_uri(_SELF(self)));
}

/* They are defined as properties.
    gtk_file_chooser_set_preview_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget));
    gtk_file_chooser_get_preview_widget(_SELF(self)));
    gtk_file_chooser_set_preview_widget_active(_SELF(self), RTEST(active));
    gtk_file_chooser_get_preview_widget_active(_SELF(self));
    gtk_file_chooser_set_use_preview_label(_SELF(self), RTEST(label));
    gtk_file_chooser_get_use_preview_label(_SELF(self));
*/

static VALUE
fcho_get_preview_filename(self)
    VALUE self;
{
    return CSTR2RVAL2(gtk_file_chooser_get_preview_filename(_SELF(self)));
}

static VALUE
fcho_get_preview_uri(self)
    VALUE self;
{
    return CSTR2RVAL2(gtk_file_chooser_get_preview_uri(_SELF(self)));
}

/* They are defined as properties.
    gtk_file_chooser_set_extra_widget(_SELF(self), GTK_WIDGET(RVAL2GOBJ(widget)));
    gtk_file_chooser_get_extra_widget(_SELF(self));
*/

static VALUE
fcho_add_filter(self, filter)
    VALUE self, filter;
{
    gtk_file_chooser_add_filter(_SELF(self), GTK_FILE_FILTER(RVAL2GOBJ(filter)));
    return self;
}

static VALUE
fcho_remove_filter(self, filter)
    VALUE self, filter;
{
    gtk_file_chooser_remove_filter(_SELF(self), GTK_FILE_FILTER(RVAL2GOBJ(filter)));
    return self;
}

static VALUE
fcho_list_filters(self)
    VALUE self;
{
    return GSLIST2ARYF(gtk_file_chooser_list_filters(_SELF(self)));
}

/*
  They are defined as property.
    gtk_file_chooser_set_filter(_SELF(self), GTK_FILE_FILTER(RVAL2GOBJ(filter)));
    gtk_file_chooser_get_filter(_SELF(self));
*/

static VALUE
fcho_add_shortcut_folder(self, folder)
    VALUE self, folder;
{
    GError *error = NULL;
    if (! gtk_file_chooser_add_shortcut_folder(_SELF(self), RVAL2CSTR(folder), &error))
        RAISE_GERROR(error);
    return self;
}

static VALUE
fcho_remove_shortcut_folder(self, folder)
    VALUE self, folder;
{
    GError *error = NULL;
    if (! gtk_file_chooser_remove_shortcut_folder(_SELF(self), RVAL2CSTR(folder), &error))
        RAISE_GERROR(error);
    return self;
}

static VALUE
fcho_list_shortcut_folders(self)
    VALUE self;
{
    return gslist2ary_free(gtk_file_chooser_list_shortcut_folders(_SELF(self)));
}


static VALUE
fcho_add_shortcut_folder_uri(self, uri)
    VALUE self, uri;
{
    GError *error = NULL;
    if (! gtk_file_chooser_add_shortcut_folder_uri(_SELF(self), RVAL2CSTR(uri), &error))
        RAISE_GERROR(error);
    return self;
}

static VALUE
fcho_remove_shortcut_folder_uri(self, uri)
    VALUE self, uri;
{
    GError *error = NULL;
    if (! gtk_file_chooser_remove_shortcut_folder_uri(_SELF(self), RVAL2CSTR(uri), &error))
        RAISE_GERROR(error);
    return self;
}

static VALUE
fcho_list_shortcut_folder_uris(self)
    VALUE self;
{
    return gslist2ary_free(gtk_file_chooser_list_shortcut_folder_uris(_SELF(self)));
}
#endif

/* Properties.
void        gtk_file_chooser_set_do_overwrite_confirmation
                                            (GtkFileChooser *chooser,
                                             gboolean do_overwrite_confirmation);
gboolean    gtk_file_chooser_get_do_overwrite_confirmation
                                           (GtkFileChooser *chooser);
*/

void 
Init_gtk_file_chooser()
{
#if GTK_CHECK_VERSION(2,4,0)

    VALUE gFileCho = G_DEF_INTERFACE(GTK_TYPE_FILE_CHOOSER, "FileChooser", mGtk);
    VALUE fse;

    rb_define_method(gFileCho, "set_action", fcho_set_action, 1);
    rb_define_method(gFileCho, "action", fcho_get_action, 0);
    rb_define_method(gFileCho, "set_current_name", fcho_set_current_name, 1);
    rb_define_method(gFileCho, "set_filename", fcho_set_filename, 1);
    rb_define_method(gFileCho, "filename", fcho_get_filename, 0);
    rb_define_method(gFileCho, "select_filename", fcho_select_filename, 1);
    rb_define_method(gFileCho, "unselect_filename", fcho_unselect_filename, 1);
    rb_define_method(gFileCho, "select_all", fcho_select_all, 0);
    rb_define_method(gFileCho, "unselect_all", fcho_unselect_all, 0);
    rb_define_method(gFileCho, "filenames", fcho_get_filenames, 0);
    rb_define_method(gFileCho, "set_current_folder", fcho_set_current_folder, 1);
    rb_define_method(gFileCho, "current_folder", fcho_get_current_folder, 0);
    rb_define_method(gFileCho, "set_uri", fcho_set_uri, 1);
    rb_define_method(gFileCho, "uri", fcho_get_uri, 0);
    rb_define_method(gFileCho, "select_uri", fcho_select_uri, 1);
    rb_define_method(gFileCho, "unselect_uri", fcho_unselect_uri, 1);
    rb_define_method(gFileCho, "uris", fcho_get_uris, 0);
    rb_define_method(gFileCho, "set_current_folder_uri", fcho_set_current_folder_uri, 1);
    rb_define_method(gFileCho, "current_folder_uri", fcho_get_current_folder_uri, 0);
    rb_define_method(gFileCho, "preview_filename", fcho_get_preview_filename, 0);
    rb_define_method(gFileCho, "preview_uri", fcho_get_preview_uri, 0);
    rb_define_method(gFileCho, "add_filter", fcho_add_filter, 1);
    rb_define_method(gFileCho, "remove_filter", fcho_remove_filter, 1);
    rb_define_method(gFileCho, "filters", fcho_list_filters, 0);
    rb_define_method(gFileCho, "add_shortcut_folder", fcho_add_shortcut_folder, 1);
    rb_define_method(gFileCho, "remove_shortcut_folder", fcho_remove_shortcut_folder, 1);
    rb_define_method(gFileCho, "shortcut_folders", fcho_list_shortcut_folders, 0);
    rb_define_method(gFileCho, "add_shortcut_folder_uri", fcho_add_shortcut_folder_uri, 1);
    rb_define_method(gFileCho, "remove_shortcut_folder_uri", fcho_remove_shortcut_folder_uri, 1);

    rb_define_method(gFileCho, "shortcut_folder_uris", fcho_list_shortcut_folder_uris, 0);

    G_DEF_SETTERS(gFileCho);

    /* GtkFileChooserAction */
    G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_ACTION, "Action", gFileCho);
    G_DEF_CONSTANTS(gFileCho, GTK_TYPE_FILE_CHOOSER_ACTION, "GTK_FILE_CHOOSER_");

    /* GtkFileChooserError */
    G_DEF_ERROR(GTK_FILE_CHOOSER_ERROR, "FileChooserError", mGtk, rb_eRuntimeError, 
                GTK_TYPE_FILE_CHOOSER_ERROR);
    /* GtkFileSystemError */
    fse = G_DEF_ERROR2(GTK_FILE_SYSTEM_ERROR, "FileSystemError", mGtk, rb_eRuntimeError);
    rb_define_const(fse, "NONEXISTENT", INT2NUM(GTK_FILE_SYSTEM_ERROR_NONEXISTENT));
    rb_define_const(fse, "NOT_FOLDER", INT2NUM(GTK_FILE_SYSTEM_ERROR_NOT_FOLDER));
    rb_define_const(fse, "INVALID_URI", INT2NUM(GTK_FILE_SYSTEM_ERROR_INVALID_URI));
    rb_define_const(fse, "BAD_FILENAME", INT2NUM(GTK_FILE_SYSTEM_ERROR_BAD_FILENAME));
    rb_define_const(fse, "FAILED", INT2NUM(GTK_FILE_SYSTEM_ERROR_FAILED));
    rb_define_const(fse, "ALREADY_EXSITS", INT2NUM(GTK_FILE_SYSTEM_ERROR_ALREADY_EXISTS));

#if GTK_CHECK_VERSION(2,8,0)
    /* GtkFileChooserConfirmation */
    G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_CONFIRMATION, "Confirmation", gFileCho);
    G_DEF_CONSTANTS(gFileCho, GTK_TYPE_FILE_CHOOSER_CONFIRMATION, "GTK_FILE_CHOOSER_");
#endif

    G_DEF_CLASS3("GtkFileChooserEmbed", "FileChooserEmbed", mGtk);

#endif
}

