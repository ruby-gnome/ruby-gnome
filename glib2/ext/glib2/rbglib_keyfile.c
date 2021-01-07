/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE cKeyFile
#define _SELF(self) ((GKeyFile*)(RVAL2BOXED(self, G_TYPE_KEY_FILE)))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, g_key_file_new());
    return Qnil;
}

static VALUE
rg_set_list_separator(VALUE self, VALUE sep)
{
    g_key_file_set_list_separator(_SELF(self), NUM2INT(sep));
    return self;
}

static VALUE
rg_load_from_file(int argc, VALUE *argv, VALUE self)
{
    VALUE file, flags;
    GError* error = NULL;
    gboolean ret;
    GKeyFileFlags gflags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;

    rb_scan_args(argc, argv, "11", &file, &flags);

    if (!NIL_P(flags)){
        gflags = RVAL2GFLAGS(flags, G_TYPE_KEY_FILE_FLAGS);
    }

    ret = g_key_file_load_from_file(_SELF(self), 
                                    RVAL2CSTR(file),
                                    gflags, &error);

    if (! ret) RAISE_GERROR(error);

    return self;
}

static VALUE
rg_load_from_data(int argc, VALUE *argv, VALUE self)
{
    VALUE data, flags;
    GError* error = NULL;
    gboolean ret;
    GKeyFileFlags gflags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;

    rb_scan_args(argc, argv, "11", &data, &flags);

    if (!NIL_P(flags)){
        gflags = RVAL2GFLAGS(flags, G_TYPE_KEY_FILE_FLAGS);
    }
    StringValue(data);

    ret = g_key_file_load_from_data(_SELF(self), 
                                    (const gchar*)RVAL2CSTR(data),
                                    (gsize)RSTRING_LEN(data),
                                    gflags, &error);

    if (! ret) RAISE_GERROR(error);

    return self;
}

static VALUE
rg_load_from_data_dirs(int argc, VALUE *argv, VALUE self)
{
    VALUE file, flags;
    GError* error = NULL;
    gboolean ret;
    gchar* full_path;
    GKeyFileFlags gflags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;

    rb_scan_args(argc, argv, "11", &file, &flags);

    if (!NIL_P(flags)){
        gflags = RVAL2GFLAGS(flags, G_TYPE_KEY_FILE_FLAGS);
    }
    StringValue(file);

    ret = g_key_file_load_from_data_dirs(_SELF(self), 
                                         (const gchar*)RVAL2CSTR(file),
                                         &full_path,
                                         gflags, &error);

    if (! ret) RAISE_GERROR(error);

    return full_path ? CSTR2RVAL(full_path) : Qnil;
}

static VALUE
rg_load_from_dirs(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_file, rb_search_dirs, rb_flags;
    GError* error = NULL;
    gboolean success;
    const gchar *file;
    gchar **search_dirs;
    gchar* full_path;
    GKeyFileFlags flags;

    rb_scan_args(argc, argv, "12", &rb_file, &rb_search_dirs, &rb_flags);

    file = RVAL2CSTR(rb_file);
    search_dirs = RVAL2STRV_ACCEPT_NIL(rb_search_dirs);
    flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;
    if (!NIL_P(rb_flags))
        flags = RVAL2GFLAGS(rb_flags, G_TYPE_KEY_FILE_FLAGS);

    if (search_dirs != NULL)
        success = g_key_file_load_from_dirs(_SELF(self), file,
                                            (const gchar **)search_dirs,
                                            &full_path, flags, &error);
    else
        success = g_key_file_load_from_data_dirs(_SELF(self), file,
                                                 &full_path, flags, &error);

    g_free(search_dirs);

    if (!success)
        RAISE_GERROR(error);

    return CSTR2RVAL(full_path);
}

static VALUE
rg_to_data(VALUE self)
{
    GError* error = NULL;
    gchar* data = g_key_file_to_data(_SELF(self), NULL, &error);

    if (error) RAISE_GERROR(error);

    return CSTR2RVAL_FREE(data);
}

static VALUE
rg_start_group(VALUE self)
{
    return CSTR2RVAL(g_key_file_get_start_group(_SELF(self)));
}

static VALUE
rg_groups(VALUE self)
{
    return STRV2RVAL_FREE(g_key_file_get_groups(_SELF(self), NULL));
}

static VALUE
rg_get_keys(VALUE self, VALUE group_name)
{
    GError *error = NULL;
    gchar **keys = g_key_file_get_keys(_SELF(self),
                                       RVAL2CSTR(group_name),
                                       NULL,
                                       &error);
    if (error != NULL)
        RAISE_GERROR(error);

    return STRV2RVAL_FREE(keys);
}

static VALUE
rg_has_group_p(VALUE self, VALUE group_name)
{
    return CBOOL2RVAL(g_key_file_has_group(_SELF(self), 
                                           (const gchar*)RVAL2CSTR(group_name)));
}

static VALUE
rg_has_key_p(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gboolean ret = g_key_file_has_key(_SELF(self), 
                                      (const gchar*)RVAL2CSTR(group_name),
                                      (const gchar*)RVAL2CSTR(key),
                                      &error);

    if (error) RAISE_GERROR(error);

    return CBOOL2RVAL(ret);
}

static VALUE
rg_get_value(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gchar* ret = g_key_file_get_value(_SELF(self), 
                                      (const gchar*)RVAL2CSTR(group_name),
                                      (const gchar*)RVAL2CSTR(key),
                                      &error);

    if (error) RAISE_GERROR(error);

    return CSTR2RVAL_FREE(ret);
}

static VALUE
rg_get_string(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gchar* ret = g_key_file_get_string(_SELF(self), 
                                       (const gchar*)RVAL2CSTR(group_name),
                                       (const gchar*)RVAL2CSTR(key),
                                       &error);

    if (error) RAISE_GERROR(error);

    return CSTR2RVAL_FREE(ret);
}

static VALUE
rg_get_locale_string(int argc, VALUE *argv, VALUE self)
{
    VALUE group_name, key, locale;
    GError* error = NULL;
    gchar* ret;

    rb_scan_args(argc, argv, "21", &group_name, &key, &locale);

    ret = g_key_file_get_locale_string(_SELF(self), 
                                       (const gchar*)RVAL2CSTR(group_name),
                                       (const gchar*)RVAL2CSTR(key),
                                       (const gchar*)RVAL2CSTR_ACCEPT_NIL(locale),
                                       &error);

    if (error) RAISE_GERROR(error);

    return CSTR2RVAL_FREE(ret);
}

static VALUE
rg_get_boolean(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gboolean ret = g_key_file_get_boolean(_SELF(self), 
                                          (const gchar*)RVAL2CSTR(group_name),
                                          (const gchar*)RVAL2CSTR(key),
                                          &error);

    if (error) RAISE_GERROR(error);

    return CBOOL2RVAL(ret);
}

static VALUE
rg_get_integer(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gint ret = g_key_file_get_integer(_SELF(self), 
                                      (const gchar*)RVAL2CSTR(group_name),
                                      (const gchar*)RVAL2CSTR(key),
                                      &error);

    if (error) RAISE_GERROR(error);

    return INT2NUM(ret);
}

static VALUE
rg_get_double(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gdouble ret = g_key_file_get_double(_SELF(self), 
                                        (const gchar*)RVAL2CSTR(group_name),
                                        (const gchar*)RVAL2CSTR(key),
                                        &error);

    if (error) RAISE_GERROR(error);

    return rb_float_new(ret);
}

static VALUE
rg_get_string_list(VALUE self, VALUE group_name, VALUE key)
{
    VALUE ary;
    gsize i;
    gsize length;
    GError* error = NULL;
    gchar** ret = g_key_file_get_string_list(_SELF(self), 
                                             (const gchar*)RVAL2CSTR(group_name),
                                             (const gchar*)RVAL2CSTR(key),
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
rg_get_locale_string_list(int argc, VALUE *argv, VALUE self)
{
    VALUE group_name, key, locale;
    GError* error = NULL;
    VALUE ary;
    gsize i;
    gsize length;
    gchar** ret;

    rb_scan_args(argc, argv, "21", &group_name, &key, &locale);

    ret = g_key_file_get_locale_string_list(_SELF(self), 
                                            (const gchar*)RVAL2CSTR(group_name),
                                            (const gchar*)RVAL2CSTR(key),
                                            (const gchar*)RVAL2CSTR_ACCEPT_NIL(locale),
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
rg_get_boolean_list(VALUE self, VALUE group_name, VALUE key)
{
    VALUE ary;
    gsize i;
    gsize length;
    GError* error = NULL;
    gboolean* ret = g_key_file_get_boolean_list(_SELF(self), 
                                                (const gchar*)RVAL2CSTR(group_name),
                                                (const gchar*)RVAL2CSTR(key),
                                                &length, &error);

    if (error) RAISE_GERROR(error);

    ary = rb_ary_new();
    for(i = 0; i < length; i++){
        rb_ary_push(ary, CBOOL2RVAL(ret[i]));
    }
    return ary;
}

static VALUE
rg_get_integer_list(VALUE self, VALUE group_name, VALUE key)
{
    VALUE ary;
    gsize i;
    gsize length;
    GError* error = NULL;
    gint* ret = g_key_file_get_integer_list(_SELF(self), 
                                            (const gchar*)RVAL2CSTR(group_name),
                                            (const gchar*)RVAL2CSTR(key),
                                            &length, &error);

    if (error) RAISE_GERROR(error);

    ary = rb_ary_new();
    for(i = 0; i < length; i++){
        rb_ary_push(ary, INT2NUM(ret[i]));
    }
    return ary;
}

static VALUE
rg_get_double_list(VALUE self, VALUE group_name, VALUE key)
{
    VALUE ary;
    gsize i;
    gsize length;
    GError* error = NULL;
    gdouble* ret = g_key_file_get_double_list(_SELF(self), 
                                              (const gchar*)RVAL2CSTR(group_name),
                                              (const gchar*)RVAL2CSTR(key),
                                              &length, &error);

    if (error) RAISE_GERROR(error);

    ary = rb_ary_new();
    for(i = 0; i < length; i++){
        rb_ary_push(ary, rb_float_new(ret[i]));
    }
    return ary;
}

static VALUE
rg_get_comment(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gchar* ret = g_key_file_get_comment(_SELF(self), 
                                        (const gchar*)RVAL2CSTR(group_name),
                                        (const gchar*)RVAL2CSTR(key),
                                        &error);

    if (error) RAISE_GERROR(error);

    return CSTR2RVAL_FREE(ret);
}

static VALUE
rg_set_value(VALUE self, VALUE group_name, VALUE key, VALUE value)
{
    g_key_file_set_value(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                         (const gchar*)RVAL2CSTR(key),
                         (const gchar*)RVAL2CSTR(value));
    return self;
}

static VALUE
rg_set_string(VALUE self, VALUE group_name, VALUE key, VALUE string)
{
    g_key_file_set_string(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                         (const gchar*)RVAL2CSTR(key),
                         (const gchar*)RVAL2CSTR(string));
    return self;
}

static VALUE
rg_set_locale_string(VALUE self, VALUE group_name, VALUE key, VALUE locale, VALUE locale_string)
{
    g_key_file_set_locale_string(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                                 (const gchar*)RVAL2CSTR(key),
                                 (const gchar*)RVAL2CSTR(locale),
                                 (const gchar*)RVAL2CSTR(locale_string));
    return self;
}

static VALUE
rg_set_boolean(VALUE self, VALUE group_name, VALUE key, VALUE value)
{
    g_key_file_set_boolean(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                           (const gchar*)RVAL2CSTR(key),
                           RVAL2CBOOL(value));
    return self;
}

static VALUE
rg_set_integer(VALUE self, VALUE group_name, VALUE key, VALUE value)
{
    g_key_file_set_integer(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                           (const gchar*)RVAL2CSTR(key),
                           NUM2INT(value));
    return self;
}

static VALUE
rg_set_double(VALUE self, VALUE group_name, VALUE key, VALUE value)
{
    g_key_file_set_double(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                          (const gchar*)RVAL2CSTR(key),
                          NUM2DBL(value));
    return self;
}

static VALUE
rg_set_string_list(VALUE self, VALUE rbgroup_name, VALUE rbkey, VALUE rblist)
{
    GKeyFile *key_file = _SELF(self);
    const gchar *group_name = RVAL2CSTR(rbgroup_name);
    const gchar *key = RVAL2CSTR(rbkey);
    long n;
    gchar **list = RVAL2STRS(rblist, n);

    g_key_file_set_string_list(key_file,
                               group_name,
                               key,
                               (const gchar * const *)list,
                               n);

    g_free(list);

    return self;
}

static VALUE
rg_set_locale_string_list(VALUE self, VALUE rbgroup_name, VALUE rbkey, VALUE rblocale, VALUE rblist)
{
    GKeyFile *key_file = _SELF(self);
    const gchar *group_name = RVAL2CSTR(rbgroup_name);
    const gchar *key = RVAL2CSTR(rbkey);
    const gchar *locale = RVAL2CSTR(rblocale);
    long n;
    gchar **list = RVAL2STRS(rblist, n);

    g_key_file_set_locale_string_list(key_file,
                                      group_name,
                                      key,
                                      locale,
                                      (const gchar * const *)list,
                                      n);

    g_free(list);

    return self;
}

static VALUE
rg_set_boolean_list(VALUE self, VALUE rbgroup_name, VALUE rbkey, VALUE rblist)
{
    GKeyFile *key_file = _SELF(self);
    const gchar *group_name = RVAL2CSTR(rbgroup_name);
    const gchar *key = RVAL2CSTR(rbkey);
    long n;
    gboolean *list = RVAL2GBOOLEANS(rblist, n);

    g_key_file_set_boolean_list(key_file, group_name, key, list, n);

    g_free(list);

    return self;
}

static VALUE
rg_set_integer_list(VALUE self, VALUE rbgroup_name, VALUE rbkey, VALUE rblist)
{
    GKeyFile *key_file = _SELF(self);
    const gchar *group_name = RVAL2CSTR(rbgroup_name);
    const gchar *key = RVAL2CSTR(rbkey);
    long n;
    gint *list = RVAL2GINTS(rblist, n);

    g_key_file_set_integer_list(key_file, group_name, key, list, n);

    g_free(list);

    return self;
}

static VALUE
rg_set_double_list(VALUE self, VALUE rbgroup_name, VALUE rbkey, VALUE rblist)
{
    GKeyFile *key_file = _SELF(self);
    const gchar *group_name = RVAL2CSTR(rbgroup_name);
    const gchar *key = RVAL2CSTR(rbkey);
    long n;
    gdouble *list = RVAL2GDOUBLES(rblist, n);

    g_key_file_set_double_list(key_file, group_name, key, list, n);

    return self;
}

static VALUE
rg_set_comment(VALUE self, VALUE group_name, VALUE key, VALUE comment)
{
    GError* error = NULL;

    g_key_file_set_comment(_SELF(self),
                           RVAL2CSTR(group_name),
                           RVAL2CSTR_ACCEPT_NIL(key),
                           RVAL2CSTR(comment),
                           &error);
    if (error != NULL)
        RAISE_GERROR(error);

    return self;
}

static VALUE
rg_remove_group(VALUE self, VALUE group_name)
{
    GError* error = NULL;

    g_key_file_remove_group(_SELF(self), RVAL2CSTR(group_name), &error);

    if (error != NULL)
        RAISE_GERROR(error);

    return self;
}

static VALUE
rg_remove_key(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;

    g_key_file_remove_key(_SELF(self), 
                          RVAL2CSTR(group_name), 
                          RVAL2CSTR(key), 
                          &error);

    if (error != NULL)
        RAISE_GERROR(error);

    return self;
}

static VALUE
rg_remove_comment(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;

    g_key_file_remove_comment(_SELF(self), 
                              RVAL2CSTR(group_name), 
                              RVAL2CSTR(key), 
                              &error);

    if (error != NULL)
        RAISE_GERROR(error);

    return self;
}

void
Init_glib_keyfile(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_KEY_FILE, "KeyFile", mGLib);   
    G_DEF_ERROR(G_KEY_FILE_ERROR, "KeyFileError", mGLib, 
                rb_eRuntimeError, G_TYPE_KEY_FILE_ERROR);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_list_separator, 1);
    RG_DEF_METHOD(load_from_file, -1);
    RG_DEF_METHOD(load_from_data, -1);
    RG_DEF_METHOD(load_from_data_dirs, -1);
    RG_DEF_METHOD(load_from_dirs, -1);
    RG_DEF_METHOD(to_data, 0);
    RG_DEF_METHOD(start_group, 0);
    RG_DEF_METHOD(groups, 0);
    RG_DEF_METHOD(get_keys, 1);
    RG_DEF_METHOD_P(has_group, 1);
    RG_DEF_METHOD_P(has_key, 2);
    RG_DEF_METHOD(get_value, 2);
    RG_DEF_METHOD(get_string, 2);
    RG_DEF_METHOD(get_locale_string, -1);
    RG_DEF_METHOD(get_boolean, 2);
    RG_DEF_METHOD(get_integer, 2);
    RG_DEF_METHOD(get_double, 2);
    RG_DEF_METHOD(get_string_list, 2);
    RG_DEF_METHOD(get_locale_string_list, -1);
    RG_DEF_METHOD(get_boolean_list, 2);
    RG_DEF_METHOD(get_integer_list, 2);
    RG_DEF_METHOD(get_double_list, 2);
    RG_DEF_METHOD(get_comment, 2);
    RG_DEF_METHOD(set_value, 3);
    RG_DEF_METHOD(set_string, 3);
    RG_DEF_METHOD(set_locale_string, 4);
    RG_DEF_METHOD(set_boolean, 3);
    RG_DEF_METHOD(set_integer, 3);
    RG_DEF_METHOD(set_double, 3);
    RG_DEF_METHOD(set_string_list, 3);
    RG_DEF_METHOD(set_locale_string_list, 4);
    RG_DEF_METHOD(set_boolean_list, 3);
    RG_DEF_METHOD(set_integer_list, 3);
    RG_DEF_METHOD(set_double_list, 3);
    RG_DEF_METHOD(set_comment, 3);
    RG_DEF_METHOD(remove_group, 1);
    RG_DEF_METHOD(remove_key, 2);
    RG_DEF_METHOD(remove_comment, 2);

    /* GKeyFileFlags */
    G_DEF_CLASS(G_TYPE_KEY_FILE_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_KEY_FILE_FLAGS, "G_KEY_FILE_");

    /* Defines for handling freedesktop.org Desktop files */
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_GROUP", CSTR2RVAL(G_KEY_FILE_DESKTOP_GROUP));

    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_TYPE",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_TYPE));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_VERSION",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_VERSION));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_NAME",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_NAME));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_GENERIC_NAME",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_GENERIC_NAME));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_NO_DISPLAY",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_NO_DISPLAY));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_COMMENT",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_COMMENT));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_ICON",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_ICON));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_HIDDEN",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_HIDDEN));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_ONLY_SHOW_IN",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_ONLY_SHOW_IN));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_NOT_SHOW_IN",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_NOT_SHOW_IN));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_TRY_EXEC",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_TRY_EXEC));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_EXEC",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_EXEC));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_PATH",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_PATH));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_TERMINAL",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_TERMINAL));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_MIME_TYPE",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_MIME_TYPE));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_CATEGORIES",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_CATEGORIES));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_STARTUP_NOTIFY",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_STARTUP_NOTIFY));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_STARTUP_WM_CLASS",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_STARTUP_WM_CLASS));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_KEY_URL",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_URL));

    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_TYPE_APPLICATION",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_TYPE_APPLICATION));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_TYPE_LINK",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_TYPE_LINK));
    rb_define_const(RG_TARGET_NAMESPACE, "DESKTOP_TYPE_DIRECTORY",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_TYPE_DIRECTORY));
}
