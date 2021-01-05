/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2020  Ruby-GNOME Project Team
 *  Copyright (C) 2006  Masao Mutoh
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

#include "rbgprivate.h"

/************************************************/
static GBookmarkFile*
bookmarkfile_copy(const GBookmarkFile* file)
{
/*
  GBookmarkFile* new_file;
  g_return_val_if_fail (file != NULL, NULL);
  new_file = g_key_file_new();
  *new_file = (GBookmarkFile*)*file;
  return new_file;
*/
    return (GBookmarkFile*)file;
}

static GType
g_bookmark_file_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("GBookmarkFile",
                                                (GBoxedCopyFunc)bookmarkfile_copy,
                                                (GBoxedFreeFunc)g_bookmark_file_free);
    return our_type;
}
/************************************************/

#define G_TYPE_BOOKMARK_FILE (g_bookmark_file_get_type())

#define RG_TARGET_NAMESPACE cBookmarkFile
#define _SELF(self) ((GBookmarkFile*)(RVAL2BOXED(self, G_TYPE_BOOKMARK_FILE)))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, g_bookmark_file_new());
    return Qnil;
}

static VALUE
rg_load_from_file(VALUE self, VALUE rbfilename)
{
    gchar *filename = RVAL2CSTRFILENAME(rbfilename);
    GError* error = NULL;
    gboolean ret = g_bookmark_file_load_from_file(_SELF(self), filename, &error);
    g_free(filename);
    if (!ret)
        RAISE_GERROR(error);

    return self;
}

static VALUE
rg_load_from_data(VALUE self, VALUE data)
{
    GError *error = NULL;

    StringValue(data);
    if (!g_bookmark_file_load_from_data(_SELF(self),
                                        RSTRING_PTR(data),
                                        RSTRING_LEN(data),
                                        &error))
        RAISE_GERROR(error);

    return Qnil;
}

static VALUE
rg_load_from_data_dirs(VALUE self, VALUE file)
{
    GError* error = NULL;
    gboolean ret;
    gchar* full_path;

    ret = g_bookmark_file_load_from_data_dirs(_SELF(self), 
                                              RVAL2CSTR(file),
                                              &full_path, &error);

    if (! ret) RAISE_GERROR(error);

    return full_path ? CSTR2RVAL(full_path) : Qnil;
}

static VALUE
rg_to_data(VALUE self)
{
    GError* error = NULL;
    gchar* data = g_bookmark_file_to_data(_SELF(self), NULL, &error);

    if (error) RAISE_GERROR(error);

    return CSTR2RVAL_FREE(data);
}

static VALUE
rg_to_file(VALUE self, VALUE rbfilename)
{
    gchar *filename = RVAL2CSTRFILENAME(rbfilename);
    GError* error = NULL;
    gboolean ret = g_bookmark_file_to_file(_SELF(self), filename, &error);
    g_free(filename);
    if (!ret)
        RAISE_GERROR(error);

    return self;
}

static VALUE
rg_has_item_p(VALUE self, VALUE uri)
{
    return CBOOL2RVAL(g_bookmark_file_has_item(_SELF(self),
                                               RVAL2CSTR(uri))); 
}

static VALUE
rg_has_group_p(VALUE self, VALUE uri, VALUE group)
{
    GError* error = NULL;
    return CBOOL2RVAL(g_bookmark_file_has_group(_SELF(self),
                                                RVAL2CSTR(uri),
                                                RVAL2CSTR(group),
                                                &error));
}

static VALUE
rg_has_application_p(VALUE self, VALUE uri, VALUE name)
{
    GError* error = NULL;
    return CBOOL2RVAL(g_bookmark_file_has_application(_SELF(self),
                                                      RVAL2CSTR(uri),
                                                      RVAL2CSTR(name),
                                                      &error));
}

static VALUE
rg_size(VALUE self)
{
    return INT2NUM(g_bookmark_file_get_size(_SELF(self)));
}

static VALUE
rg_uris(VALUE self)
{
    return STRV2RVAL_FREE(g_bookmark_file_get_uris(_SELF(self), NULL));
}

static VALUE
rg_get_title(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gchar* ret = g_bookmark_file_get_title(_SELF(self),
                                           RVAL2CSTR(uri),
                                           &error);
    if (error) RAISE_GERROR(error);
    return CSTR2RVAL_FREE(ret);
}

static VALUE
rg_get_description(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gchar* ret = g_bookmark_file_get_description(_SELF(self),
                                                 RVAL2CSTR(uri),
                                                 &error);
    if (error) RAISE_GERROR(error);
    return CSTR2RVAL_FREE(ret);
}

static VALUE
rg_get_mime_type(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gchar* ret = g_bookmark_file_get_mime_type(_SELF(self),
                                               RVAL2CSTR(uri),
                                               &error);
    if (error) RAISE_GERROR(error);
    return CSTR2RVAL_FREE(ret);
}

static VALUE
rg_private_p(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gboolean ret = g_bookmark_file_get_is_private(_SELF(self),
                                                  RVAL2CSTR(uri),
                                                  &error);
    if (error) RAISE_GERROR(error);
    return CBOOL2RVAL(ret);
}

static VALUE
rg_get_icon(VALUE self, VALUE uri)
{
    gchar* href;
    gchar* mime_type;
    GError *error = NULL;
    gboolean ret = g_bookmark_file_get_icon(_SELF(self),
                                            RVAL2CSTR(uri),
                                            &href, &mime_type,
                                            &error);
    if (!ret){
        if (error) RAISE_GERROR(error);
        return Qnil;
    }
    return rb_assoc_new(CSTR2RVAL_FREE(href), CSTR2RVAL_FREE(mime_type)); 
}

static VALUE
rg_get_added(VALUE self, VALUE uri)
{
    GError *error = NULL;
    time_t ret = g_bookmark_file_get_added(_SELF(self),
                                           RVAL2CSTR(uri),
                                           &error);
    if (!ret) RAISE_GERROR(error);

    return rb_time_new(ret, 0);
}

static VALUE
rg_get_modified(VALUE self, VALUE uri)
{
    GError *error = NULL;
    time_t ret = g_bookmark_file_get_modified(_SELF(self),
                                              RVAL2CSTR(uri),
                                              &error);
    if (!ret) RAISE_GERROR(error);

    return rb_time_new(ret, 0);
}

static VALUE
rg_get_visited(VALUE self, VALUE uri)
{
    GError *error = NULL;
    time_t ret = g_bookmark_file_get_visited(_SELF(self),
                                             RVAL2CSTR(uri),
                                             &error);
    if (!ret) RAISE_GERROR(error);

    return rb_time_new(ret, 0);
}

static VALUE
rg_get_groups(VALUE self, VALUE uri)
{
    gsize length;
    VALUE ary;
    gsize i;
    GError* error = NULL;
    gchar** ret = g_bookmark_file_get_groups(_SELF(self),
                                             RVAL2CSTR(uri),
                                             &length, &error);
    if (error) RAISE_GERROR(error);

    ary = rb_ary_new();
    for(i = 0; i < length; i++){
        rb_ary_push(ary, CSTR2RVAL(ret[i]));
    }

    g_strfreev(ret);
    return ary;
}

static VALUE
rg_get_applications(VALUE self, VALUE uri)
{
    gsize length;
    VALUE ary;
    gsize i;
    GError* error = NULL;
    gchar** ret = g_bookmark_file_get_applications(_SELF(self),
                                                   RVAL2CSTR(uri),
                                                   &length, &error);
    if (error) RAISE_GERROR(error);

    ary = rb_ary_new();
    for(i = 0; i < length; i++){
        rb_ary_push(ary, CSTR2RVAL(ret[i]));
    }

    g_strfreev(ret);
    return ary;
}

static VALUE
rg_get_app_info(VALUE self, VALUE uri, VALUE name)
{
    gchar* exec;
    guint count;
    time_t stamp;
    GError* error = NULL;

    gboolean ret = g_bookmark_file_get_app_info(_SELF(self),
                                                RVAL2CSTR(uri),
                                                RVAL2CSTR(name),
                                                &exec, &count, &stamp, &error);
    if (!ret) RAISE_GERROR(error);

    return rb_ary_new3(3, CSTR2RVAL(exec), UINT2NUM(count), rb_time_new(stamp, 0));
}

static VALUE
rg_set_title(VALUE self, VALUE uri, VALUE title)
{
    g_bookmark_file_set_title(_SELF(self),
                              RVAL2CSTR(uri),
                              RVAL2CSTR(title));
    return self;
}

static VALUE
rg_set_description(VALUE self, VALUE uri, VALUE description)
{
    g_bookmark_file_set_description(_SELF(self),
                                    RVAL2CSTR(uri),
                                    RVAL2CSTR(description));
    return self;
}

static VALUE
rg_set_mime_type(VALUE self, VALUE uri, VALUE mime_type)
{
    g_bookmark_file_set_mime_type(_SELF(self),
                                  RVAL2CSTR(uri),
                                  RVAL2CSTR(mime_type));
    return self;
}

static VALUE
rg_set_private(VALUE self, VALUE uri, VALUE is_private)
{
    g_bookmark_file_set_is_private(_SELF(self),
                                   RVAL2CSTR(uri),
                                   RVAL2CBOOL(is_private));
    return self;
}

static VALUE
rg_set_icon(VALUE self, VALUE uri, VALUE href, VALUE mime_type)
{
    g_bookmark_file_set_icon(_SELF(self),
                             RVAL2CSTR(uri),
                             RVAL2CSTR(href),
                             RVAL2CSTR(mime_type));
    return self;
}

static VALUE
rg_set_added(VALUE self, VALUE uri, VALUE time)
{
    g_bookmark_file_set_added(_SELF(self), 
                              RVAL2CSTR(uri),
                              (time_t)NUM2LONG(rb_Integer(time)));
    return self;
}

static VALUE
rg_set_groups(VALUE self, VALUE rburi, VALUE rbgroups)
{
    GBookmarkFile *bookmark = _SELF(self);
    const gchar *uri = RVAL2CSTR(rburi);
    long n;
    gchar **groups = RVAL2STRS(rbgroups, n);

    g_bookmark_file_set_groups(bookmark, uri, (const gchar **)groups, n);

    g_free(groups);

    return self;
}

static VALUE
rg_set_modified(VALUE self, VALUE uri, VALUE time)
{
    g_bookmark_file_set_modified(_SELF(self), 
                                 RVAL2CSTR(uri),
                                 (time_t)NUM2LONG(rb_Integer(time)));
    return self;
}

static VALUE
rg_set_visited(VALUE self, VALUE uri, VALUE time)
{
    g_bookmark_file_set_visited(_SELF(self), 
                                RVAL2CSTR(uri),
                                (time_t)NUM2LONG(rb_Integer(time)));
    return self;
}

static VALUE
rg_set_app_info(VALUE self, VALUE uri, VALUE name, VALUE exec, VALUE count, VALUE stamp)
{
    GError* error = NULL;
    gboolean ret = g_bookmark_file_set_app_info(_SELF(self),
                                                RVAL2CSTR(uri),
                                                RVAL2CSTR(name),
                                                RVAL2CSTR(exec),
                                                NUM2INT(count),
                                                (time_t)NUM2LONG(rb_Integer(stamp)),
                                                &error);

    if (! ret) RAISE_GERROR(error);

    return self;
}

static VALUE 
rg_add_group(VALUE self, VALUE uri, VALUE group)
{
    g_bookmark_file_add_group(_SELF(self),
                              RVAL2CSTR(uri),
                              RVAL2CSTR(group));
    return self;
}

static VALUE
rg_add_application(VALUE self, VALUE uri, VALUE name, VALUE exec)
{
    g_bookmark_file_add_application(_SELF(self),
                                    RVAL2CSTR(uri),
                                    RVAL2CSTR(name),
                                    RVAL2CSTR(exec));
    return self;
}

static VALUE
rg_remove_group(VALUE self, VALUE uri, VALUE group)
{
    GError* error = NULL;
    gboolean ret = g_bookmark_file_remove_group(_SELF(self),
                                                RVAL2CSTR(uri),
                                                RVAL2CSTR(group),
                                                &error);
    if (! ret) RAISE_GERROR(error);

    return self;
}

static VALUE
rg_remove_application(VALUE self, VALUE uri, VALUE name)
{
    GError *error = NULL;
    gboolean ret = g_bookmark_file_remove_application(_SELF(self),
                                                      RVAL2CSTR(uri),
                                                      RVAL2CSTR(name),
                                                      &error);
    if (! ret) RAISE_GERROR(error);

    return self;
}

static VALUE
rg_remove_item(VALUE self, VALUE uri)
{
    GError *error = NULL;
    gboolean ret = g_bookmark_file_remove_item(_SELF(self),
                                               RVAL2CSTR(uri),
                                               &error);
    if (! ret) RAISE_GERROR(error);

    return self;
}

static VALUE
rg_move_item(VALUE self, VALUE old_uri, VALUE new_uri)
{
    GError *error = NULL;
    gboolean ret = g_bookmark_file_move_item(_SELF(self),
                                             RVAL2CSTR(old_uri),
                                             RVAL2CSTR(new_uri),
                                             &error);
    if (! ret) RAISE_GERROR(error);

    return self;
}

void
Init_glib_bookmark_file(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_BOOKMARK_FILE, "BookmarkFile", mGLib);

    G_DEF_ERROR(G_BOOKMARK_FILE_ERROR, "BookmarkFileError", mGLib, 
                rb_eRuntimeError, G_TYPE_BOOKMARK_FILE_ERROR);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(load_from_file, 1);
    RG_DEF_METHOD(load_from_data, 1);
    RG_DEF_METHOD(load_from_data_dirs, 1);
    RG_DEF_METHOD(to_data, 0);
    RG_DEF_METHOD(to_file, 1);
    RG_DEF_METHOD_P(has_item, 1);
    RG_DEF_METHOD_P(has_group, 2);
    RG_DEF_METHOD_P(has_application, 2);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(uris, 0);
    RG_DEF_METHOD(get_title, 1);
    RG_DEF_METHOD(get_description, 1);
    RG_DEF_METHOD(get_mime_type, 1);
    RG_DEF_METHOD_P(private, 1);
    RG_DEF_METHOD(get_icon, 1);
    RG_DEF_METHOD(get_added, 1);
    RG_DEF_METHOD(get_modified, 1);
    RG_DEF_METHOD(get_visited, 1);
    RG_DEF_METHOD(get_groups, 1);
    RG_DEF_METHOD(get_applications, 1);
    RG_DEF_METHOD(get_app_info, 2);
    RG_DEF_METHOD(set_title, 2);
    RG_DEF_METHOD(set_description, 2);
    RG_DEF_METHOD(set_mime_type, 2);
    RG_DEF_METHOD(set_private, 2);
    RG_DEF_METHOD(set_icon, 3);
    RG_DEF_METHOD(set_added, 2);
    RG_DEF_METHOD(set_groups, 2);
    RG_DEF_METHOD(set_modified, 2);
    RG_DEF_METHOD(set_visited, 2);
    RG_DEF_METHOD(set_app_info, 5);
    RG_DEF_METHOD(add_group, 2);
    RG_DEF_METHOD(add_application, 3);
    RG_DEF_METHOD(remove_group, 2);
    RG_DEF_METHOD(remove_application, 2);
    RG_DEF_METHOD(remove_item, 1);
    RG_DEF_METHOD(move_item, 2);
}
