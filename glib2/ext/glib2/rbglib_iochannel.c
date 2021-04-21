/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

static ID id_call;
static ID id_puts;
static ID id_unpack;

static VALUE default_rs;

static VALUE rb_mIOChannelSource;

#define RG_TARGET_NAMESPACE cIOChannel
#define _SELF(s) ((GIOChannel*)RVAL2BOXED(s, G_TYPE_IO_CHANNEL))

static void
ioc_error(GIOStatus status, GError *err)
{
    if (err != NULL) RAISE_GERROR(err);

    if (status == G_IO_STATUS_EOF){
        rb_raise(rb_eEOFError, "End of file reached");
    } else if (status == G_IO_STATUS_AGAIN){
        rb_raise(rb_eRuntimeError, "G_IO_STATUS_AGAIN");
    } else if (status == G_IO_STATUS_NORMAL){
        /* Do nothing */
    } else {
        rb_raise(rb_eRuntimeError, "An error occurred. status = %d\n", status);
    }
}

static VALUE
rg_initialize(gint argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2;

    GIOChannel* io = NULL;
    rb_scan_args(argc, argv, "11", &arg1, &arg2);

    if (TYPE(arg1) != T_STRING){
        gint fd;
        if (TYPE(arg1) == T_FIXNUM){
            fd = NUM2INT(arg1);
        } else {
            fd = NUM2INT(rb_funcall(arg1, rb_intern("to_i"), 0));
        }
#ifdef G_OS_UNIX
        io = g_io_channel_unix_new(fd);
#elif defined(G_OS_WIN32)
        io = g_io_channel_win32_new_fd(fd);
#else
        rb_raise(rb_eRuntimeError, "GLib::IOChannel.new(fd) is supported on UNIX environment only");
#endif
    } else {
        GError* err = NULL;
        io = g_io_channel_new_file(RVAL2CSTR(arg1), 
                                   NIL_P(arg2) ? "r" : RVAL2CSTR(arg2), &err);

        if (err != NULL) RAISE_GERROR(err);
    }

    G_INITIALIZE(self, io);

    return Qnil;
}

static VALUE
ioc_shutdown(VALUE self)
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_shutdown(_SELF(self), TRUE, &err);

    ioc_error(status, err);
    return self;
}

static VALUE
rg_s_open(gint argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE arg1, arg2;
    VALUE rio;
    GIOChannel* io = NULL;

    rb_scan_args(argc, argv, "11", &arg1, &arg2);

    if (TYPE(arg1) == T_FIXNUM){
#ifdef G_OS_UNIX
        io = g_io_channel_unix_new(NUM2INT(arg1));
#elif defined(G_OS_WIN32)
        io = g_io_channel_win32_new_fd(NUM2INT(arg1));
#else
        rb_raise(rb_eRuntimeError,
                 "GLib::IOChannel.new(fd) is supported on "
                 "UNIX and Windows environment only");
#endif
    } else {
        GError* err = NULL;
        io = g_io_channel_new_file(RVAL2CSTR(arg1),
                                   NIL_P(arg2) ? "r" : RVAL2CSTR(arg2), &err);

        if (err != NULL) RAISE_GERROR(err);
    }

    rio = BOXED2RVAL(io, G_TYPE_IO_CHANNEL);

    if (rb_block_given_p()) {
        return rb_ensure(rb_yield, rio, ioc_shutdown, rio);
    }
    return rio;
}

static VALUE
rg_fileno(VALUE self)
{
#ifdef G_OS_UNIX
    return INT2NUM(g_io_channel_unix_get_fd(_SELF(self)));
#elif defined(G_OS_WIN32)
    return INT2NUM(g_io_channel_win32_get_fd(_SELF(self)));
#else
    rb_warn("GLib::IOChannel#fd is supported on "
            "UNIX and Windows environment only.");
    return Qnil;
#endif
}

/* Don't need this
void        g_io_channel_init               (GIOChannel *channel);
*/

static VALUE
rg_read(gint argc, VALUE *argv, VALUE self)
{
    VALUE rbcount;
    gsize count;
    gchar *buffer;
    gsize bytes_read;
    GIOChannel *channel = _SELF(self);
    GError *error = NULL;
    GIOStatus status;

    rb_scan_args(argc, argv, "01", &rbcount);

    if (NIL_P(rbcount)) {
        status = g_io_channel_read_to_end(channel, &buffer, &bytes_read, &error);
        ioc_error(status, error);

        return buffer != NULL ? CSTR2RVAL_LEN_FREE(buffer, bytes_read) : CSTR2RVAL("");
    }

    count = NUM2UINT(rbcount);

    buffer = g_new(gchar, count);
    memset(buffer, '\0', count);

    status = g_io_channel_read_chars(channel, buffer, count, &bytes_read, &error);
    if (status == G_IO_STATUS_NORMAL)
        return CSTR2RVAL_LEN_FREE(buffer, bytes_read);
    else if (status == G_IO_STATUS_EOF)
        return CSTR2RVAL("");

    ioc_error(status, error);

    /* Not reached. */
    return Qnil;
}

static VALUE
rg_readchar(VALUE self)
{
    gunichar thechar;
    GError* err = NULL;
    GIOStatus status = g_io_channel_read_unichar(_SELF(self), &thechar, &err);

    ioc_error(status, err);

    return UINT2NUM(thechar);
}

static VALUE
rg_getc(VALUE self)
{
    gunichar thechar;
    GError* err = NULL;
    VALUE ret;
    GIOStatus status = g_io_channel_read_unichar(_SELF(self), &thechar, &err);

    if (status == G_IO_STATUS_EOF){
        ret = Qnil;
    } else {
        ioc_error(status, err);
        ret = UINT2NUM(thechar);
    }

    return ret;
}

static VALUE
rg_each_char(VALUE self)
{
    if (!rb_block_given_p()) {
        rb_raise(rb_eArgError, "called without a block");
    }

    while (TRUE){
        gunichar thechar;
        GError* err = NULL;
        GIOStatus status = g_io_channel_read_unichar(_SELF(self), &thechar, &err);
        if (status == G_IO_STATUS_EOF){
            break;
        } else {
            ioc_error(status, err);
            rb_yield(UINT2NUM(thechar));
        }
    }
    return self;
}

static VALUE
rg_readline(gint argc, VALUE *argv, VALUE self)
{
    gchar* str;
    VALUE line_term, ret;
    GIOStatus status;
    GError* err = NULL;

    const gchar* old_line_term = NULL;
    gint old_line_term_len;

    rb_scan_args(argc, argv, "01", &line_term);

    if (!NIL_P(line_term)) {
        StringValue(line_term);
        old_line_term = g_io_channel_get_line_term(_SELF(self), &old_line_term_len);

        g_io_channel_set_line_term(_SELF(self),
                                   StringValuePtr(line_term),
                                   (gint)RSTRING_LEN(line_term));
    }

    status = g_io_channel_read_line(_SELF(self), &str, NULL, NULL, &err);

    if (!NIL_P(line_term)) {
        g_io_channel_set_line_term(_SELF(self), old_line_term, old_line_term_len);
    }

    ioc_error(status, err);

    ret = str ? CSTR2RVAL(str) : CSTR2RVAL("");
    g_free(str);

    return ret;
}

static VALUE
rg_gets(gint argc, VALUE *argv, VALUE self)
{
    gchar* str;
    VALUE line_term, ret;
    GIOStatus status;
    GError* err = NULL;

    const gchar* old_line_term = NULL;
    gint old_line_term_len;

    rb_scan_args(argc, argv, "01", &line_term);

    if (!NIL_P(line_term)) {
        StringValue(line_term);

        old_line_term = g_io_channel_get_line_term(_SELF(self), &old_line_term_len);
        g_io_channel_set_line_term(_SELF(self),
                                   StringValuePtr(line_term),
                                   (gint)RSTRING_LEN(line_term));
    }

    status = g_io_channel_read_line(_SELF(self), &str, NULL, NULL, &err);

    if (! NIL_P(line_term)) {
        g_io_channel_set_line_term(_SELF(self), old_line_term, old_line_term_len);
    }

    if (status == G_IO_STATUS_EOF){
        ret = Qnil;
    } else {
        ioc_error(status, err);
        ret = str ? CSTR2RVAL(str) : CSTR2RVAL("");
    }
    g_free(str);

    return ret;
}

/* Internal use only */
static VALUE
ioc_set_line_term(VALUE args)
{
    VALUE self = RARRAY_PTR(args)[0];
    VALUE doit = RARRAY_PTR(args)[1];
    VALUE line_term = RARRAY_PTR(args)[2];

    if (doit == Qtrue){
        StringValue(line_term);
        g_io_channel_set_line_term(_SELF(self),
                                   StringValuePtr(line_term),
                                   (gint)RSTRING_LEN(line_term));
    }
    return self;
}

static VALUE
rg_each(gint argc, VALUE *argv, VALUE self)
{
    gchar* str;
    VALUE line_term;
    GIOStatus status;
    GError* err = NULL;
    GIOChannel *channel;
    const gchar* old_line_term = NULL;
    gint old_line_term_len;

    if (!rb_block_given_p()) {
        rb_raise(rb_eArgError, "called without a block");
    }

    rb_scan_args(argc, argv, "01", &line_term);

    channel = _SELF(self);
    if (!NIL_P(line_term)) {
        StringValue(line_term);

        old_line_term = g_io_channel_get_line_term(channel, &old_line_term_len);
        g_io_channel_set_line_term(channel,
                                   StringValuePtr(line_term),
                                   (gint)RSTRING_LEN(line_term));
    }

    while (TRUE) {
        status = g_io_channel_read_line(channel, &str, NULL, NULL, &err);
        if (status == G_IO_STATUS_EOF) {
            break;
        } else {
            VALUE rstr;
            ioc_error(status, err);
            if (str) {
                rstr = CSTR2RVAL(str);
            } else {
                rstr = CSTR2RVAL("");
            }
            g_free(str);
            rb_ensure(rb_yield, rstr, ioc_set_line_term,
                      rb_ary_new3(3, self,
                                  NIL_P(line_term) ? Qfalse :  Qtrue,
                                  CSTR2RVAL(old_line_term)));
        }
    }
    return self;
}

/* Don't need this.
GIOStatus   g_io_channel_read_line_string   (GIOChannel *channel,
                                             GString *buffer,
                                             gsize *terminator_pos,
                                             GError **error);
*/

/* Use GLib::IOChannel#read instead.
static VALUE
ioc_read_to_end(VALUE self)
{
    gchar* str;
    gsize length;
    VALUE ret;
    GError* err = NULL;

    GIOStatus status = g_io_channel_read_to_end(_SELF(self), &str,
                                                &length, &err);

    ioc_error(status, err);

    ret = str ? rb_str_new(str, length) : CSTR2RVAL("");
    g_free(str);

    return ret;
}
*/

static VALUE
rg_write(VALUE self, VALUE buf)
{
    gssize count;
    gsize bytes_written;
    GIOStatus status;
    GError* err = NULL;

    buf = rb_obj_as_string(buf);

    StringValue(buf);

    count = RSTRING_LEN(buf);

    status = g_io_channel_write_chars(_SELF(self), RVAL2CSTR(buf), count, &bytes_written, &err);

    ioc_error(status, err);
    return UINT2NUM(bytes_written);
}

static VALUE
rg_putc(VALUE self, VALUE thechar)
{
    GError* err = NULL;
    GIOStatus status;
    gunichar unichar;

    if (TYPE(thechar) == T_FIXNUM) {
        unichar = NUM2UINT(thechar);
    } else {
        VALUE ary = rb_funcall(thechar, id_unpack, 1, CSTR2RVAL("U"));
        unichar = NUM2UINT(RARRAY_PTR(ary)[0]);
    }

    status = g_io_channel_write_unichar(_SELF(self), unichar, &err);

    ioc_error(status, err);

    return self;
}

static VALUE
rg_flush(VALUE self)
{
    GError* err = NULL;
    GIOStatus status =  g_io_channel_flush(_SELF(self), &err);
    ioc_error(status, err);
    return self;
}

static VALUE
rg_seek(gint argc, VALUE *argv, VALUE self)
{
    VALUE ofs, type;
    GIOStatus status;
    GError* err = NULL;
    GSeekType gtype = G_SEEK_SET;

    rb_scan_args(argc, argv, "11", &ofs, &type);

    if (!NIL_P(type))
        gtype = NUM2INT(type);

    status = g_io_channel_seek_position(_SELF(self), NUM2INT(ofs),
                                        gtype, &err);
    ioc_error(status, err);
    return self;
}

static VALUE
rg_set_pos(VALUE self, VALUE pos)
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_seek_position(_SELF(self), NUM2INT(pos),
                                                  G_SEEK_SET, &err);
    ioc_error(status, err);
    return self;
}

static VALUE
rg_close(gint argc, VALUE *argv, VALUE self)
{
    VALUE flush;
    GError* err = NULL;
    gboolean gflush = TRUE;
    GIOStatus status;

    rb_scan_args(argc, argv, "01", &flush);

    if (!NIL_P(flush)){
        gflush = RVAL2CBOOL(flush);
    }

    status = g_io_channel_shutdown(_SELF(self), gflush, &err);
    ioc_error(status, err);

    return self;
}

static VALUE
rg_create_watch(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_condition;
    VALUE rb_block;
    VALUE rb_source;

    rb_scan_args(argc, argv, "1&", &rb_condition, &rb_block);
    rb_source = BOXED2RVAL(g_io_create_watch(_SELF(self), NUM2INT(rb_condition)),
                           G_TYPE_SOURCE);
    rb_extend_object(rb_source, rb_mIOChannelSource);
    if (!NIL_P(rb_block)) {
        ID id_set_callback;
        CONST_ID(id_set_callback, "set_callback");
        rb_funcall_with_block(rb_source, id_set_callback, 0, NULL, rb_block);
    }
    return rb_source;
}

static gboolean
io_func(GIOChannel *source, GIOCondition condition, gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 2, 
                            BOXED2RVAL(source, G_TYPE_IO_CHANNEL), 
                            INT2NUM(condition)));
}

static VALUE
rg_add_watch(VALUE self, VALUE condition)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    return UINT2NUM(g_io_add_watch(_SELF(self), NUM2INT(condition), 
                                   (GIOFunc)io_func, (gpointer)func));
}

/* Don't need this
guint       g_io_add_watch_full             (GIOChannel *channel,
                                             gint priority,
                                             GIOCondition condition,
                                             GIOFunc func,
                                             gpointer user_data,
                                             GDestroyNotify notify);
*/

static VALUE
rg_io_channel_source_set_callback(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_callback;

    rb_scan_args(argc, argv, "&", &rb_callback);
    G_RELATIVE(self, rb_callback);
    g_source_set_callback(RVAL2BOXED(self, G_TYPE_SOURCE),
                          (GSourceFunc)io_func,
                          (gpointer)rb_callback,
                          (GDestroyNotify)NULL);
    return self;
}

static VALUE
rg_buffer_size(VALUE self)
{
    return UINT2NUM(g_io_channel_get_buffer_size(_SELF(self)));
}

static VALUE
rg_set_buffer_size(VALUE self, VALUE buffer_size)
{
    g_io_channel_set_buffer_size(_SELF(self), NUM2UINT(buffer_size));
    return self;
}

static VALUE
rg_buffer_condition(VALUE self)
{
    return INT2NUM(g_io_channel_get_buffer_condition(_SELF(self)));
}

static VALUE
rg_flags(VALUE self)
{
    return INT2NUM(g_io_channel_get_flags(_SELF(self)));
}

static VALUE
rg_set_flags(VALUE self, VALUE flags)
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_set_flags(_SELF(self), 
                                              NUM2INT(flags), &err);
    ioc_error(status, err);
    return self;
}

/* Use them with GLib::IOChannel#gets, #readline, #readlines 
static VALUE
ioc_get_line_term(VALUE self)
{
    gint length;
    const gchar* ret = g_io_channel_get_line_term(_SELF(self), &length);
    if (ret) {
        if (length < 0) {
            return CSTR2RVAL(ret);
        } else {
            return rb_str_new(ret, length);
        }
    }
    return Qnil;
}    

static VALUE
ioc_set_line_term(VALUE self, VALUE line_term)
{
    StringValue(line_term);
    g_io_channel_set_line_term(_SELF(self), RVAL2CSTR(line_term),
                               RSTRING_LEN(line_term));
    return self;
}
*/

static VALUE
rg_buffered(VALUE self)
{
    return CBOOL2RVAL(g_io_channel_get_buffered(_SELF(self)));
}

static VALUE
rg_set_buffered(VALUE self, VALUE buffered)
{
    g_io_channel_set_buffered(_SELF(self), RVAL2CBOOL(buffered));
    return self;
}

static VALUE
rg_encoding(VALUE self)
{
    return CSTR2RVAL(g_io_channel_get_encoding(_SELF(self)));
}

static VALUE
rg_set_encoding(VALUE self, VALUE encoding)
{
    GError* err = NULL;
    GIOStatus status;

    status = g_io_channel_set_encoding(_SELF(self),
                                       RVAL2CSTR_ACCEPT_NIL(encoding),
                                       &err);
    ioc_error(status, err);
    return self;
}

/* Don't we need them ?
gboolean    g_io_channel_get_close_on_unref (GIOChannel *channel);
void        g_io_channel_set_close_on_unref (GIOChannel *channel,
                                             gboolean do_close);
*/

/* Deprecated
GIOError    g_io_channel_read               (GIOChannel *channel,
                                             gchar *buf,
                                             gsize count,
                                             gsize *bytes_read);
enum        GIOError;
GIOError    g_io_channel_write              (GIOChannel *channel,
                                             const gchar *buf,
                                             gsize count,
                                             gsize *bytes_written);
GIOError    g_io_channel_seek               (GIOChannel *channel,
                                             gint64 offset,
                                             GSeekType type);
void        g_io_channel_close              (GIOChannel *channel);
*/

/* 
 * Stolen some convenient methods from io.c
 */
static VALUE
rg_printf(int argc, VALUE *argv, VALUE self)
{
    rg_write(self, rb_f_sprintf(argc, argv));
    return Qnil;
}

static VALUE
ioc_puts_ary(VALUE ary, VALUE out, int recur)
{
    VALUE tmp;
    long i;

    for (i=0; i<RARRAY_LEN(ary); i++) {
        tmp = RARRAY_PTR(ary)[i];
        if (recur) {
            tmp = rb_str_new2("[...]");
        }
        rb_funcall(out, id_puts, 1, tmp);
    }
    return Qnil;
}

static VALUE
rg_puts(int argc, VALUE *argv, VALUE self)
{
    int i;
    VALUE line;

    /* if no argument given, print newline. */
    if (argc == 0) {
        rg_write(self, default_rs);
        return Qnil;
    }
    for (i=0; i<argc; i++) {
        if (NIL_P(argv[i])) {
            line = rb_str_new2("nil");
        }
        else {
          line = rbg_check_array_type(argv[i]);
          if (!NIL_P(line)) {
#ifdef HAVE_RB_EXEC_RECURSIVE
            rb_exec_recursive(ioc_puts_ary, line, self);
#else
            rb_protect_inspect(ioc_puts_ary, line, self);
#endif
            continue;
          }
          line = rb_obj_as_string(argv[i]);
        }
        rg_write(self, line);
        if (RSTRING_LEN(line) == 0 ||
            RSTRING_PTR(line)[RSTRING_LEN(line)-1] != '\n') {
            rg_write(self, default_rs);
        }
    }

    return Qnil;
}

static VALUE
rg_print(int argc, VALUE *argv, VALUE out)
{
    int i;
    VALUE line;
    VALUE output_field_separator;

    /* if no argument given, print `$_' */
    if (argc == 0) {
        argc = 1;
        line = rb_lastline_get();
        argv = &line;
    }

    output_field_separator = rb_gv_get("$,");
    for (i=0; i<argc; i++) {
        if (!NIL_P(output_field_separator) && i>0) {
            rg_write(out, output_field_separator);
        }
        switch (TYPE(argv[i])) {
          case T_NIL:
            rg_write(out, rb_str_new2("nil"));
            break;
          default:
            rg_write(out, argv[i]);
            break;
        }
    }
    if (!NIL_P(output_field_separator)) {
        rg_write(out, output_field_separator);
    }

    return Qnil;
}

void
Init_glib_io_channel(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_IO_CHANNEL, "IOChannel", mGLib); 

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    id_call = rb_intern("call");
    id_puts = rb_intern("puts");
    id_unpack = rb_intern("unpack");

    default_rs = rb_str_new_cstr("\n");
#ifdef HAVE_RB_GC_REGISTER_MARK_OBJECT
    rb_gc_register_mark_object(default_rs);
#else
    rb_global_variable(&default_rs);
#endif

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_SMETHOD(open, -1);
    RG_DEF_METHOD(fileno, 0);
    RG_DEF_ALIAS("to_i", "fileno");
    RG_DEF_METHOD(read, -1);
    RG_DEF_METHOD(readchar, 0);
    RG_DEF_METHOD(getc, 0);
    RG_DEF_METHOD(readline, -1);
    RG_DEF_METHOD(gets, -1);
    RG_DEF_METHOD(each, -1);
    RG_DEF_ALIAS("each_line", "each");
    RG_DEF_METHOD(each_char, 0);
    RG_DEF_METHOD(write, 1);
    RG_DEF_METHOD(printf, -1);
    RG_DEF_METHOD(print, -1);
    RG_DEF_METHOD(puts, -1);
    RG_DEF_METHOD(putc, 1);
    RG_DEF_METHOD(flush, 0);
    RG_DEF_METHOD(seek, -1);
    RG_DEF_METHOD(set_pos, 1);
    RG_DEF_METHOD(close, -1);
    RG_DEF_METHOD(create_watch, -1);
    RG_DEF_METHOD(add_watch, 1);
    RG_DEF_METHOD(buffer_size, 0);
    RG_DEF_METHOD(set_buffer_size, 1);
    RG_DEF_METHOD(buffer_condition, 0);
    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(set_flags, 1);
    RG_DEF_METHOD(buffered, 0);
    RG_DEF_METHOD(set_buffered, 1);
    RG_DEF_METHOD(encoding, 0);
    RG_DEF_METHOD(set_encoding, 1);

    /* GSeekType */
    rb_define_const(RG_TARGET_NAMESPACE, "SEEK_CUR", INT2NUM(G_SEEK_CUR));
    rb_define_const(RG_TARGET_NAMESPACE, "SEEK_SET", INT2NUM(G_SEEK_SET));
    rb_define_const(RG_TARGET_NAMESPACE, "SEEK_END", INT2NUM(G_SEEK_END));

    /* GIOStatus */
    rb_define_const(RG_TARGET_NAMESPACE, "STATUS_ERROR", INT2NUM(G_IO_STATUS_ERROR));
    rb_define_const(RG_TARGET_NAMESPACE, "STATUS_NORMAL", INT2NUM(G_IO_STATUS_NORMAL));
    rb_define_const(RG_TARGET_NAMESPACE, "STATUS_EOF", INT2NUM(G_IO_STATUS_EOF));
    rb_define_const(RG_TARGET_NAMESPACE, "STATUS_AGAIN", INT2NUM(G_IO_STATUS_AGAIN));

    /* GIOCondition */
    /* Deprecated. Just for bacakward compatibility. Use
     * GLib::IOCondition::* instead. */
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_IO_CONDITION, "G_IO_");

    /* GIOFlags */
    rb_define_const(RG_TARGET_NAMESPACE, "FLAG_APPEND", INT2NUM(G_IO_FLAG_APPEND));
    rb_define_const(RG_TARGET_NAMESPACE, "FLAG_NONBLOCK", INT2NUM(G_IO_FLAG_NONBLOCK));
    rb_define_const(RG_TARGET_NAMESPACE, "FLAG_READABLE", INT2NUM(G_IO_FLAG_IS_READABLE));
    rb_define_const(RG_TARGET_NAMESPACE, "FLAG_WRITEABLE", INT2NUM(G_IO_FLAG_IS_WRITEABLE));
    rb_define_const(RG_TARGET_NAMESPACE, "FLAG_IS_SEEKABLE", INT2NUM(G_IO_FLAG_IS_SEEKABLE));
    rb_define_const(RG_TARGET_NAMESPACE, "FLAG_MASK", INT2NUM(G_IO_FLAG_MASK));
    rb_define_const(RG_TARGET_NAMESPACE, "FLAG_GET_MASK", INT2NUM(G_IO_FLAG_GET_MASK));
    rb_define_const(RG_TARGET_NAMESPACE, "FLAG_SET_MASK", INT2NUM(G_IO_FLAG_SET_MASK));

    rb_mIOChannelSource = rb_define_module_under(mGLib, "IOChannelSource");
    rb_define_method(rb_mIOChannelSource,
                     "set_callback",
                     rg_io_channel_source_set_callback,
                     -1);
}
