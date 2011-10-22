/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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
        rb_raise(rb_eRuntimeError, "An error occured. status = %d\n", status);
    }
}

static VALUE
ioc_initialize(gint argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2;

    GIOChannel* io = NULL;
    rb_secure(4);
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
ioc_close(VALUE self)
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_shutdown(_SELF(self), TRUE, &err);

    ioc_error(status, err);
    return self;
}

static VALUE
ioc_s_open(gint argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
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
        return rb_ensure(rb_yield, rio, ioc_close, rio);
    }
    return rio;
}

static VALUE
ioc_get_fd(VALUE self)
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
ioc_read_chars(gint argc, VALUE *argv, VALUE self)
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
ioc_read_unichar(VALUE self)
{
    gunichar thechar;
    GError* err = NULL;
    GIOStatus status = g_io_channel_read_unichar(_SELF(self), &thechar, &err);

    ioc_error(status, err);

    return UINT2NUM(thechar);
}

static VALUE
ioc_getuc(VALUE self)
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
ioc_each_char(VALUE self)
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
ioc_read_line(gint argc, VALUE *argv, VALUE self)
{
    gchar* str;
    VALUE line_term, ret;
    GIOStatus status;
    GError* err = NULL;

    const gchar* old_line_term = NULL;
    gint old_line_term_len;

    rb_scan_args(argc, argv, "01", &line_term);

    if (! NIL_P(line_term)){
        StringValue(line_term);
        old_line_term = g_io_channel_get_line_term(_SELF(self), &old_line_term_len);

        g_io_channel_set_line_term(_SELF(self), RVAL2CSTR(line_term),
                                   RSTRING_LEN(line_term));   
    }

    status = g_io_channel_read_line(_SELF(self), &str, NULL, NULL, &err);

    if (! NIL_P(line_term)){
        g_io_channel_set_line_term(_SELF(self), old_line_term, old_line_term_len);
    }   

    ioc_error(status, err);
    
    ret = str ? CSTR2RVAL(str) : CSTR2RVAL("");
    g_free(str);

    return ret;
}

static VALUE
ioc_gets(gint argc, VALUE *argv, VALUE self)
{
    gchar* str;
    VALUE line_term, ret;
    GIOStatus status;
    GError* err = NULL;

    const gchar* old_line_term = NULL;
    gint old_line_term_len;

    rb_scan_args(argc, argv, "01", &line_term);

    if (! NIL_P(line_term)){
        StringValue(line_term);

        old_line_term = g_io_channel_get_line_term(_SELF(self), &old_line_term_len);
        g_io_channel_set_line_term(_SELF(self), RVAL2CSTR(line_term),
                                   RSTRING_LEN(line_term));   
    }

    status = g_io_channel_read_line(_SELF(self), &str, NULL, NULL, &err);

    if (! NIL_P(line_term)){
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
        g_io_channel_set_line_term(_SELF(self), RVAL2CSTR(line_term),
                                   RSTRING_LEN(line_term));  
    }
    return self;
}

static VALUE
ioc_each_line(gint argc, VALUE *argv, VALUE self)
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
        g_io_channel_set_line_term(channel, RVAL2CSTR(line_term),
                                   RSTRING_LEN(line_term));
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
ioc_write_chars(VALUE self, VALUE buf)
{
    gssize count;
    gsize bytes_written;
    GIOStatus status;
    GError* err = NULL;

    rb_secure(4);
    buf = rb_obj_as_string(buf);

    StringValue(buf);

    count = RSTRING_LEN(buf);

    status = g_io_channel_write_chars(_SELF(self), RVAL2CSTR(buf), count, &bytes_written, &err);

    ioc_error(status, err);
    return UINT2NUM(bytes_written);
}

static ID id_unpack;

static VALUE
ioc_write_unichar(VALUE self, VALUE thechar)
{
    GError* err = NULL;
    GIOStatus status;
    gunichar unichar;

    rb_secure(4);
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
ioc_flush(VALUE self)
{
    GError* err = NULL;
    GIOStatus status =  g_io_channel_flush(_SELF(self), &err);
    ioc_error(status, err);
    return self;
}

static VALUE
ioc_seek(gint argc, VALUE *argv, VALUE self)
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
ioc_seek_pos(VALUE self, VALUE pos)
{
    GError* err = NULL;
    GIOStatus status = g_io_channel_seek_position(_SELF(self), NUM2INT(pos),
                                                  G_SEEK_SET, &err);
    ioc_error(status, err);
    return self;
}

static VALUE
ioc_shutdown(gint argc, VALUE *argv, VALUE self)
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
ioc_create_watch(VALUE self, VALUE condition)
{
    return BOXED2RVAL(g_io_create_watch(_SELF(self), NUM2INT(condition)), 
                      G_TYPE_SOURCE);
}

static gboolean
io_func(GIOChannel *source, GIOCondition condition, gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 2, 
                            BOXED2RVAL(source, G_TYPE_IO_CHANNEL), 
                            INT2NUM(condition)));
}

static VALUE
ioc_add_watch(VALUE self, VALUE condition)
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
ioc_get_buffer_size(VALUE self)
{
    return UINT2NUM(g_io_channel_get_buffer_size(_SELF(self)));
}

static VALUE
ioc_set_buffer_size(VALUE self, VALUE buffer_size)
{
    g_io_channel_set_buffer_size(_SELF(self), NUM2UINT(buffer_size));
    return self;
}

static VALUE
ioc_get_buffer_condition(VALUE self)
{
    return INT2NUM(g_io_channel_get_buffer_condition(_SELF(self)));
}

static VALUE
ioc_get_flags(VALUE self)
{
    return INT2NUM(g_io_channel_get_flags(_SELF(self)));
}

static VALUE
ioc_set_flags(VALUE self, VALUE flags)
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
ioc_get_buffered(VALUE self)
{
    return CBOOL2RVAL(g_io_channel_get_buffered(_SELF(self)));
}

static VALUE
ioc_set_buffered(VALUE self, VALUE buffered)
{
    g_io_channel_set_buffered(_SELF(self), RVAL2CBOOL(buffered));
    return self;
}

static VALUE
ioc_get_encoding(VALUE self)
{
    return CSTR2RVAL(g_io_channel_get_encoding(_SELF(self)));
}

static VALUE
ioc_set_encoding(VALUE self, VALUE encoding)
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

#ifdef G_OS_WIN32
static VALUE
ioc_win32_socket_initialize(VALUE self, VALUE socket)
{
    GIOChannel *io = NULL;
    int fd;

    rb_secure(4);
    /* TODO: support IO object */
    fd = NUM2INT(socket);
    io = g_io_channel_win32_new_socket(rb_w32_get_osfhandle(fd));
    G_INITIALIZE(self, io);

    return Qnil;
}
#endif

static VALUE
ioc_error_s_from_errno(G_GNUC_UNUSED VALUE self, VALUE errno_)
{
    return INT2NUM(g_io_channel_error_from_errno(NUM2INT(errno_)));
}

/* 
 * Stolen some convenient methods from io.c
 */
static VALUE
ioc_printf(int argc, VALUE *argv, VALUE self)
{
    ioc_write_chars(self, rb_f_sprintf(argc, argv));
    return Qnil;
}

static VALUE ioc_puts(int argc, VALUE* argv, VALUE self);

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
        rb_io_puts(1, &tmp, out);
    }
    return Qnil;
}

static VALUE
ioc_puts(int argc, VALUE *argv, VALUE self)
{
    int i;
    VALUE line;

    /* if no argument given, print newline. */
    if (argc == 0) {
        ioc_write_chars(self, rb_default_rs);
        return Qnil;
    }
    for (i=0; i<argc; i++) {
        if (NIL_P(argv[i])) {
            line = rb_str_new2("nil");
        }
        else {
#ifdef HAVE_RB_CHECK_ARRAY_TYPE
          line = rb_check_array_type(argv[i]);
#else
          line = rb_check_convert_type(argv[i], T_ARRAY, "Array", "to_ary");
#endif
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
        ioc_write_chars(self, line);
        if (RSTRING_LEN(line) == 0 ||
            RSTRING_PTR(line)[RSTRING_LEN(line)-1] != '\n') {
            ioc_write_chars(self, rb_default_rs);
        }
    }

    return Qnil;
}

static VALUE
ioc_print(int argc, VALUE *argv, VALUE out)
{
    int i;
    VALUE line;

    /* if no argument given, print `$_' */
    if (argc == 0) {
        argc = 1;
        line = rb_lastline_get();
        argv = &line;
    }
    for (i=0; i<argc; i++) {
        if (!NIL_P(rb_output_fs) && i>0) {
            ioc_write_chars(out, rb_output_fs);
        }
        switch (TYPE(argv[i])) {
          case T_NIL:
            ioc_write_chars(out, rb_str_new2("nil"));
            break;
          default:
            ioc_write_chars(out, argv[i]);
            break;
        }
    }
    if (!NIL_P(rb_output_rs)) {
        ioc_write_chars(out, rb_output_rs);
    }

    return Qnil;
}


void
Init_glib_io_channel(void)
{
    VALUE io = G_DEF_CLASS(G_TYPE_IO_CHANNEL, "IOChannel", mGLib); 
    VALUE ioc_error = G_DEF_ERROR2(G_IO_CHANNEL_ERROR, "IOChannelError", mGLib, rb_eIOError);

    rb_include_module(io, rb_mEnumerable);

    id_call = rb_intern("call");
    id_unpack = rb_intern("unpack");


    rb_define_method(io, "initialize", ioc_initialize, -1);
    rb_define_singleton_method(io, "open", ioc_s_open, -1);
    rb_define_method(io, "fileno", ioc_get_fd, 0);          /* ref: IO#fileno */
    rb_define_alias(io, "to_i", "fileno");                  /* ref: IO#to_i */        
    rb_define_method(io, "read", ioc_read_chars, -1);       /* ref: IO#read(count = nil) */
    rb_define_method(io, "readchar", ioc_read_unichar, 0);  /* ref: IO#readchar but this return an UTF-8 char */
    rb_define_method(io, "getc", ioc_getuc, 0);             /* ref: IO#getc but this return an UTF-8 char */
    rb_define_method(io, "readline", ioc_read_line, -1);    /* ref: IO#readline(rs = nil) */
    rb_define_method(io, "gets", ioc_gets, -1);             /* ref: IO#gets(rs = nil) */
    rb_define_method(io, "each", ioc_each_line, -1);        /* ref: IO#each(rs = nil){|line|...} */
    rb_define_alias(io, "each_line", "each");               /* ref: IO#each_line(rs = nil){|line|...} */
    rb_define_method(io, "each_char", ioc_each_char, 0);    /* ref: IO#each_byte{|line|...} */
    rb_define_method(io, "write", ioc_write_chars, 1);      /* ref: IO#write(str) */
    rb_define_method(io, "printf", ioc_printf, -1);         /* ref: IO#printf(...) */
    rb_define_method(io, "print", ioc_print, -1);           /* ref: IO#print(...) */
    rb_define_method(io, "puts", ioc_puts, -1);             /* ref: IO#puts(...) */
    rb_define_method(io, "putc", ioc_write_unichar, 1);     /* ref: IO#putc(ch) but this accept an UTF-8 code */
    rb_define_method(io, "flush", ioc_flush, 0);            /* ref: IO#flush */
    rb_define_method(io, "seek", ioc_seek, -1);             /* ref: IO#seek */
    rb_define_method(io, "set_pos", ioc_seek_pos, 1);       /* ref: IO#pos= */
    rb_define_method(io, "close", ioc_shutdown, -1);        /* ref: IO#close */
    rb_define_method(io, "create_watch", ioc_create_watch, 1);
    rb_define_method(io, "add_watch", ioc_add_watch, 1);
    rb_define_method(io, "buffer_size", ioc_get_buffer_size, 0);
    rb_define_method(io, "set_buffer_size", ioc_set_buffer_size, 1);
    rb_define_method(io, "buffer_condition", ioc_get_buffer_condition, 0);
    rb_define_method(io, "flags", ioc_get_flags, 0);
    rb_define_method(io, "set_flags", ioc_set_flags, 1);
    rb_define_method(io, "buffered", ioc_get_buffered, 0);
    rb_define_method(io, "set_buffered", ioc_set_buffered, 1);
    rb_define_method(io, "encoding", ioc_get_encoding, 0);
    rb_define_method(io, "set_encoding", ioc_set_encoding, 1);

    G_DEF_SETTERS(io);

#ifdef G_OS_WIN32
    {
        /* GIOWin32Channel */
        VALUE io_channel_win32_socket;
        io_channel_win32_socket =
            rb_define_class_under(mGLib,
                                  "IOChannelWin32Socket",
                                  io);
        rb_define_method(io_channel_win32_socket, "initialize",
                         ioc_win32_socket_initialize, 1);
    }
#endif

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
