/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.c -

  $Author: mutoh $
  $Date: 2005/09/28 17:47:56 $

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include <locale.h>
#include "global.h"
#include "rbglib.h"

VALUE mGLib;

extern void Init_utils_int64();
extern void Init_gutil();
extern void Init_glib_error();
extern void Init_glib_threads();
extern void Init_glib_convert();
extern void Init_glib_messages();
extern void Init_glib_spawn();
extern void Init_glib_fileutils();
extern void Init_glib_utils();
extern void Init_gobject();
extern void Init_glib_main_loop();
extern void Init_glib_main_context();
extern void Init_glib_source();
extern void Init_glib_poll_fd();
extern void Init_glib_io_channel();

char *
rbg_string_value_ptr(ptr)
    volatile VALUE *ptr;
{
    VALUE s = *ptr;
    if (TYPE(s) != T_STRING) {
        s = rb_str_to_str(s);
        *ptr = s;
    }
    if (!RSTRING(s)->ptr) {
        rb_str_modify(s);
    }
    return RSTRING(s)->ptr;
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

void Init_glib2()
{
    mGLib = rb_define_module("GLib");

    setlocale (LC_CTYPE, "");
#if LC_MESSAGES
    setlocale (LC_MESSAGES, "");
#endif    
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

    /* From "The Main Event Loop" */
    rb_define_const(mGLib, "PRIORITY_HIGH", INT2FIX(G_PRIORITY_HIGH));
    rb_define_const(mGLib, "PRIORITY_DEFAULT", INT2FIX(G_PRIORITY_DEFAULT));
    rb_define_const(mGLib, "PRIORITY_HIGH_IDLE", INT2FIX(G_PRIORITY_HIGH_IDLE));
    rb_define_const(mGLib, "PRIORITY_DEFAULT_IDLE", INT2FIX(G_PRIORITY_DEFAULT_IDLE));
    rb_define_const(mGLib, "PRIORITY_LOW", INT2FIX(G_PRIORITY_LOW));

/*    Init_mem(); */
    Init_utils_int64();
    Init_gutil();
    Init_glib_error();
    Init_glib_threads();
    Init_glib_convert();
    Init_glib_messages();
    Init_glib_spawn();
    Init_glib_fileutils();
    Init_glib_utils();

    Init_gobject();

    /* Require GBoxed/GObject */
    Init_glib_main_loop();
    Init_glib_main_context();
    Init_glib_source();
    Init_glib_poll_fd();
    Init_glib_io_channel();

    /* This is called here once. */
    G_DEF_SETTERS(mGLib);
}
