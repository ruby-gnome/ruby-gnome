/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005,2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Seiya Nishizawa, Masao Mutoh
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

#define RG_TARGET_NAMESPACE mFileChooser
#define _SELF(self) GTK_FILE_CHOOSER(RVAL2GOBJ(self))

#ifdef HAVE_GTK_GTKFILESYSTEM_H
/* For error handling */
#  define GTK_FILE_SYSTEM_ENABLE_UNSUPPORTED
#  include <gtk/gtkfilesystem.h>
#endif

static VALUE
gslist2ary_free(GSList* list)
{
    GSList* l;
    VALUE ary = rb_ary_new();
    for (l = list; l != NULL; l = g_slist_next(l)) {
        rb_ary_push(ary, CSTR2RVAL_FREE(l->data));
    }
    g_slist_free(list);
    return ary;
}

static VALUE
fcho_set_current_name(VALUE self, VALUE name)
{
    /* doc says the awaited string is UTF-8, so use RVAL2CSTR */
    gtk_file_chooser_set_current_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
fcho_get_filename(VALUE self)
{
    return CSTRFILENAME2RVAL_FREE(gtk_file_chooser_get_filename(_SELF(self)));
}

static VALUE
fcho_set_filename(VALUE self, VALUE rbfilename)
{
    gchar *filename = RVAL2CSTRFILENAME(rbfilename);
    gboolean ret = gtk_file_chooser_set_filename(_SELF(self), filename);
    g_free(filename);
    if (!ret)
        rb_raise(rb_eRuntimeError, "Can't set filename");

    return self;
}

static VALUE
fcho_select_filename(VALUE self, VALUE rbfilename)
{
    gchar *filename = RVAL2CSTRFILENAME(rbfilename);
    gboolean ret = gtk_file_chooser_select_filename(_SELF(self), filename);
    g_free(filename);

    return CBOOL2RVAL(ret);
}

static VALUE
fcho_unselect_filename(VALUE self, VALUE rbfilename)
{
    gchar *filename = RVAL2CSTRFILENAME(rbfilename);
    gtk_file_chooser_unselect_filename(_SELF(self), filename);
    g_free(filename);

    return self;
}

static VALUE
fcho_select_all(VALUE self)
{
    gtk_file_chooser_select_all(_SELF(self));
    return self;
}

static VALUE
fcho_unselect_all(VALUE self)
{
    gtk_file_chooser_unselect_all(_SELF(self));
    return self;
}

static VALUE
fcho_get_filenames(VALUE self)
{
    return CSTRFILENAMEARRAY2RVAL_FREE(gtk_file_chooser_get_filenames(_SELF(self)));
}

static VALUE
fcho_set_current_folder(VALUE self, VALUE filename)
{
    gboolean ret = gtk_file_chooser_set_current_folder(_SELF(self), RVAL2CSTR(filename));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set current folder");
    return self;
}

static VALUE
fcho_get_current_folder(VALUE self)
{
    return CSTR2RVAL_FREE(gtk_file_chooser_get_current_folder(_SELF(self)));
}

static VALUE
fcho_get_uri(VALUE self)
{
    return CSTR2RVAL_FREE(gtk_file_chooser_get_uri(_SELF(self)));
}

static VALUE
fcho_set_uri(VALUE self, VALUE uri)
{
    gboolean ret = gtk_file_chooser_set_uri(_SELF(self), RVAL2CSTR(uri));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set uri");
    return self;
}

static VALUE
fcho_select_uri(VALUE self, VALUE uri)
{
    return CBOOL2RVAL(gtk_file_chooser_select_uri(_SELF(self), RVAL2CSTR(uri)));
}

static VALUE
fcho_unselect_uri(VALUE self, VALUE uri)
{
    gtk_file_chooser_unselect_uri(_SELF(self), RVAL2CSTR(uri));
    return self;
}

static VALUE
fcho_get_uris(VALUE self)
{
    return gslist2ary_free(gtk_file_chooser_get_uris(_SELF(self)));
}

static VALUE
fcho_set_current_folder_uri(VALUE self, VALUE uri)
{
    gboolean ret = gtk_file_chooser_set_current_folder_uri(_SELF(self), RVAL2CSTR(uri));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set current folder uri");
    return self;
}

static VALUE
fcho_get_current_folder_uri(VALUE self)
{
    return CSTR2RVAL_FREE(gtk_file_chooser_get_current_folder_uri(_SELF(self)));
}

static VALUE
fcho_get_preview_filename(VALUE self)
{
    return CSTR2RVAL_FREE(gtk_file_chooser_get_preview_filename(_SELF(self)));
}

static VALUE
fcho_get_preview_uri(VALUE self)
{
    return CSTR2RVAL_FREE(gtk_file_chooser_get_preview_uri(_SELF(self)));
}

static VALUE
fcho_add_filter(VALUE self, VALUE filter)
{
    gtk_file_chooser_add_filter(_SELF(self), GTK_FILE_FILTER(RVAL2GOBJ(filter)));
    return self;
}

static VALUE
fcho_remove_filter(VALUE self, VALUE filter)
{
    gtk_file_chooser_remove_filter(_SELF(self), GTK_FILE_FILTER(RVAL2GOBJ(filter)));
    return self;
}

static VALUE
fcho_list_filters(VALUE self)
{
    return GSLIST2ARYF(gtk_file_chooser_list_filters(_SELF(self)));
}

static VALUE
fcho_add_shortcut_folder(VALUE self, VALUE rbfolder)
{
    gchar *folder = RVAL2CSTRFILENAME(rbfolder);
    GError *error = NULL;
    gboolean ret = gtk_file_chooser_add_shortcut_folder(_SELF(self), folder, &error);
    g_free(folder);
    if (!ret)
        RAISE_GERROR(error);

    return self;
}

static VALUE
fcho_remove_shortcut_folder(VALUE self, VALUE rbfolder)
{
    gchar *folder = RVAL2CSTRFILENAME(rbfolder);
    GError *error = NULL;
    gboolean ret = gtk_file_chooser_remove_shortcut_folder(_SELF(self), folder, &error);
    g_free(folder);
    if (!ret)
        RAISE_GERROR(error);

    return self;
}

static VALUE
fcho_list_shortcut_folders(VALUE self)
{
    return CSTRFILENAMEARRAY2RVAL_FREE(gtk_file_chooser_list_shortcut_folders(_SELF(self)));
}


static VALUE
fcho_add_shortcut_folder_uri(VALUE self, VALUE uri)
{
    GError *error = NULL;
    if (! gtk_file_chooser_add_shortcut_folder_uri(_SELF(self), RVAL2CSTR(uri), &error))
        RAISE_GERROR(error);
    return self;
}

static VALUE
fcho_remove_shortcut_folder_uri(VALUE self, VALUE uri)
{
    GError *error = NULL;
    if (! gtk_file_chooser_remove_shortcut_folder_uri(_SELF(self), RVAL2CSTR(uri), &error))
        RAISE_GERROR(error);
    return self;
}

static VALUE
fcho_list_shortcut_folder_uris(VALUE self)
{
    return gslist2ary_free(gtk_file_chooser_list_shortcut_folder_uris(_SELF(self)));
}
#endif

void 
Init_gtk_file_chooser(void)
{

#if GTK_CHECK_VERSION(2,4,0)

    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_FILE_CHOOSER, "FileChooser", mGtk);
#ifdef GTK_FILE_SYSTEM_ERROR
    VALUE eFileSystemError;
#endif

    rb_define_method(RG_TARGET_NAMESPACE, "set_current_name", fcho_set_current_name, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_filename", fcho_set_filename, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "filename", fcho_get_filename, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "select_filename", fcho_select_filename, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "unselect_filename", fcho_unselect_filename, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "select_all", fcho_select_all, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "unselect_all", fcho_unselect_all, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "filenames", fcho_get_filenames, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_current_folder", fcho_set_current_folder, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "current_folder", fcho_get_current_folder, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_uri", fcho_set_uri, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "uri", fcho_get_uri, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "select_uri", fcho_select_uri, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "unselect_uri", fcho_unselect_uri, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "uris", fcho_get_uris, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_current_folder_uri", fcho_set_current_folder_uri, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "current_folder_uri", fcho_get_current_folder_uri, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "preview_filename", fcho_get_preview_filename, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "preview_uri", fcho_get_preview_uri, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "add_filter", fcho_add_filter, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "remove_filter", fcho_remove_filter, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "filters", fcho_list_filters, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "add_shortcut_folder", fcho_add_shortcut_folder, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "remove_shortcut_folder", fcho_remove_shortcut_folder, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "shortcut_folders", fcho_list_shortcut_folders, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "add_shortcut_folder_uri", fcho_add_shortcut_folder_uri, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "remove_shortcut_folder_uri", fcho_remove_shortcut_folder_uri, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "shortcut_folder_uris", fcho_list_shortcut_folder_uris, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GtkFileChooserAction */
    G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_ACTION, "Action", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_FILE_CHOOSER_ACTION, "GTK_FILE_CHOOSER_");

    /* GtkFileChooserError */
    G_DEF_ERROR(GTK_FILE_CHOOSER_ERROR, "FileChooserError", mGtk, rb_eRuntimeError, 
                GTK_TYPE_FILE_CHOOSER_ERROR);

#ifdef GTK_FILE_SYSTEM_ERROR
    /* GtkFileSystemError */
    eFileSystemError = G_DEF_ERROR2(GTK_FILE_SYSTEM_ERROR, "FileSystemError",
				    mGtk, rb_eRuntimeError);
    rb_define_const(eFileSystemError, "NONEXISTENT",
		    INT2NUM(GTK_FILE_SYSTEM_ERROR_NONEXISTENT));
    rb_define_const(eFileSystemError, "NOT_FOLDER",
		    INT2NUM(GTK_FILE_SYSTEM_ERROR_NOT_FOLDER));
    rb_define_const(eFileSystemError, "INVALID_URI",
		    INT2NUM(GTK_FILE_SYSTEM_ERROR_INVALID_URI));
    rb_define_const(eFileSystemError, "BAD_FILENAME",
		    INT2NUM(GTK_FILE_SYSTEM_ERROR_BAD_FILENAME));
    rb_define_const(eFileSystemError, "FAILED",
		    INT2NUM(GTK_FILE_SYSTEM_ERROR_FAILED));
    rb_define_const(eFileSystemError, "ALREADY_EXSITS",
		    INT2NUM(GTK_FILE_SYSTEM_ERROR_ALREADY_EXISTS));
#endif

#if GTK_CHECK_VERSION(2,8,0)
    /* GtkFileChooserConfirmation */
    G_DEF_CLASS(GTK_TYPE_FILE_CHOOSER_CONFIRMATION, "Confirmation", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_FILE_CHOOSER_CONFIRMATION, "GTK_FILE_CHOOSER_");
#endif

    G_DEF_CLASS3("GtkFileChooserEmbed", "FileChooserEmbed", mGtk);

#endif
}
