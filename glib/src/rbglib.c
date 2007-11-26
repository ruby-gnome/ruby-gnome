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

char *
rbg_string_value_ptr(ptr)
    volatile VALUE *ptr;
{
    return rb_string_value_ptr(ptr);
}

VALUE
rbg_cstr2rval(const char* str)
{
    return str ? rb_str_new2(str) : Qnil;
}

VALUE
rbg_cstr2rval_with_free(gchar* str)
{
    VALUE ret = Qnil;
    if (str) {
        ret = rb_str_new2(str);
        g_free(str);
    }
    return ret;
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
rbg_s_os_win32(self)
    VALUE self;
{
#ifdef G_OS_WIN32
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
rbg_s_os_beos(self)
    VALUE self;
{
#ifdef G_OS_BEOS
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
rbg_s_os_unix(self)
    VALUE self;
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
    rb_define_const(mGLib, "MAXINT", INT2FIX(G_MAXINT));
    rb_define_const(mGLib, "MAXUINT", INT2FIX(G_MAXUINT));
    
    rb_define_const(mGLib, "MINSHORT", INT2FIX(G_MINSHORT));
    rb_define_const(mGLib, "MAXSHORT", INT2FIX(G_MAXSHORT));
    rb_define_const(mGLib, "MAXUSHORT", INT2FIX(G_MAXUSHORT));

    rb_define_const(mGLib, "MINLONG", INT2FIX(G_MINLONG));
    rb_define_const(mGLib, "MAXLONG", INT2FIX(G_MAXLONG));
    rb_define_const(mGLib, "MAXULONG", INT2FIX(G_MAXULONG));

#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_const(mGLib, "MININT8", INT2FIX(G_MININT8));
    rb_define_const(mGLib, "MAXINT8", INT2FIX(G_MAXINT8));
    rb_define_const(mGLib, "MAXUINT8", INT2FIX(G_MAXUINT8));

    rb_define_const(mGLib, "MININT16", INT2FIX(G_MININT16));
    rb_define_const(mGLib, "MAXINT16", INT2FIX(G_MAXINT16));
    rb_define_const(mGLib, "MAXUINT16", INT2FIX(G_MAXUINT16));

    rb_define_const(mGLib, "MININT32", INT2FIX(G_MININT32));
    rb_define_const(mGLib, "MAXINT32", INT2FIX(G_MAXINT32));
    rb_define_const(mGLib, "MAXUINT32", INT2FIX(G_MAXUINT32));
#endif
    rb_define_const(mGLib, "MININT64", INT2FIX(G_MININT64));
    rb_define_const(mGLib, "MAXINT64", INT2FIX(G_MAXINT64));
    rb_define_const(mGLib, "MAXUINT64", INT2FIX(G_MAXUINT64));
#if GLIB_CHECK_VERSION(2,4,0)
    rb_define_const(mGLib, "MAXSIZE", INT2FIX(G_MAXSIZE));
#endif
    rb_define_const(mGLib, "MINFLOAT", INT2FIX(G_MINFLOAT));
    rb_define_const(mGLib, "MAXFLOAT", INT2FIX(G_MAXFLOAT));

    rb_define_const(mGLib, "MINDOUBLE", INT2FIX(G_MINDOUBLE));
    rb_define_const(mGLib, "MAXDOUBLE", INT2FIX(G_MAXDOUBLE));

    /* Standard Macros */
    rb_define_module_function(mGLib, "os_win32?", rbg_s_os_win32, 0);
    rb_define_module_function(mGLib, "os_beos?", rbg_s_os_beos, 0);
    rb_define_module_function(mGLib, "os_unix?", rbg_s_os_unix, 0);

    rb_define_const(mGLib, "DIR_SEPARATOR", CSTR2RVAL(G_DIR_SEPARATOR_S));
    rb_define_const(mGLib, "SEARCHPATH_SEPARATOR", CSTR2RVAL(G_SEARCHPATH_SEPARATOR_S));

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
    Init_glib_utils();
    Init_glib_i18n();
    Init_glib_win32();

    Init_gobject();

    /* Require GBoxed/GObject */
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
