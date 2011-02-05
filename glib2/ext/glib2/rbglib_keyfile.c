/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_keyfile.c -

  $Author: sakai $
  $Date: 2007/06/16 02:46:28 $

  Copyright (C) 2006 Masao Mutoh
************************************************/

#include "rbgprivate.h"

#if GLIB_CHECK_VERSION(2,6,0)
/************************************************/
static GKeyFile*
keyfile_copy(const GKeyFile* keyfile)
{
//  GKeyFile* new_keyfile;
  g_return_val_if_fail (keyfile != NULL, NULL);
/*
  new_keyfile = g_key_file_new();
  *new_keyfile = (GKeyFile*)*keyfile;
  return new_keyfile;
*/
  return (GKeyFile*)keyfile;
}


GType
g_key_file_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static("GKeyFile",
                                            (GBoxedCopyFunc)keyfile_copy,
                                            (GBoxedFreeFunc)g_key_file_free);
  return our_type;
}
/************************************************/

#define _SELF(self) ((GKeyFile*)(RVAL2BOXED(self, G_TYPE_KEY_FILE)))

static VALUE
keyfile_initialize(VALUE self)
{
    G_INITIALIZE(self, g_key_file_new());
    return Qnil;
}

static VALUE
keyfile_set_list_separator(VALUE self, VALUE sep)
{
    g_key_file_set_list_separator(_SELF(self), NUM2INT(sep));
    return self;
}

static VALUE
keyfile_load_from_file(int argc, VALUE *argv, VALUE self)
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
                                    (const gchar*)RVAL2CSTR(file),
                                    gflags, &error);

    if (! ret) RAISE_GERROR(error);

    return self;
}

static VALUE
keyfile_load_from_data(int argc, VALUE *argv, VALUE self)
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
keyfile_load_from_data_dirs(int argc, VALUE *argv, VALUE self)
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

#if GLIB_CHECK_VERSION(2, 14, 0)
static VALUE
keyfile_load_from_dirs(int argc, VALUE *argv, VALUE self)
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
    if (NIL_P(rb_search_dirs)) {
	search_dirs = NULL;
    }
    else {
	long i, len;

	Check_Type(rb_search_dirs, T_ARRAY);
	len = RARRAY_LEN(rb_search_dirs);
	search_dirs = ALLOCA_N(gchar *, len + 1);
	for (i = 0; i < len; i++) {
	    search_dirs[i] = RVAL2CSTR(RARRAY_PTR(rb_search_dirs)[i]);
	}
	search_dirs[i] = NULL;
    }
    flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;
    if (!NIL_P(rb_flags))
        flags = RVAL2GFLAGS(rb_flags, G_TYPE_KEY_FILE_FLAGS);

    if (search_dirs)
	success = g_key_file_load_from_dirs(_SELF(self), file,
					    (const gchar **)search_dirs,
					    &full_path, flags, &error);
    else
	success = g_key_file_load_from_data_dirs(_SELF(self), file,
						 &full_path, flags, &error);

    if (!success)
	RAISE_GERROR(error);

    return CSTR2RVAL(full_path);
}
#endif

static VALUE
keyfile_to_data(VALUE self)
{
    GError* error = NULL;
    gchar* data = g_key_file_to_data(_SELF(self), NULL, &error);

    if (error) RAISE_GERROR(error);

    return CSTR2RVAL_FREE(data);
}

static VALUE
keyfile_get_start_group(VALUE self)
{
    return CSTR2RVAL(g_key_file_get_start_group(_SELF(self)));
}

static VALUE
keyfile_get_groups(VALUE self)
{
    gsize length;
    int i;
    gchar** groups = g_key_file_get_groups(_SELF(self), &length);
    VALUE ary = rb_ary_new();

    for (i = 0; i < length; i++){
        rb_ary_push(ary, CSTR2RVAL(groups[i]));
    }

    g_strfreev(groups);

    return ary;
}

static VALUE
keyfile_get_keys(VALUE self, VALUE group_name)
{
    gsize length;
    int i;
    GError* error = NULL;
    gchar** keys = g_key_file_get_keys(_SELF(self), 
                                       (const gchar*)RVAL2CSTR(group_name),
                                       &length, &error);
    VALUE ary = rb_ary_new();

    if (error) RAISE_GERROR(error);

    for (i = 0; i < length; i++){
        rb_ary_push(ary, CSTR2RVAL(keys[i]));
    }

    g_strfreev(keys);

    return ary;
}

static VALUE
keyfile_has_group(VALUE self, VALUE group_name)
{
    return CBOOL2RVAL(g_key_file_has_group(_SELF(self), 
                                           (const gchar*)RVAL2CSTR(group_name)));
}

static VALUE
keyfile_has_key(VALUE self, VALUE group_name, VALUE key)
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
keyfile_get_value(VALUE self, VALUE group_name, VALUE key)
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
keyfile_get_string(VALUE self, VALUE group_name, VALUE key)
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
keyfile_get_locale_string(int argc, VALUE *argv, VALUE self)
{
    VALUE group_name, key, locale;
    GError* error = NULL;
    gchar* ret;

    rb_scan_args(argc, argv, "21", &group_name, &key, &locale);

    ret = g_key_file_get_locale_string(_SELF(self), 
                                       (const gchar*)RVAL2CSTR(group_name),
                                       (const gchar*)RVAL2CSTR(key),
                                       (const gchar*)(NIL_P(locale) ? NULL : RVAL2CSTR(locale)),
                                       &error);

    if (error) RAISE_GERROR(error);

    return CSTR2RVAL_FREE(ret);
}

static VALUE
keyfile_get_boolean(VALUE self, VALUE group_name, VALUE key)
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
keyfile_get_integer(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gint ret = g_key_file_get_integer(_SELF(self), 
                                      (const gchar*)RVAL2CSTR(group_name),
                                      (const gchar*)RVAL2CSTR(key),
                                      &error);

    if (error) RAISE_GERROR(error);

    return INT2NUM(ret);
}

#if GLIB_CHECK_VERSION(2,12,0)
static VALUE
keyfile_get_double(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    gdouble ret = g_key_file_get_double(_SELF(self), 
                                        (const gchar*)RVAL2CSTR(group_name),
                                        (const gchar*)RVAL2CSTR(key),
                                        &error);

    if (error) RAISE_GERROR(error);

    return rb_float_new(ret);
}
#endif

static VALUE
keyfile_get_string_list(VALUE self, VALUE group_name, VALUE key)
{
    VALUE ary;
    gint i;
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
keyfile_get_locale_string_list(int argc, VALUE *argv, VALUE self)
{
    VALUE group_name, key, locale;
    GError* error = NULL;
    VALUE ary;
    gint i;
    gsize length;
    gchar** ret;

    rb_scan_args(argc, argv, "21", &group_name, &key, &locale);

    ret = g_key_file_get_locale_string_list(_SELF(self), 
                                            (const gchar*)RVAL2CSTR(group_name),
                                            (const gchar*)RVAL2CSTR(key),
                                            (const gchar*)(NIL_P(locale) ? NULL : RVAL2CSTR(locale)),
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
keyfile_get_boolean_list(VALUE self, VALUE group_name, VALUE key)
{
    VALUE ary;
    gint i;
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
keyfile_get_integer_list(VALUE self, VALUE group_name, VALUE key)
{
    VALUE ary;
    gint i;
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

#if GLIB_CHECK_VERSION(2,12,0)
static VALUE
keyfile_get_double_list(VALUE self, VALUE group_name, VALUE key)
{
    VALUE ary;
    gint i;
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
#endif

static VALUE
keyfile_get_comment(VALUE self, VALUE group_name, VALUE key)
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
keyfile_set_value(VALUE self, VALUE group_name, VALUE key, VALUE value)
{
    g_key_file_set_value(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                         (const gchar*)RVAL2CSTR(key),
                         (const gchar*)RVAL2CSTR(value));
    return self;
}

static VALUE
keyfile_set_string(VALUE self, VALUE group_name, VALUE key, VALUE string)
{
    g_key_file_set_string(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                         (const gchar*)RVAL2CSTR(key),
                         (const gchar*)RVAL2CSTR(string));
    return self;
}

static VALUE
keyfile_set_locale_string(VALUE self, VALUE group_name, VALUE key, VALUE locale, VALUE locale_string)
{
    g_key_file_set_locale_string(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                                 (const gchar*)RVAL2CSTR(key),
                                 (const gchar*)RVAL2CSTR(locale),
                                 (const gchar*)RVAL2CSTR(locale_string));
    return self;
}

static VALUE
keyfile_set_boolean(VALUE self, VALUE group_name, VALUE key, VALUE value)
{
    g_key_file_set_boolean(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                           (const gchar*)RVAL2CSTR(key),
                           RVAL2CBOOL(value));
    return self;
}

static VALUE
keyfile_set_integer(VALUE self, VALUE group_name, VALUE key, VALUE value)
{
    g_key_file_set_integer(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                           (const gchar*)RVAL2CSTR(key),
                           NUM2INT(value));
    return self;
}

#if GLIB_CHECK_VERSION(2,12,0)
static VALUE
keyfile_set_double(VALUE self, VALUE group_name, VALUE key, VALUE value)
{
    g_key_file_set_double(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                          (const gchar*)RVAL2CSTR(key),
                          NUM2DBL(value));
    return self;
}
#endif

static VALUE
keyfile_set_string_list(VALUE self, VALUE group_name, VALUE key, VALUE list)
{
    gint len = RARRAY_LEN(list);
    gchar** glist = ALLOCA_N(gchar*, len);
    gint i;

    for (i = 0; i < len; i++){
        glist[i] = RVAL2CSTR(RARRAY_PTR(list)[i]);
    }

    g_key_file_set_string_list(_SELF(self), 
                               (const gchar*)RVAL2CSTR(group_name),
                               (const gchar*)RVAL2CSTR(key),
                               (const gchar**)glist, len);
    return self;
}

static VALUE
keyfile_set_locale_string_list(VALUE self, VALUE group_name, VALUE key, VALUE locale, VALUE list)
{
    gint len = RARRAY_LEN(list);
    gchar** glist = ALLOCA_N(gchar*, len);
    gint i;

    for (i = 0; i < len; i++){
        glist[i] = RVAL2CSTR(RARRAY_PTR(list)[i]);
    }

    g_key_file_set_locale_string_list(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                                      (const gchar*)RVAL2CSTR(key),
                                      (const gchar*)RVAL2CSTR(locale),
                                      (const gchar**)glist, len);

    return self;
}

static VALUE
keyfile_set_boolean_list(VALUE self, VALUE group_name, VALUE key, VALUE list)
{
    gint len = RARRAY_LEN(list);
    gboolean* glist = ALLOCA_N(gboolean, len);
    gint i;

    for (i = 0; i < len; i++){
        glist[i] = RVAL2CBOOL(RARRAY_PTR(list)[i]);
    }

    g_key_file_set_boolean_list(_SELF(self),
                                (const gchar*)RVAL2CSTR(group_name),
                                (const gchar*)RVAL2CSTR(key),
                                glist, len);
    return self;
}

static VALUE
keyfile_set_integer_list(VALUE self, VALUE group_name, VALUE key, VALUE list)
{
    gint len = RARRAY_LEN(list);
    gint* glist = ALLOCA_N(gint, len);
    gint i;

    for (i = 0; i < len; i++){
        glist[i] = RVAL2CBOOL(RARRAY_PTR(list)[i]);
    }

    g_key_file_set_integer_list(_SELF(self),
                                (const gchar*)RVAL2CSTR(group_name),
                                (const gchar*)RVAL2CSTR(key),
                                glist, len);
    return self;
}

#if GLIB_CHECK_VERSION(2,12,0)
static VALUE
keyfile_set_double_list(VALUE self, VALUE group_name, VALUE key, VALUE list)
{
    gint len = RARRAY_LEN(list);
    gdouble* glist = ALLOCA_N(gdouble, len);
    gint i;

    for (i = 0; i < len; i++){
        glist[i] = RVAL2CBOOL(RARRAY_PTR(list)[i]);
    }

    g_key_file_set_double_list(_SELF(self),
                                (const gchar*)RVAL2CSTR(group_name),
                                (const gchar*)RVAL2CSTR(key),
                               glist, len);
    return self;
}
#endif

static VALUE
keyfile_set_comment(VALUE self, VALUE group_name, VALUE key, VALUE comment)
{
    GError* error = NULL;
    g_key_file_set_comment(_SELF(self), (const gchar*)RVAL2CSTR(group_name),
                           (const gchar*)(NIL_P(key) ? NULL : RVAL2CSTR(key)),
                           (const gchar*)RVAL2CSTR(comment), &error);

    if (error) RAISE_GERROR(error);

    return self;
}

static VALUE
keyfile_remove_group(VALUE self, VALUE group_name)
{
    GError* error = NULL;
    g_key_file_remove_group(_SELF(self), (const gchar*)RVAL2CSTR(group_name), &error);

    if (error) RAISE_GERROR(error);

    return self;
}

static VALUE
keyfile_remove_key(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    g_key_file_remove_key(_SELF(self), 
                          (const gchar*)RVAL2CSTR(group_name), 
                          (const gchar*)RVAL2CSTR(key), 
                          &error);

    if (error) RAISE_GERROR(error);

    return self;
}


static VALUE
keyfile_remove_comment(VALUE self, VALUE group_name, VALUE key)
{
    GError* error = NULL;
    g_key_file_remove_comment(_SELF(self), 
                          (const gchar*)RVAL2CSTR(group_name), 
                          (const gchar*)RVAL2CSTR(key), 
                          &error);

    if (error) RAISE_GERROR(error);

    return self;
}
#endif

void
Init_glib_keyfile()
{
#if GLIB_CHECK_VERSION(2,6,0)
    VALUE kf = G_DEF_CLASS(G_TYPE_KEY_FILE, "KeyFile", mGLib);   
    G_DEF_ERROR(G_KEY_FILE_ERROR, "KeyFileError", mGLib, 
                rb_eRuntimeError, G_TYPE_KEY_FILE_ERROR);

    rb_define_method(kf, "initialize", keyfile_initialize, 0);
    rb_define_method(kf, "set_list_separator", keyfile_set_list_separator, 1);
    rb_define_method(kf, "load_from_file", keyfile_load_from_file, -1);
    rb_define_method(kf, "load_from_data", keyfile_load_from_data, -1);
    rb_define_method(kf, "load_from_data_dirs", keyfile_load_from_data_dirs, -1);
#if GLIB_CHECK_VERSION(2, 14, 0)
    rb_define_method(kf, "load_from_dirs", keyfile_load_from_dirs, -1);
#endif
    rb_define_method(kf, "to_data", keyfile_to_data, 0);
    rb_define_method(kf, "start_group", keyfile_get_start_group, 0);
    rb_define_method(kf, "groups", keyfile_get_groups, 0);
    rb_define_method(kf, "get_keys", keyfile_get_keys, 1);
    rb_define_method(kf, "has_group?", keyfile_has_group, 1);
    rb_define_method(kf, "has_key?", keyfile_has_key, 2);
    rb_define_method(kf, "get_value", keyfile_get_value, 2);
    rb_define_method(kf, "get_string", keyfile_get_string, 2);
    rb_define_method(kf, "get_locale_string", keyfile_get_locale_string, -1);
    rb_define_method(kf, "get_boolean", keyfile_get_boolean, 2);
    rb_define_method(kf, "get_integer", keyfile_get_integer, 2);
#if GLIB_CHECK_VERSION(2,12,0)
    rb_define_method(kf, "get_double", keyfile_get_double, 2);
#endif
    rb_define_method(kf, "get_string_list", keyfile_get_string_list, 2);
    rb_define_method(kf, "get_locale_string_list", keyfile_get_locale_string_list, -1);
    rb_define_method(kf, "get_boolean_list", keyfile_get_boolean_list, 2);
    rb_define_method(kf, "get_integer_list", keyfile_get_integer_list, 2);
#if GLIB_CHECK_VERSION(2,12,0)
    rb_define_method(kf, "get_double_list", keyfile_get_double_list, 2);
#endif
    rb_define_method(kf, "get_comment", keyfile_get_comment, 2);
    rb_define_method(kf, "set_value", keyfile_set_value, 3);
    rb_define_method(kf, "set_string", keyfile_set_string, 3);
    rb_define_method(kf, "set_locale_string", keyfile_set_locale_string, 4);
    rb_define_method(kf, "set_boolean", keyfile_set_boolean, 3);
    rb_define_method(kf, "set_integer", keyfile_set_integer, 3);
#if GLIB_CHECK_VERSION(2,12,0)
    rb_define_method(kf, "set_double", keyfile_set_double, 3);
#endif
    rb_define_method(kf, "set_string_list", keyfile_set_string_list, 3);
    rb_define_method(kf, "set_locale_string_list", keyfile_set_locale_string_list, 4);
    rb_define_method(kf, "set_boolean_list", keyfile_set_boolean_list, 3);
    rb_define_method(kf, "set_integer_list", keyfile_set_integer_list, 3);
#if GLIB_CHECK_VERSION(2,12,0)
    rb_define_method(kf, "set_double_list", keyfile_set_double_list, 3);
#endif
    rb_define_method(kf, "set_comment", keyfile_set_comment, 3);
    rb_define_method(kf, "remove_group", keyfile_remove_group, 1);
    rb_define_method(kf, "remove_key", keyfile_remove_key, 2);
    rb_define_method(kf, "remove_comment", keyfile_remove_comment, 2);

    /* GKeyFileFlags */
    G_DEF_CLASS(G_TYPE_KEY_FILE_FLAGS, "Flags", kf);
    G_DEF_CONSTANTS(kf, G_TYPE_KEY_FILE_FLAGS, "G_KEY_FILE_");

#if GLIB_CHECK_VERSION(2, 14, 0)
    /* Defines for handling freedesktop.org Desktop files */
    rb_define_const(kf, "DESKTOP_GROUP", CSTR2RVAL(G_KEY_FILE_DESKTOP_GROUP));

    rb_define_const(kf, "DESKTOP_KEY_TYPE",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_TYPE));
    rb_define_const(kf, "DESKTOP_KEY_VERSION",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_VERSION));
    rb_define_const(kf, "DESKTOP_KEY_NAME",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_NAME));
    rb_define_const(kf, "DESKTOP_KEY_GENERIC_NAME",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_GENERIC_NAME));
    rb_define_const(kf, "DESKTOP_KEY_NO_DISPLAY",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_NO_DISPLAY));
    rb_define_const(kf, "DESKTOP_KEY_COMMENT",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_COMMENT));
    rb_define_const(kf, "DESKTOP_KEY_ICON",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_ICON));
    rb_define_const(kf, "DESKTOP_KEY_HIDDEN",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_HIDDEN));
    rb_define_const(kf, "DESKTOP_KEY_ONLY_SHOW_IN",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_ONLY_SHOW_IN));
    rb_define_const(kf, "DESKTOP_KEY_NOT_SHOW_IN",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_NOT_SHOW_IN));
    rb_define_const(kf, "DESKTOP_KEY_TRY_EXEC",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_TRY_EXEC));
    rb_define_const(kf, "DESKTOP_KEY_EXEC",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_EXEC));
    rb_define_const(kf, "DESKTOP_KEY_PATH",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_PATH));
    rb_define_const(kf, "DESKTOP_KEY_TERMINAL",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_TERMINAL));
    rb_define_const(kf, "DESKTOP_KEY_MIME_TYPE",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_MIME_TYPE));
    rb_define_const(kf, "DESKTOP_KEY_CATEGORIES",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_CATEGORIES));
    rb_define_const(kf, "DESKTOP_KEY_STARTUP_NOTIFY",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_STARTUP_NOTIFY));
    rb_define_const(kf, "DESKTOP_KEY_STARTUP_WM_CLASS",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_STARTUP_WM_CLASS));
    rb_define_const(kf, "DESKTOP_KEY_URL",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_KEY_URL));

    rb_define_const(kf, "DESKTOP_TYPE_APPLICATION",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_TYPE_APPLICATION));
    rb_define_const(kf, "DESKTOP_TYPE_LINK",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_TYPE_LINK));
    rb_define_const(kf, "DESKTOP_TYPE_DIRECTORY",
                    CSTR2RVAL(G_KEY_FILE_DESKTOP_TYPE_DIRECTORY));
#endif
#endif
}
