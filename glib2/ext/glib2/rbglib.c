/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2019  Ruby-GNOME Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
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

#include <locale.h>
#include "rbgprivate.h"
#include "rbglib.h"

#define RG_TARGET_NAMESPACE mGLib

static ID id_inspect;

VALUE RG_TARGET_NAMESPACE;

const gchar *
rbg_rval2cstr(VALUE *str)
{
    StringValue(*str);

    if (rb_enc_get(*str) != rb_utf8_encoding())
        *str = rb_str_export_to_enc(*str, rb_utf8_encoding());

    return StringValueCStr(*str);
}

const gchar *
rbg_rval2cstr_raw(VALUE *str)
{
    StringValue(*str);

    return StringValueCStr(*str);
}

const gchar *
rbg_rval2cstr_ptr(VALUE *str)
{
    StringValue(*str);

    return RSTRING_PTR(*str);
}

const gchar *
rbg_rval_inspect(VALUE object)
{
    VALUE inspected = rb_funcall(object, id_inspect, 0);

    return RVAL2CSTR(inspected);
}

char *
rbg_string_value_ptr(volatile VALUE *ptr)
{
    return rb_string_value_ptr(ptr);
}

const gchar *
rbg_rval2cstr_accept_nil(VALUE *str)
{
    return NIL_P(*str) ? NULL : RVAL2CSTR(*str);
}

const gchar *
rbg_rval2cstr_raw_accept_nil(VALUE *str)
{
    return NIL_P(*str) ? NULL : RVAL2CSTR_RAW(*str);
}

const gchar *
rbg_rval2cstr_ptr_accept_nil(VALUE *str)
{
    return NIL_P(*str) ? NULL : RVAL2CSTR_PTR(*str);
}

/* TODO: How do we deal with encodings? */
const gchar *
rbg_rval2cstr_accept_symbol(volatile VALUE *value)
{
    if (!SYMBOL_P(*value))
        return rbg_rval2cstr((VALUE *)value);

    return rb_id2name(SYM2ID(*value));
}

const gchar *
rbg_rval2cstr_accept_symbol_accept_nil(volatile VALUE *value)
{
    return NIL_P(*value) ? NULL : rbg_rval2cstr_accept_symbol(value);
}

const gchar *
rbg_rval2glibid(volatile VALUE *value, volatile VALUE *buf, gboolean accept_nil)
{
    gchar *id, *p;

    if (accept_nil && NIL_P(*value))
        return NULL;

    if (SYMBOL_P(*value)) {
        *buf = rb_String(*value);
    } else {
        StringValue(*value);
        *buf = rb_str_dup(*value);
    }
    RB_GC_GUARD(*buf);

    id = RSTRING_PTR(*buf);
    for (p = id; *p; p++)
        if (*p == '_')
            *p = '-';

    return id;
}

VALUE
rbg_cstr2rval(const gchar *str)
{
    return str != NULL ? CSTR2RVAL_LEN(str, strlen(str)) : Qnil;
}

VALUE
rbg_cstr2rval_len(const gchar *str, gsize len)
{
    if (str == NULL)
        return Qnil;

    return rb_external_str_new_with_enc(str, len, rb_utf8_encoding());
}

struct rbg_cstr2rval_len_free_args {
    gchar *str;
    gsize len;
};

static VALUE
rbg_cstr2rval_len_free_body(VALUE value)
{
    struct rbg_cstr2rval_len_free_args *args = (struct rbg_cstr2rval_len_free_args *)value;

    return CSTR2RVAL_LEN(args->str, args->len);
}

static VALUE
rbg_cstr2rval_len_free_ensure(VALUE str)
{
    g_free((gchar *)str);

    return Qnil;
}

VALUE
rbg_cstr2rval_len_free(gchar *str, gsize len)
{
    struct rbg_cstr2rval_len_free_args args = { str, len };

    return str != NULL ? rb_ensure(rbg_cstr2rval_len_free_body, (VALUE)&args,
                                   rbg_cstr2rval_len_free_ensure, (VALUE)str) : Qnil;
}

VALUE
rbg_cstr2rval_with_encoding(const gchar *str, const gchar *encoding)
{
    return str != NULL ? CSTR2RVAL_LEN_ENC(str, strlen(str), encoding) : Qnil;
}

VALUE
rbg_cstr2rval_len_with_encoding(const gchar *str, gsize len,
                                const gchar *encoding)
{
    if (str == NULL)
        return Qnil;

    return rb_external_str_new_with_enc(str, len,
                                        encoding != NULL ?
                                            rb_enc_find(encoding) :
                                            rb_utf8_encoding());
}

static VALUE
rbg_cstr2rval_free_body(VALUE str)
{
    return CSTR2RVAL((const gchar *)str);
}

static VALUE
rbg_cstr2rval_free_ensure(VALUE str)
{
    g_free((gchar *)str);

    return Qnil;
}

VALUE
rbg_cstr2rval_free(gchar *str)
{
    return str != NULL? rb_ensure(rbg_cstr2rval_free_body, (VALUE)str,
                                  rbg_cstr2rval_free_ensure, (VALUE)str) : Qnil;
}

/* just for backward compatibility. */
VALUE
rbg_cstr2rval_with_free(gchar *str)
{
    return rbg_cstr2rval_free(str);
}

rb_encoding *rbg_filename_encoding;

static VALUE
rbg_filename_to_ruby_body(VALUE filename)
{
    const gchar *filename_utf8 = (const gchar *)filename;
    VALUE rb_filename;

    rb_filename = rb_external_str_new_with_enc(filename_utf8,
                                               strlen(filename_utf8),
                                               rb_utf8_encoding());

    /* if needed, change encoding of Ruby String to filename encoding, so that
       upcoming File operations will work properly */
    return rbg_filename_encoding == rb_utf8_encoding() ?
        rb_filename :
        rb_str_export_to_enc(rb_filename, rbg_filename_encoding);
}

static VALUE
rbg_filename_to_ruby_ensure(VALUE filename)
{
    g_free((gchar *)filename);

    return Qnil;
}

VALUE
rbg_filename_to_ruby(const gchar *filename)
{
    gchar *filename_utf8;
    gsize written;
    GError *error = NULL;

    if (filename == NULL)
        return Qnil;

    if (rbg_filename_encoding == rb_utf8_encoding())
        return CSTR2RVAL(filename);

    filename_utf8 = g_filename_to_utf8(filename, -1, NULL, &written, &error);
    if (error != NULL)
        RAISE_GERROR(error);

    return rb_ensure(rbg_filename_to_ruby_body, (VALUE)filename_utf8,
                     rbg_filename_to_ruby_ensure, (VALUE)filename_utf8);
}

VALUE
rbg_filename_to_ruby_free(gchar *filename)
{
    gchar *filename_utf8;
    gsize written;

    if (filename == NULL)
        return Qnil;

    /* convert filename to UTF-8 if needed */
    if (rbg_filename_encoding == rb_utf8_encoding()) {
        filename_utf8 = filename;
    } else {
        GError *error = NULL;

        filename_utf8 = g_filename_to_utf8(filename, -1, NULL, &written, &error);
        g_free(filename);
        if (error != NULL)
            RAISE_GERROR(error);
    }

    return rb_ensure(rbg_filename_to_ruby_body, (VALUE)filename_utf8,
                     rbg_filename_to_ruby_ensure, (VALUE)filename_utf8);
}

gchar *
rbg_filename_from_ruby(VALUE filename)
{
    gchar *retval;
    gsize written;
    GError *error = NULL;

    /* if needed, change encoding of Ruby String to UTF-8 */
    StringValue(filename);
    if (rb_enc_get(filename) != rb_utf8_encoding())
        filename = rb_str_export_to_enc(filename, rb_utf8_encoding());

    /* convert it to filename encoding if needed */
    if (rbg_filename_encoding == rb_utf8_encoding())
        return g_strndup(RSTRING_PTR(filename), RSTRING_LEN(filename));

    retval = g_filename_from_utf8(RSTRING_PTR(filename),
                                  RSTRING_LEN(filename),
                                  NULL,
                                  &written,
                                  &error);
    if (error != NULL)
        RAISE_GERROR(error);

    return retval;
}

struct rval2strv_args {
    VALUE ary;
    long n;
    gchar **result;
};

static VALUE
rbg_rval2strv_body(VALUE value)
{
    long i;
    struct rval2strv_args *args = (struct rval2strv_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = (gchar *)RVAL2CSTR(RARRAY_PTR(args->ary)[i]);
    args->result[args->n] = NULL;

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2strv_rescue(VALUE value, VALUE error)
{
    g_free(((struct rval2strv_args *)value)->result);

    rb_exc_raise(error);
}

gchar **
rbg_rval2strv(volatile VALUE *value, long *n)
{
    struct rval2strv_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(gchar *, args.n + 1);

    rb_rescue(rbg_rval2strv_body, (VALUE)&args,
              rbg_rval2strv_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

gchar **
rbg_rval2strv_accept_nil(volatile VALUE *value, long *n)
{
    if (!NIL_P(*value))
        return rbg_rval2strv(value, n);

    if (n != NULL)
        *n = 0;

    return NULL;
}

struct rval2strv_dup_args {
    VALUE ary;
    long n;
    gchar **result;
};

static VALUE
rbg_rval2strv_dup_body(VALUE value)
{
    long i;
    struct rval2strv_dup_args *args = (struct rval2strv_dup_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = g_strdup(RVAL2CSTR(RARRAY_PTR(args->ary)[i]));
    args->result[args->n] = NULL;

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2strv_dup_rescue(VALUE value, VALUE error)
{
    g_strfreev(((struct rval2strv_dup_args *)value)->result);

    rb_exc_raise(error);
}

gchar **
rbg_rval2strv_dup(volatile VALUE *value, long *n)
{
    struct rval2strv_dup_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new0(gchar *, args.n + 1);

    rb_rescue(rbg_rval2strv_dup_body, (VALUE)&args,
              rbg_rval2strv_dup_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

gchar **
rbg_rval2strv_dup_accept_nil(volatile VALUE *value, long *n)
{
    if (!NIL_P(*value))
        rbg_rval2strv_dup(value, n);

    if (n != NULL)
        *n = 0;

    return NULL;
}

struct rval2filenamev_args {
    VALUE ary;
    long n;
    gchar **result;
};

static VALUE
rbg_rval2filenamev_body(VALUE value)
{
    long i;
    struct rval2filenamev_args *args = (struct rval2filenamev_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = RVAL2CSTRFILENAME(RARRAY_PTR(args->ary)[i]);
    args->result[args->n] = NULL;

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2filenamev_rescue(VALUE value, VALUE error)
{
    g_strfreev(((struct rval2filenamev_args *)value)->result);

    rb_exc_raise(error);
}

gchar **
rbg_rval2filenamev(volatile VALUE *value, long *n)
{
    struct rval2filenamev_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new0(gchar *, args.n + 1);

    rb_rescue(rbg_rval2filenamev_body, (VALUE)&args,
              rbg_rval2filenamev_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

gchar **
rbg_rval2filenamev_accept_nil(volatile VALUE *value, long *n)
{
    if (!NIL_P(*value))
        return rbg_rval2filenamev(value, n);

    if (n != NULL)
        *n = 0;

    return NULL;
}

VALUE
rbg_strv2rval(const gchar **strings)
{
    VALUE ary;
    const gchar **p;

    if (strings == NULL)
        return Qnil;

    ary = rb_ary_new();
    for (p = strings; *p != NULL; p++)
        rb_ary_push(ary, CSTR2RVAL(*p));

    return ary;
}

static VALUE
rbg_strv2rval_free_body(VALUE strings)
{
    return STRV2RVAL((const gchar **)strings);
}

static VALUE
rbg_strv2rval_free_ensure(VALUE strings)
{
    g_strfreev((gchar **)strings);

    return Qnil;
}

VALUE
rbg_strv2rval_free(gchar **strings)
{
    return rb_ensure(rbg_strv2rval_free_body, (VALUE)strings,
                     rbg_strv2rval_free_ensure, (VALUE)strings);
}

struct rbg_rval2gbooleans_args {
    VALUE ary;
    long n;
    gboolean *result;
};

static VALUE
rbg_rval2gbooleans_body(VALUE value)
{
    long i;
    struct rbg_rval2gbooleans_args *args = (struct rbg_rval2gbooleans_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = RVAL2CBOOL(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gbooleans_rescue(VALUE value, VALUE error)
{
    g_free(((struct rbg_rval2gbooleans_args *)value)->result);

    rb_exc_raise(error);
}

gboolean *
rbg_rval2gbooleans(volatile VALUE *value, long *n)
{
    struct rbg_rval2gbooleans_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(gboolean, args.n + 1);

    rb_rescue(rbg_rval2gbooleans_body, (VALUE)&args,
              rbg_rval2gbooleans_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

struct rbg_rval2gints_args {
    VALUE ary;
    long n;
    gint *result;
};

static VALUE
rbg_rval2gints_body(VALUE value)
{
    long i;
    struct rbg_rval2gints_args *args = (struct rbg_rval2gints_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = NUM2INT(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gints_rescue(VALUE value, VALUE error)
{
    g_free(((struct rbg_rval2gints_args *)value)->result);

    rb_exc_raise(error);
}

gint *
rbg_rval2gints(volatile VALUE *value, long *n)
{
    struct rbg_rval2gints_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(gint, args.n + 1);

    rb_rescue(rbg_rval2gints_body, (VALUE)&args,
              rbg_rval2gints_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

struct rbg_rval2gint8s_args {
    VALUE ary;
    long n;
    gint8 *result;
};

static VALUE
rbg_rval2gint8s_body(VALUE value)
{
    long i;
    struct rbg_rval2gint8s_args *args = (struct rbg_rval2gint8s_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = NUM2CHR(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gint8s_rescue(VALUE value, VALUE error)
{
    g_free(((struct rbg_rval2gint8s_args *)value)->result);

    rb_exc_raise(error);
}

gint8 *
rbg_rval2gint8s(volatile VALUE *value, long *n)
{
    struct rbg_rval2gint8s_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(gint8, args.n + 1);

    rb_rescue(rbg_rval2gint8s_body, (VALUE)&args,
              rbg_rval2gint8s_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

struct rbg_rval2guint8s_args {
    VALUE ary;
    long n;
    guint8 *result;
};

static VALUE
rbg_rval2guint8s_body(VALUE value)
{
    long i;
    struct rbg_rval2guint8s_args *args = (struct rbg_rval2guint8s_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = NUM2UINT(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2guint8s_rescue(VALUE value, VALUE error)
{
    g_free(((struct rbg_rval2guint8s_args *)value)->result);

    rb_exc_raise(error);
}

guint8 *
rbg_rval2guint8s(volatile VALUE *value, long *n)
{
    struct rbg_rval2guint8s_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(guint8, args.n + 1);

    rb_rescue(rbg_rval2guint8s_body, (VALUE)&args,
              rbg_rval2guint8s_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

struct rbg_rval2guint16s_args {
    VALUE ary;
    long n;
    guint16 *result;
};

static VALUE
rbg_rval2guint16s_body(VALUE value)
{
    long i;
    struct rbg_rval2guint16s_args *args = (struct rbg_rval2guint16s_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = NUM2UINT(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2guint16s_rescue(VALUE value, VALUE error)
{
    g_free(((struct rbg_rval2guint16s_args *)value)->result);

    rb_exc_raise(error);
}

guint16 *
rbg_rval2guint16s(volatile VALUE *value, long *n)
{
    struct rbg_rval2guint16s_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(guint16, args.n + 1);

    rb_rescue(rbg_rval2guint16s_body, (VALUE)&args,
              rbg_rval2guint16s_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

struct rbg_rval2guint32s_args {
    VALUE ary;
    long n;
    guint32 *result;
};

static VALUE
rbg_rval2guint32s_body(VALUE value)
{
    long i;
    struct rbg_rval2guint32s_args *args = (struct rbg_rval2guint32s_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = NUM2UINT(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2guint32s_rescue(VALUE value, VALUE error)
{
    g_free(((struct rbg_rval2guint32s_args *)value)->result);

    rb_exc_raise(error);
}

guint32 *
rbg_rval2guint32s(volatile VALUE *value, long *n)
{
    struct rbg_rval2guint32s_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(guint32, args.n + 1);

    rb_rescue(rbg_rval2guint32s_body, (VALUE)&args,
              rbg_rval2guint32s_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

struct rbg_rval2gdoubles_args {
    VALUE ary;
    long n;
    gdouble *result;
};

static VALUE
rbg_rval2gdoubles_body(VALUE value)
{
    long i;
    struct rbg_rval2gdoubles_args *args = (struct rbg_rval2gdoubles_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = NUM2DBL(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gdoubles_rescue(VALUE value, VALUE error)
{
    g_free(((struct rbg_rval2gdoubles_args *)value)->result);

    rb_exc_raise(error);
}

gdouble *
rbg_rval2gdoubles(volatile VALUE *value, long *n)
{
    struct rbg_rval2gdoubles_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(gdouble, args.n + 1);

    rb_rescue(rbg_rval2gdoubles_body, (VALUE)&args,
              rbg_rval2gdoubles_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
}

VALUE
rbg_gints2rval(const gint *gints, long n)
{
    long i;
    VALUE ary = rb_ary_new();

    for (i = 0; i < n; i++)
        rb_ary_push(ary, INT2NUM(gints[i]));

    return ary;
}

struct rbg_gints2rval_free_args {
    gint *gints;
    long n;
};

static VALUE
rbg_gints2rval_free_body(VALUE value)
{
    struct rbg_gints2rval_free_args *args = (struct rbg_gints2rval_free_args *)value;

    return rbg_gints2rval(args->gints, args->n);
}

static VALUE
rbg_gints2rval_free_ensure(VALUE value)
{
    g_free(((struct rbg_gints2rval_free_args *)value)->gints);

    return Qnil;
}

VALUE
rbg_gints2rval_free(gint *gints, long n)
{
    struct rbg_gints2rval_free_args args = { gints, n };

    return rb_ensure(rbg_gints2rval_free_body, (VALUE)&args,
                     rbg_gints2rval_free_ensure, (VALUE)&args);
}

const gchar *
rbg_inspect (VALUE object)
{
    VALUE inspected;

    inspected = rb_funcall(object, rb_intern("inspect"), 0);
    return StringValueCStr(inspected);
}

VALUE
rbg_to_array (VALUE object)
{
    return rb_convert_type(object, RUBY_T_ARRAY, "Array", "to_ary");
}

VALUE
rbg_to_hash (VALUE object)
{
    return rb_convert_type(object, RUBY_T_HASH, "Hash", "to_hash");
}

VALUE
rbg_check_array_type (VALUE object)
{
#ifdef HAVE_RB_CHECK_ARRAY_TYPE
    return rb_check_array_type(object);
#else
    return rb_check_convert_type(object, RUBY_T_ARRAY, "Array", "to_ary");
#endif
}

VALUE
rbg_check_hash_type (VALUE object)
{
#ifdef HAVE_RB_CHECK_HASH_TYPE
    return rb_check_hash_type(object);
#else
    return rb_check_convert_type(object, RUBY_T_HASH, "Hash", "to_hash");
#endif
}

void
rbg_scan_options (VALUE options, ...)
{
    const char *key;
    gsize n_keys = 0;
    gsize n_found_keys = 0;
    va_list args;
    va_list args_copy;

    if (!NIL_P(options)) {
        VALUE original_options = options;
        options = rbg_check_hash_type(options);
        if (NIL_P(options)) {
            rb_raise(rb_eArgError,
                     "options must be Hash or nil: %+" PRIsVALUE,
                     original_options);
        }
    }

    va_start(args, options);
    va_copy(args_copy, args);
    key = va_arg(args, const char *);
    n_keys = 0;
    n_found_keys = 0;
    while (key) {
        VALUE *value;

        value = va_arg(args, VALUE *);
        if (NIL_P(options)) {
            *value = Qnil;
        } else {
            VALUE rb_key;
            rb_key = ID2SYM(rb_intern(key));
            if (RTEST(rb_funcall(options, rb_intern("key?"), 1, rb_key))) {
                n_found_keys++;
            }
            *value = rb_hash_aref(options, rb_key);
        }
        n_keys++;

        key = va_arg(args, const char *);
    }
    va_end(args);

    if (NIL_P(options)) {
        return;
    }

    if (n_found_keys == RHASH_SIZE(options)) {
        return;
    }

    {
        VALUE rb_available_keys;

        rb_available_keys = rb_ary_new();
        key = va_arg(args_copy, const char *);
        while (key) {
            VALUE rb_key;

            va_arg(args_copy, VALUE *);
            rb_key = ID2SYM(rb_intern(key));
            rb_ary_push(rb_available_keys, rb_key);
            key = va_arg(args_copy, const char *);
        }
        va_end(args_copy);

        rb_raise(rb_eArgError,
                 "unexpected key(s) exist: %+" PRIsVALUE
                 ": available keys: %+" PRIsVALUE,
                 rb_funcall(rb_funcall(options, rb_intern("keys"), 0),
                            rb_intern("-"),
                            1,
                            rb_available_keys),
                 rb_available_keys);
    }
}

#if 0
/*
2004-04-15 Commented out by Masao.

These functions replace g_malloc/g_realloc/g_free of GLib.
When g_malloc is called and the memory area can not reserved,
rb_gc() will be called. It makes Ruby-GNOME2 uses memory efficiently.

But rb_gc() does not work under multithread.
So they occur "cross-thread violation".
*/

static gpointer
my_malloc(gsize n_bytes)
{
    /* Should we rescue NoMemoryError? */
    return ruby_xmalloc(n_bytes);
}

static gpointer
my_realloc(gpointer mem, gsize n_bytes)
{
    /* Should we rescue NoMemoryError? */
    return ruby_xrealloc(mem, n_bytes);
}

static void
my_free(gpointer mem)
{
    return ruby_xfree(mem);
}

static void
Init_mem()
{
    GMemVTable mem_table = {
        my_malloc,
        my_realloc,
        my_free,
        NULL,
        NULL,
        NULL,
    };
    g_mem_set_vtable(&mem_table);
}
#endif

static VALUE
rg_s_os_win32_p(G_GNUC_UNUSED VALUE self)
{
#ifdef G_OS_WIN32
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
rg_s_os_beos_p(G_GNUC_UNUSED VALUE self)
{
#ifdef G_OS_BEOS
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
rg_s_os_unix_p(G_GNUC_UNUSED VALUE self)
{
#ifdef G_OS_UNIX
    return Qtrue;
#else
    return Qfalse;
#endif
}

extern void Init_glib2(void);

void
Init_glib2(void)
{
#ifdef HAVE_RB_EXT_RACTOR_SAFE
    rb_ext_ractor_safe(true);
#endif

    const gchar **filename_charsets;

    id_inspect = rb_intern("inspect");

    RG_TARGET_NAMESPACE = rb_define_module("GLib");

    setlocale (LC_CTYPE, "");
#ifdef LC_MESSAGES
    setlocale (LC_MESSAGES, "");
#endif

    /* Version Information */
    rb_define_const(RG_TARGET_NAMESPACE, "VERSION",
                    rb_ary_new3(3,
                                INT2FIX(glib_major_version),
                                INT2FIX(glib_minor_version),
                                INT2FIX(glib_micro_version)));
    rb_define_const(RG_TARGET_NAMESPACE, "MAJOR_VERSION", INT2FIX(glib_major_version));
    rb_define_const(RG_TARGET_NAMESPACE, "MINOR_VERSION", INT2FIX(glib_minor_version));
    rb_define_const(RG_TARGET_NAMESPACE, "MICRO_VERSION", INT2FIX(glib_micro_version));
    rb_define_const(RG_TARGET_NAMESPACE, "INTERFACE_AGE", INT2FIX(glib_interface_age));
    rb_define_const(RG_TARGET_NAMESPACE, "BINARY_AGE", INT2FIX(glib_binary_age));

    rb_define_const(RG_TARGET_NAMESPACE, "BINDING_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(RBGLIB_MAJOR_VERSION),
                                INT2FIX(RBGLIB_MINOR_VERSION),
                                INT2FIX(RBGLIB_MICRO_VERSION)));

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GLIB_MAJOR_VERSION),
                                INT2FIX(GLIB_MINOR_VERSION),
                                INT2FIX(GLIB_MICRO_VERSION)));

    /* Limits of Basic Types */
    rb_define_const(RG_TARGET_NAMESPACE, "MININT", INT2FIX(G_MININT));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXINT", INT2NUM(G_MAXINT));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXUINT", UINT2NUM(G_MAXUINT));

    rb_define_const(RG_TARGET_NAMESPACE, "MINSHORT", INT2FIX(G_MINSHORT));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXSHORT", INT2FIX(G_MAXSHORT));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXUSHORT", UINT2NUM(G_MAXUSHORT));

    rb_define_const(RG_TARGET_NAMESPACE, "MINLONG", LL2NUM(G_MINLONG));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXLONG", LL2NUM(G_MAXLONG));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXULONG", ULL2NUM(G_MAXULONG));

    rb_define_const(RG_TARGET_NAMESPACE, "MININT8", INT2FIX(G_MININT8));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXINT8", INT2FIX(G_MAXINT8));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXUINT8", UINT2NUM(G_MAXUINT8));

    rb_define_const(RG_TARGET_NAMESPACE, "MININT16", INT2FIX(G_MININT16));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXINT16", INT2FIX(G_MAXINT16));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXUINT16", UINT2NUM(G_MAXUINT16));

    rb_define_const(RG_TARGET_NAMESPACE, "MININT32", INT2FIX(G_MININT32));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXINT32", INT2NUM(G_MAXINT32));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXUINT32", UINT2NUM(G_MAXUINT32));

    rb_define_const(RG_TARGET_NAMESPACE, "MININT64", LL2NUM(G_MININT64));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXINT64", LL2NUM(G_MAXINT64));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXUINT64", ULL2NUM(G_MAXUINT64));

    rb_define_const(RG_TARGET_NAMESPACE, "MAXSIZE", ULL2NUM(G_MAXSIZE));

    rb_define_const(RG_TARGET_NAMESPACE, "MINFLOAT", DBL2NUM(G_MINFLOAT));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXFLOAT", DBL2NUM(G_MAXFLOAT));

    rb_define_const(RG_TARGET_NAMESPACE, "MINDOUBLE", DBL2NUM(G_MINDOUBLE));
    rb_define_const(RG_TARGET_NAMESPACE, "MAXDOUBLE", DBL2NUM(G_MAXDOUBLE));

    /* Standard Macros */
    RG_DEF_SMETHOD_P(os_win32, 0);
    RG_DEF_SMETHOD_P(os_beos, 0);
    RG_DEF_SMETHOD_P(os_unix, 0);

    rb_define_const(RG_TARGET_NAMESPACE, "DIR_SEPARATOR", CSTR2RVAL(G_DIR_SEPARATOR_S));
    rb_define_const(RG_TARGET_NAMESPACE, "SEARCHPATH_SEPARATOR", CSTR2RVAL(G_SEARCHPATH_SEPARATOR_S));

    /* discover and store glib filename encoding */
    if (g_get_filename_charsets(&filename_charsets)
        || filename_charsets == NULL
        || filename_charsets[0] == NULL
        || !strcmp(filename_charsets[0], "UTF-8")
        || rb_enc_find(filename_charsets[0]) == rb_enc_find("ASCII-8BIT")) {
        rbg_filename_encoding = rb_utf8_encoding();
    } else {
        rbg_filename_encoding = rb_enc_find(filename_charsets[0]);
    }
    rb_define_const(RG_TARGET_NAMESPACE,
                    "FILENAME_ENCODING",
                    rb_enc_from_encoding(rbg_filename_encoding));

/* Don't implement them.
#define     G_DIR_SEPARATOR_S
#define     G_IS_DIR_SEPARATOR              (c)
#define     G_SEARCHPATH_SEPARATOR
#define     TRUE
#define     FALSE
#define     NULL
#define     MIN                             (a, b)
#define     MAX                             (a, b)
#define     ABS                             (a)
#define     CLAMP                           (x, low, high)
#define     G_STRUCT_MEMBER                 (member_type, struct_p, struct_offset)
#define     G_STRUCT_MEMBER_P               (struct_p, struct_offset)
#define     G_STRUCT_OFFSET                 (struct_type, member)
#define     G_MEM_ALIGN
#define     G_CONST_RETURN
*/

    /* Numerical Definitions */
/* Don't implement them.
#define     G_IEEE754_FLOAT_BIAS
#define     G_IEEE754_DOUBLE_BIAS
union       GFloatIEEE754;
union       GDoubleIEEE754;
*/
    rb_define_const(RG_TARGET_NAMESPACE, "E", CSTR2RVAL(G_STRINGIFY(G_E)));
    rb_define_const(RG_TARGET_NAMESPACE, "LN2", CSTR2RVAL(G_STRINGIFY(G_LN2)));
    rb_define_const(RG_TARGET_NAMESPACE, "LN10", CSTR2RVAL(G_STRINGIFY(G_LN10)));
    rb_define_const(RG_TARGET_NAMESPACE, "PI", CSTR2RVAL(G_STRINGIFY(G_PI)));
    rb_define_const(RG_TARGET_NAMESPACE, "PI_2", CSTR2RVAL(G_STRINGIFY(G_PI_2)));
    rb_define_const(RG_TARGET_NAMESPACE, "PI_4", CSTR2RVAL(G_STRINGIFY(G_PI_4)));
    rb_define_const(RG_TARGET_NAMESPACE, "SQRT2", CSTR2RVAL(G_STRINGIFY(G_SQRT2)));
    rb_define_const(RG_TARGET_NAMESPACE, "LOG_2_BASE_10", CSTR2RVAL(G_STRINGIFY(G_LOG_2_BASE_10)));

    /* From "The Main Event Loop" */
    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_HIGH", INT2FIX(G_PRIORITY_HIGH));
    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_DEFAULT", INT2FIX(G_PRIORITY_DEFAULT));
    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_HIGH_IDLE", INT2FIX(G_PRIORITY_HIGH_IDLE));
    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_DEFAULT_IDLE", INT2FIX(G_PRIORITY_DEFAULT_IDLE));
    rb_define_const(RG_TARGET_NAMESPACE, "PRIORITY_LOW", INT2FIX(G_PRIORITY_LOW));

/*    Init_mem(); */

    Init_glib_gc();

    Init_gutil();
    Init_gutil_callback();

    Init_glib_gettext();
    Init_glib_int64();
    Init_glib_error();
    Init_glib_threads();
    Init_glib_convert();
    Init_glib_messages();
    Init_glib_fileutils();
    Init_glib_i18n();
    Init_glib_win32();

    Init_gobject();

    /* Require GBoxed/GObject */
    Init_glib_error_conversions();
    Init_glib_utils();
    Init_glib_spawn();
    Init_glib_spawnerror();
    Init_glib_main_loop();
    Init_glib_source();
    Init_glib_main_context();
    Init_glib_poll_fd();
    Init_glib_io_constants();
    Init_glib_io_channel();
    Init_glib_io_channelerror();
    Init_glib_io_channel_win32_socket();
    Init_glib_shell();
    Init_glib_shellerror();
    Init_glib_timer();
    Init_glib_unicode();
    Init_glib_utf8();
    Init_glib_utf16();
    Init_glib_ucs4();
    Init_glib_unichar();
    Init_glib_keyfile();
    Init_glib_bookmark_file();
    Init_glib_variant_type();
    Init_glib_variant();
    Init_glib_regex();
    Init_glib_matchinfo();
    Init_glib_date_time();
    Init_glib_time_zone();
    Init_glib_bytes();
}
