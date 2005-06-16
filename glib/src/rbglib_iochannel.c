/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbglib_iochannel.c -

  $Author: mutoh $
  $Date: 2005/06/16 13:27:33 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

static ID id_call;

#define _SELF(s) ((GIOChannel*)RVAL2BOXED(s, G_TYPE_IO_CHANNEL))

static void
ioc_error(status, err)
    GIOStatus status;
    GError* err;
{
    if (err != NULL) RAISE_GERROR(err);

    if (status == G_IO_STATUS_EOF){
        rb_raise(rb_eEOFError, "End of file reached");
    } else if (status == G_IO_STATUS_AGAIN){
        rb_raise(rb_eRuntimeError, "G_IO_STATUS_AGAIN");
    } else if (status == G_IO_STATUS_NORMAL){
        /* Do nothing */
    } else {
        rb_raise(rb_eRuntimeError, "An error occured.");
    }
}

static VALUE
ioc_initialize(argc, argv, self)
    gint argc;
    VALUE* argv;
    VALUE self;
{
    VALUE arg1, arg2;

    GIOChannel* io = NULL;

    rb_scan_args(argc, argv, "11", &arg1, &arg2);

    if (TYPE(arg1) == T_FIXNUM){
#ifdef G_OS_UNIX
        io = g_io_channel_unix_new(NUM2INT(arg1));
#else
        rb_raise("GLib::IOChannel.new(fd) is supported on UNIX environment only");
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
ioc_get_fd(self)
    VALUE self;
{
#ifdef G_OS_UNIX
    return INT2NUM(g_io_channel_unix_get_fd(_SELF(self)));
#else
    rb_warn("GLib::IOChannel#fd is supported on UNIX environment only.");
    return Qnil;
#endif
}

/* Don't need this
void        g_io_channel_init               (GIOChannel *channel);
*/


static VALUE
ioc_read_chars(self, count)
    VALUE self, count;
{
    gchar* buf = NULL;
    gsize bytes_read;
    GError* err = NULL;
    GIOStatus status = g_io_channel_read_chars(_SELF(self), buf, NUM2UINT(count),
                                                &bytes_read, &err);
    ioc_error(status, err);
    return buf ? CSTR2RVAL(buf) : CSTR2RVAL("");
}

static VALUE
ioc_read_unichar(self)
    VALUE self;
{
    gunichar thechar;
    GError* err = NULL;
    GIOStatus status = g_io_channel_read_unichar(_SELF(self), &thechar, &err);

    ioc_error(status, err);

    return UINT2NUM(thechar);
}

static VALUE
ioc_read_line(self)
    VALUE self;
{
    gchar* str;
    GError* err = NULL;
    VALUE ret;
    GIOStatus status = g_io_channel_read_line(_SELF(self), &str, NULL, NULL, &err);

    ioc_error(status, err);

    ret = str ? CSTR2RVAL(str) : CSTR2RVAL("");
    g_free(str);

    return ret;
}

/* Don't need this.
GIOStatus   g_io_channel_read_line_string   (GIOChannel *channel,
                                             GString *buffer,
                                             gsize *terminator_pos,
                                             GError **error);
*/

static VALUE
ioc_read_to_end(self)
    VALUE self;
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

static VALUE
ioc_write_chars(self, buf)
    VALUE self, buf;
{
    gssize count;
    gsize bytes_written;
    GIOStatus status;
    GError* err = NULL;

    StringValue(buf);

    count = RSTRING(buf)->len;

    status = g_io_channel_write_chars(_SELF(self), RVAL2CSTR(buf), count, &bytes_written, &err);

    ioc_error(status, err);
    return self;
}

static VALUE
ioc_write_unichar(self, thechar)
    VALUE self, thechar;
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_write_unichar(_SELF(self), 
                                                  NUM2UINT(thechar), &err);
    ioc_error(status, err);
    return self;
}

static VALUE
ioc_flush(self)
    VALUE self;
{
    GError* err = NULL;
    GIOStatus status =  g_io_channel_flush(_SELF(self), &err);
    ioc_error(status, err);
    return self;
}

static VALUE
ioc_seek(self, ofs, type)
    VALUE self, ofs, type;
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_seek_position(_SELF(self), NUM2INT(ofs),
                                                  NUM2INT(type), &err);
    ioc_error(status, err);
    return self;
}

static VALUE
ioc_shutdown(argc, argv, self)
    gint argc;
    VALUE* argv;
    VALUE self;
{
    VALUE flush;
    GError* err = NULL;
    gboolean gflush = TRUE;
    rb_scan_args(argc, argv, "01", &flush);

    if (!NIL_P(flush)){
        gflush = RTEST(flush);
    }

    GIOStatus status = g_io_channel_shutdown(_SELF(self), gflush, &err);
    ioc_error(status, err);

    return self;
}

static VALUE
ioc_create_watch(self, condition)
    VALUE self, condition;
{
    return BOXED2RVAL(g_io_create_watch(_SELF(self), NUM2INT(condition)), 
                      G_TYPE_SOURCE);
}

static gboolean
io_func(source, condition, func)
    GIOChannel* source;
    GIOCondition condition;
    gpointer func;
{
    return RTEST(rb_funcall((VALUE)func, id_call, 2, 
                            BOXED2RVAL(source, G_TYPE_IO_CHANNEL), 
                            INT2NUM(condition)));
}

static VALUE
ioc_add_watch(self, condition)
    VALUE self, condition;
{
    VALUE func = G_BLOCK_PROC();
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
ioc_get_buffer_size(self)
    VALUE self;
{
    return UINT2NUM(g_io_channel_get_buffer_size(_SELF(self)));
}

static VALUE
ioc_set_buffer_size(self, buffer_size)
    VALUE self, buffer_size;
{
    g_io_channel_set_buffer_size(_SELF(self), NUM2UINT(buffer_size));
    return self;
}

static VALUE
ioc_get_buffer_condition(self)
    VALUE self;
{
    return INT2NUM(g_io_channel_get_buffer_condition(_SELF(self)));
}

static VALUE
ioc_get_flags(self)
    VALUE self;
{
    return INT2NUM(g_io_channel_get_flags(_SELF(self)));
}

static VALUE
ioc_set_flags(self, flags)
    VALUE self, flags;
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_set_flags(_SELF(self), 
                                              NUM2INT(flags), &err);
    ioc_error(status, err);
    return self;
}

static VALUE
ioc_get_line_term(self)
    VALUE self;
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
ioc_set_line_term(self, line_term)
    VALUE self, line_term;
{
    StringValue(line_term);
    g_io_channel_set_line_term(_SELF(self), RVAL2CSTR(line_term),
                               RSTRING(line_term)->len);
    return self;
}

static VALUE
ioc_get_buffered(self)
    VALUE self;
{
    return CBOOL2RVAL(g_io_channel_get_buffered(_SELF(self)));
}

static VALUE
ioc_set_buffered(self, buffered)
    VALUE self, buffered;
{
    g_io_channel_set_buffered(_SELF(self), RTEST(buffered));
    return self;
}

static VALUE
ioc_get_encoding(self)
    VALUE self;
{
    return CSTR2RVAL(g_io_channel_get_encoding(_SELF(self)));
}

static VALUE
ioc_set_encoding(self, encoding)
    VALUE self, encoding;
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_set_encoding(_SELF(self), 
                                                 RVAL2CSTR(encoding), &err);
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

static VALUE
ioc_error_s_from_errno(self, errno_)
    VALUE self, errno_;
{
    return INT2NUM(g_io_channel_error_from_errno(NUM2INT(errno_)));
}

void
Init_glib_io_channel()
{
    VALUE io = G_DEF_CLASS(G_TYPE_IO_CHANNEL, "IOChannel", mGLib); 

    id_call = rb_intern("call");

    rb_define_method(io, "initialize", ioc_initialize, -1);
    rb_define_method(io, "fd", ioc_get_fd, 0);
    rb_define_method(io, "read_chars", ioc_read_chars, 1);
    rb_define_method(io, "read_unichar", ioc_read_unichar, 0);
    rb_define_method(io, "read_line", ioc_read_line, 0);
    rb_define_method(io, "read_to_end", ioc_read_to_end, 0);
    rb_define_method(io, "write_chars", ioc_write_chars, 1);
    rb_define_method(io, "write_unichar", ioc_write_unichar, 1);
    rb_define_method(io, "flush", ioc_flush, 0);
    rb_define_method(io, "seek", ioc_seek, 2);
    rb_define_method(io, "shutdown", ioc_shutdown, -1);
    rb_define_method(io, "create_watch", ioc_create_watch, 1);
    rb_define_method(io, "add_watch", ioc_add_watch, 1);
    rb_define_method(io, "buffer_size", ioc_get_buffer_size, 0);
    rb_define_method(io, "set_buffer_size", ioc_set_buffer_size, 1);
    rb_define_method(io, "buffer_condition", ioc_get_buffer_condition, 0);
    rb_define_method(io, "flags", ioc_get_flags, 0);
    rb_define_method(io, "set_flags", ioc_set_flags, 1);
    rb_define_method(io, "line_term", ioc_get_line_term, 0);
    rb_define_method(io, "set_line_term", ioc_set_line_term, 1);
    rb_define_method(io, "buffered", ioc_get_buffered, 0);
    rb_define_method(io, "set_buffered", ioc_set_buffered, 1);
    rb_define_method(io, "encoding", ioc_get_encoding, 0);
    rb_define_method(io, "set_encoding", ioc_set_encoding, 1);

    /* GSeekType */
    rb_define_const(io, "SEEK_CUR", INT2NUM(G_SEEK_CUR));
    rb_define_const(io, "SEEK_SET", INT2NUM(G_SEEK_SET));
    rb_define_const(io, "SEEK_END", INT2NUM(G_SEEK_END));
    
    /* GIOStatus */
    rb_define_const(io, "STATUS_ERROR", INT2NUM(G_IO_STATUS_ERROR));
    rb_define_const(io, "STATUS_NORMAL", INT2NUM(G_IO_STATUS_NORMAL));
    rb_define_const(io, "STATUS_EOF", INT2NUM(G_IO_STATUS_EOF));
    rb_define_const(io, "STATUS_AGAIN", INT2NUM(G_IO_STATUS_AGAIN));

    /* GIOCondition */
    rb_define_const(io, "IN", INT2NUM(G_IO_IN));
    rb_define_const(io, "OUT", INT2NUM(G_IO_OUT));
    rb_define_const(io, "PRI", INT2NUM(G_IO_PRI));
    rb_define_const(io, "ERR", INT2NUM(G_IO_ERR));
    rb_define_const(io, "HUP", INT2NUM(G_IO_HUP));
    rb_define_const(io, "NVAL", INT2NUM(G_IO_NVAL));

    /* GIOFlags */
    rb_define_const(io, "FLAG_APPEND", INT2NUM(G_IO_FLAG_APPEND));
    rb_define_const(io, "FLAG_NONBLOCK", INT2NUM(G_IO_FLAG_NONBLOCK));
    rb_define_const(io, "FLAG_READABLE", INT2NUM(G_IO_FLAG_IS_READABLE));
    rb_define_const(io, "FLAG_WRITEABLE", INT2NUM(G_IO_FLAG_IS_WRITEABLE));
    rb_define_const(io, "FLAG_IS_SEEKABLE", INT2NUM(G_IO_FLAG_IS_SEEKABLE));
    rb_define_const(io, "FLAG_MASK", INT2NUM(G_IO_FLAG_MASK));
    rb_define_const(io, "FLAG_GET_MASK", INT2NUM(G_IO_FLAG_GET_MASK));
    rb_define_const(io, "FLAG_SET_MASK", INT2NUM(G_IO_FLAG_SET_MASK));

    /* GIOChannelError */
    VALUE ioc_error = G_DEF_ERROR2(G_IO_CHANNEL_ERROR, "IOChannelError", mGLib, rb_eIOError);
    rb_define_singleton_method(ioc_error, "from_errno", ioc_error_s_from_errno, 1);

    rb_define_const(ioc_error, "FBIG", INT2NUM(G_IO_CHANNEL_ERROR_FBIG));
    rb_define_const(ioc_error, "INVAL", INT2NUM(G_IO_CHANNEL_ERROR_INVAL));
    rb_define_const(ioc_error, "IO", INT2NUM(G_IO_CHANNEL_ERROR_IO));
    rb_define_const(ioc_error, "ISDIR", INT2NUM(G_IO_CHANNEL_ERROR_ISDIR));
    rb_define_const(ioc_error, "NOSPC", INT2NUM(G_IO_CHANNEL_ERROR_NOSPC));
    rb_define_const(ioc_error, "NXIO", INT2NUM(G_IO_CHANNEL_ERROR_NXIO));
    rb_define_const(ioc_error, "OVERFLOW", INT2NUM(G_IO_CHANNEL_ERROR_OVERFLOW));
    rb_define_const(ioc_error, "PIPE", INT2NUM(G_IO_CHANNEL_ERROR_PIPE));
    rb_define_const(ioc_error, "FAILED", INT2NUM(G_IO_CHANNEL_ERROR_FAILED));

}
