/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.c -

  $Author: sakai $
  $Date: 2007/07/14 13:42:08 $

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include <locale.h>
#include "rbgprivate.h"
#include "rbglib.h"

static ID id_inspect;

VALUE mGLib;

extern void Init_gutil();
extern void Init_gutil_callback();
extern void Init_glib_int64();
extern void Init_glib_error();
extern void Init_glib_threads();
extern void Init_glib_convert();
extern void Init_glib_messages();
extern void Init_glib_spawn();
extern void Init_glib_fileutils();
extern void Init_glib_utils();
extern void Init_glib_i18n();
extern void Init_glib_win32();
extern void Init_gobject();
extern void Init_glib_main_loop();
extern void Init_glib_main_context();
extern void Init_glib_source();
extern void Init_glib_poll_fd();
extern void Init_glib_io_channel();
extern void Init_glib_shell();
extern void Init_glib_completion();
extern void Init_glib_timer();
extern void Init_glib_unicode();
extern void Init_glib_keyfile();
extern void Init_glib_bookmark_file();

const gchar *
rbg_rval2cstr(VALUE *str)
{
    StringValue(*str);

#ifdef HAVE_RUBY_ENCODING_H
    if (rb_enc_get(*str) != rb_utf8_encoding())
        *str = rb_str_export_to_enc(*str, rb_utf8_encoding());
#endif

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

#ifdef HAVE_RUBY_ENCODING_H
    return rb_external_str_new_with_enc(str, len, rb_utf8_encoding());
#else
    return rb_str_new(str, len);
#endif
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

#ifdef HAVE_RUBY_ENCODING_H
    return rb_external_str_new_with_enc(str, len,
                                        encoding != NULL ?
                                            rb_enc_find(encoding) :
                                            rb_utf8_encoding());
#else
    return rb_str_new(str, len);
#endif
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

#ifdef HAVE_RUBY_ENCODING_H
static rb_encoding *filename_encoding_if_not_utf8;
#endif

#ifdef HAVE_RUBY_ENCODING_H
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
    return filename_encoding_if_not_utf8 != NULL ?
        rb_str_export_to_enc(rb_filename, filename_encoding_if_not_utf8) :
        rb_filename;
}

static VALUE
rbg_filename_to_ruby_ensure(VALUE filename)
{
    g_free((gchar *)filename);

    return Qnil;
}
#endif

VALUE
rbg_filename_to_ruby(const gchar *filename)
{
#ifdef HAVE_RUBY_ENCODING_H
    gchar *filename_utf8;
    gsize written;
    GError *error = NULL;

    if (filename == NULL)
        return Qnil;

    if (filename_encoding_if_not_utf8 == NULL)
        return CSTR2RVAL(filename);

    filename_utf8 = g_filename_to_utf8(filename, -1, NULL, &written, &error);
    if (error != NULL)
        RAISE_GERROR(error);

    return rb_ensure(rbg_filename_to_ruby_body, (VALUE)filename_utf8,
                     rbg_filename_to_ruby_ensure, (VALUE)filename_utf8);
#else
    return CSTR2RVAL(filename);
#endif
}

VALUE
rbg_filename_to_ruby_free(gchar *filename)
{
#ifdef HAVE_RUBY_ENCODING_H
    gchar *filename_utf8;
    gsize written;

    if (filename == NULL)
        return Qnil;

    /* convert filename to UTF-8 if needed */
    if (filename_encoding_if_not_utf8 != NULL) {
        GError *error = NULL;

        filename_utf8 = g_filename_to_utf8(filename, -1, NULL, &written, &error);
        g_free(filename);
        if (error != NULL)
            RAISE_GERROR(error);
    } else {
        filename_utf8 = filename;
    }

    return rb_ensure(rbg_filename_to_ruby_body, (VALUE)filename_utf8,
                     rbg_filename_to_ruby_ensure, (VALUE)filename_utf8);
#else
    return CSTR2RVAL_FREE(filename);
#endif
}

gchar *
rbg_filename_from_ruby(VALUE filename)
{
#ifdef HAVE_RUBY_ENCODING_H
    gchar *retval;
    gsize written;
    GError *error = NULL;

    /* if needed, change encoding of Ruby String to UTF-8 */
    StringValue(filename);
    if (rb_enc_get(filename) != rb_utf8_encoding())
        filename = rb_str_export_to_enc(filename, rb_utf8_encoding());

    /* convert it to filename encoding if needed */
    if (filename_encoding_if_not_utf8 == NULL)
        return g_strdup(RSTRING_PTR(filename));

    retval = g_filename_from_utf8(RSTRING_PTR(filename), -1, NULL, &written, &error);
    if (error != NULL)
        RAISE_GERROR(error);

    return retval;
#else
    return g_strdup(RVAL2CSTR(filename));
#endif
}

static VALUE
rbg_filename_gslist_to_array_free_body(VALUE list)
{
    VALUE ary = rb_ary_new();
    GSList *p;

    for (p = (GSList *)list; p != NULL; p = g_slist_next(p))
        rb_ary_push(ary, CSTRFILENAME2RVAL(p->data));

    return ary;
}

static VALUE
rbg_filename_gslist_to_array_free_ensure(VALUE val)
{
    GSList *list = (GSList *)val;
    GSList *p;

    for (p = list; p != NULL; p = g_slist_next(p))
        g_free((gchar *)p->data);

    g_slist_free(list);

    return Qnil;
}

VALUE
rbg_filename_gslist_to_array_free(GSList *list)
{
    return rb_ensure(rbg_filename_gslist_to_array_free_body, (VALUE)list,
                     rbg_filename_gslist_to_array_free_ensure, (VALUE)list);
}

struct rval2strv_args {
    VALUE ary;
    long n;
    const gchar **result;
};

static VALUE
rbg_rval2strv_body(VALUE value)
{
    long i;
    struct rval2strv_args *args = (struct rval2strv_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = RVAL2CSTR(RARRAY_PTR(args->ary)[i]);
    args->result[args->n] = NULL;

    return Qnil;
}

static VALUE
rbg_rval2strv_rescue(VALUE value)
{
    g_free(((struct rval2strv_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

const gchar **
rbg_rval2strv(VALUE value, long *n)
{
    struct rval2strv_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(const gchar *, args.n + 1);

    rb_rescue(rbg_rval2strv_body, (VALUE)&args,
              rbg_rval2strv_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

const gchar **
rbg_rval2strv_accept_nil(VALUE ary, long *n)
{
    if (!NIL_P(ary))
        return rbg_rval2strv(ary, n);

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

static VALUE
rbg_rval2strv_dup_rescue(VALUE value)
{
    g_free(((struct rval2strv_dup_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

gchar **
rbg_rval2strv_dup(VALUE value, long *n)
{
    struct rval2strv_dup_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(gchar *, args.n + 1);

    rb_rescue(rbg_rval2strv_dup_body, (VALUE)&args,
              rbg_rval2strv_dup_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

gchar **
rbg_rval2strv_dup_accept_nil(VALUE ary, long *n)
{
    if (!NIL_P(ary))
        rbg_rval2strv_dup(ary, n);

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

static VALUE
rbg_rval2gbooleans_rescue(VALUE value)
{
    g_free(((struct rbg_rval2gbooleans_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

gboolean *
rbg_rval2gbooleans(VALUE value, long *n)
{
    struct rbg_rval2gbooleans_args args;

    args.ary = rb_ary_to_ary(value);
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

static VALUE
rbg_rval2gints_rescue(VALUE value)
{
    g_free(((struct rbg_rval2gints_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

gint *
rbg_rval2gints(VALUE value, long *n)
{
    struct rbg_rval2gints_args args;

    args.ary = rb_ary_to_ary(value);
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

static VALUE
rbg_rval2gint8s_rescue(VALUE value)
{
    g_free(((struct rbg_rval2gint8s_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

gint8 *
rbg_rval2gint8s(VALUE value, long *n)
{
    struct rbg_rval2gint8s_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(gint8, args.n + 1);

    rb_rescue(rbg_rval2gint8s_body, (VALUE)&args,
              rbg_rval2gint8s_rescue, (VALUE)&args);

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

static VALUE
rbg_rval2guint16s_rescue(VALUE value)
{
    g_free(((struct rbg_rval2guint16s_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

guint16 *
rbg_rval2guint16s(VALUE value, long *n)
{
    struct rbg_rval2guint16s_args args;

    args.ary = rb_ary_to_ary(value);
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

static VALUE
rbg_rval2guint32s_rescue(VALUE value)
{
    g_free(((struct rbg_rval2guint32s_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

guint32 *
rbg_rval2guint32s(VALUE value, long *n)
{
    struct rbg_rval2guint32s_args args;

    args.ary = rb_ary_to_ary(value);
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

static VALUE
rbg_rval2gdoubles_rescue(VALUE value)
{
    g_free(((struct rbg_rval2gdoubles_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

gdouble *
rbg_rval2gdoubles(VALUE value, long *n)
{
    struct rbg_rval2gdoubles_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(gdouble, args.n + 1);

    rb_rescue(rbg_rval2gdoubles_body, (VALUE)&args,
              rbg_rval2gdoubles_rescue, (VALUE)&args);

    *n = args.n;

    return args.result;
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
rbg_s_os_win32(VALUE self)
{
#ifdef G_OS_WIN32
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
rbg_s_os_beos(VALUE self)
{
#ifdef G_OS_BEOS
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
rbg_s_os_unix(VALUE self)
{
#ifdef G_OS_UNIX
    return Qtrue;
#else
    return Qfalse;
#endif
}

void 
Init_glib2()
{
#ifdef HAVE_RUBY_ENCODING_H
    const gchar **filename_charsets;
#endif

    id_inspect = rb_intern("inspect");

    mGLib = rb_define_module("GLib");

    setlocale (LC_CTYPE, "");
#if LC_MESSAGES
    setlocale (LC_MESSAGES, "");
#endif

    /* Version Information */
    rb_define_const(mGLib, "VERSION",
                    rb_ary_new3(3,
                                INT2FIX(glib_major_version),
                                INT2FIX(glib_minor_version),
                                INT2FIX(glib_micro_version)));
    rb_define_const(mGLib, "MAJOR_VERSION", INT2FIX(glib_major_version));
    rb_define_const(mGLib, "MINOR_VERSION", INT2FIX(glib_minor_version));
    rb_define_const(mGLib, "MICRO_VERSION", INT2FIX(glib_micro_version));
    rb_define_const(mGLib, "INTERFACE_AGE", INT2FIX(glib_interface_age));
    rb_define_const(mGLib, "BINARY_AGE", INT2FIX(glib_binary_age));

    rb_define_const(mGLib, "BINDING_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(RBGLIB_MAJOR_VERSION),
                                INT2FIX(RBGLIB_MINOR_VERSION),
                                INT2FIX(RBGLIB_MICRO_VERSION)));

    rb_define_const(mGLib, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GLIB_MAJOR_VERSION),
                                INT2FIX(GLIB_MINOR_VERSION),
                                INT2FIX(GLIB_MICRO_VERSION)));

    /* Limits of Basic Types */
    rb_define_const(mGLib, "MININT", INT2FIX(G_MININT));
    rb_define_const(mGLib, "MAXINT", INT2NUM(G_MAXINT));
    rb_define_const(mGLib, "MAXUINT", UINT2NUM(G_MAXUINT));
    
    rb_define_const(mGLib, "MINSHORT", INT2FIX(G_MINSHORT));
    rb_define_const(mGLib, "MAXSHORT", INT2FIX(G_MAXSHORT));
    rb_define_const(mGLib, "MAXUSHORT", UINT2NUM(G_MAXUSHORT));

    rb_define_const(mGLib, "MINLONG", INT2FIX(G_MINLONG));
    rb_define_const(mGLib, "MAXLONG", INT2NUM(G_MAXLONG));
    rb_define_const(mGLib, "MAXULONG", UINT2NUM(G_MAXULONG));

#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_const(mGLib, "MININT8", INT2FIX(G_MININT8));
    rb_define_const(mGLib, "MAXINT8", INT2FIX(G_MAXINT8));
    rb_define_const(mGLib, "MAXUINT8", UINT2NUM(G_MAXUINT8));

    rb_define_const(mGLib, "MININT16", INT2FIX(G_MININT16));
    rb_define_const(mGLib, "MAXINT16", INT2FIX(G_MAXINT16));
    rb_define_const(mGLib, "MAXUINT16", UINT2NUM(G_MAXUINT16));

    rb_define_const(mGLib, "MININT32", INT2FIX(G_MININT32));
    rb_define_const(mGLib, "MAXINT32", INT2NUM(G_MAXINT32));
    rb_define_const(mGLib, "MAXUINT32", UINT2NUM(G_MAXUINT32));
#endif
    rb_define_const(mGLib, "MININT64", INT2FIX(G_MININT64));
    rb_define_const(mGLib, "MAXINT64", LL2NUM(G_MAXINT64));
    rb_define_const(mGLib, "MAXUINT64", ULL2NUM(G_MAXUINT64));
#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_const(mGLib, "MAXSIZE", UINT2NUM(G_MAXSIZE));
#endif
    rb_define_const(mGLib, "MINFLOAT", INT2FIX(G_MINFLOAT));
    rb_define_const(mGLib, "MAXFLOAT", DBL2NUM(G_MAXFLOAT));

    rb_define_const(mGLib, "MINDOUBLE", INT2FIX(G_MINDOUBLE));
    rb_define_const(mGLib, "MAXDOUBLE", DBL2NUM(G_MAXDOUBLE));

    /* Standard Macros */
    rb_define_module_function(mGLib, "os_win32?", rbg_s_os_win32, 0);
    rb_define_module_function(mGLib, "os_beos?", rbg_s_os_beos, 0);
    rb_define_module_function(mGLib, "os_unix?", rbg_s_os_unix, 0);

    rb_define_const(mGLib, "DIR_SEPARATOR", CSTR2RVAL(G_DIR_SEPARATOR_S));
    rb_define_const(mGLib, "SEARCHPATH_SEPARATOR", CSTR2RVAL(G_SEARCHPATH_SEPARATOR_S));

    /* discover and store glib filename encoding */
#ifdef HAVE_RUBY_ENCODING_H
    if (g_get_filename_charsets(&filename_charsets)
        || filename_charsets == NULL
        || filename_charsets[0] == NULL
        || !strcmp(filename_charsets[0], "UTF-8")
        || rb_enc_find(filename_charsets[0]) == rb_enc_find("ASCII-8BIT")) {
        /* set to NULL, mean do not perform transcoding, either filename
           encoding is unknown, UTF-8, or unsupported */
        filename_encoding_if_not_utf8 = NULL;
    } else {
        filename_encoding_if_not_utf8 = rb_enc_find(filename_charsets[0]);
    }
#endif

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
    rb_define_const(mGLib, "E", CSTR2RVAL(G_STRINGIFY(G_E)));
    rb_define_const(mGLib, "LN2", CSTR2RVAL(G_STRINGIFY(G_LN2)));
    rb_define_const(mGLib, "LN10", CSTR2RVAL(G_STRINGIFY(G_LN10)));
    rb_define_const(mGLib, "PI", CSTR2RVAL(G_STRINGIFY(G_PI)));
    rb_define_const(mGLib, "PI_2", CSTR2RVAL(G_STRINGIFY(G_PI_2)));
    rb_define_const(mGLib, "PI_4", CSTR2RVAL(G_STRINGIFY(G_PI_4)));
    rb_define_const(mGLib, "SQRT2", CSTR2RVAL(G_STRINGIFY(G_SQRT2)));
    rb_define_const(mGLib, "LOG_2_BASE_10", CSTR2RVAL(G_STRINGIFY(G_LOG_2_BASE_10)));

    /* From "The Main Event Loop" */
    rb_define_const(mGLib, "PRIORITY_HIGH", INT2FIX(G_PRIORITY_HIGH));
    rb_define_const(mGLib, "PRIORITY_DEFAULT", INT2FIX(G_PRIORITY_DEFAULT));
    rb_define_const(mGLib, "PRIORITY_HIGH_IDLE", INT2FIX(G_PRIORITY_HIGH_IDLE));
    rb_define_const(mGLib, "PRIORITY_DEFAULT_IDLE", INT2FIX(G_PRIORITY_DEFAULT_IDLE));
    rb_define_const(mGLib, "PRIORITY_LOW", INT2FIX(G_PRIORITY_LOW));


/*    Init_mem(); */
    Init_gutil();
    Init_gutil_callback();
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
    Init_glib_utils();
    Init_glib_spawn();
    Init_glib_main_loop();
    Init_glib_source();
    Init_glib_main_context();
    Init_glib_poll_fd();
    Init_glib_io_channel();
    Init_glib_shell();
    Init_glib_completion();
    Init_glib_timer();
    Init_glib_unicode();
    Init_glib_keyfile();
    Init_glib_bookmark_file();

    /* This is called here once. */
    G_DEF_SETTERS(mGLib);
}
