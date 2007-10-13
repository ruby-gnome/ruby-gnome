/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbpoppler-attachment.c -

  $Author: ktou $
  $Date: 2007/10/13 05:53:10 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"

#define SELF(self) ((PopplerAttachment *)RVAL2GOBJ(self))

static ID id_call;

static gboolean
attachment_save_func(const gchar *buf, gsize count, gpointer data,
                     GError **error)
{
    VALUE result;
    VALUE callback = (VALUE)data;

    result = rb_funcall(callback, id_call, 1, rb_str_new(buf, count));
    return RVAL2CBOOL(result);
}

static VALUE
attachment_save(int argc, VALUE *argv, VALUE self)
{
    VALUE filename;
    gboolean result;
    GError *error = NULL;

    rb_scan_args(argc, argv, "01", &filename);

    if (NIL_P(filename)) {
        if (rb_block_given_p()) {
            gpointer user_data = (gpointer)rb_block_proc();
            result = poppler_attachment_save_to_callback(SELF(self),
                                                         attachment_save_func,
                                                         user_data,
                                                         &error);
        } else {
            rb_raise(rb_eArgError, "must provide filename or block");
        }
    } else {
        result = poppler_attachment_save(SELF(self), RVAL2CSTR(filename),
                                         &error);
    }

    if (error)
        RAISE_GERROR(error);

    return CBOOL2RVAL(result);
}

static VALUE
attachment_get_name(VALUE self)
{
    return CSTR2RVAL(SELF(self)->name);
}

static VALUE
attachment_get_description(VALUE self)
{
    return CSTR2RVAL(SELF(self)->description);
}

#if POPPLER_CHECK_VERSION(0, 6, 0)
static VALUE
attachment_get_size(VALUE self)
{
    return INT2NUM(SELF(self)->size);
}
#endif

static VALUE
attachment_get_mtime(VALUE self)
{
    return rb_time_new(SELF(self)->mtime, 0);
}

static VALUE
attachment_get_ctime(VALUE self)
{
    return rb_time_new(SELF(self)->ctime, 0);
}

#if POPPLER_CHECK_VERSION(0, 6, 0)
static VALUE
attachment_get_checksum(VALUE self)
{
    GString *checksum;
    checksum = SELF(self)->checksum;
    return rb_str_new(checksum->str, checksum->len);
}
#endif

void
Init_poppler_attachment(VALUE mPoppler)
{
    VALUE cAttachment;

    id_call = rb_intern("call");

    cAttachment = G_DEF_CLASS(POPPLER_TYPE_ATTACHMENT, "Attachment", mPoppler);

    rb_define_method(cAttachment, "save", attachment_save, -1);
    rb_define_method(cAttachment, "name", attachment_get_name, 0);
    rb_define_method(cAttachment, "description", attachment_get_description, 0);
#if POPPLER_CHECK_VERSION(0, 6, 0)
    rb_define_method(cAttachment, "size", attachment_get_size, 0);
#endif
    rb_define_method(cAttachment, "mtime", attachment_get_mtime, 0);
    rb_define_method(cAttachment, "ctime", attachment_get_ctime, 0);
#if POPPLER_CHECK_VERSION(0, 6, 0)
    rb_define_method(cAttachment, "checksum", attachment_get_checksum, 0);
#endif

    G_DEF_SETTERS(cAttachment);
}
