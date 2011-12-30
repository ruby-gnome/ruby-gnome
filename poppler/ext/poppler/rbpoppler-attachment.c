/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
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

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE cAttachment
#define SELF(self) (RVAL2POPPLERATTACHMENT(self))

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
rg_save(int argc, VALUE *argv, VALUE self)
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
rg_name(VALUE self)
{
    return CSTR2RVAL(SELF(self)->name);
}

static VALUE
rg_description(VALUE self)
{
    return CSTR2RVAL(SELF(self)->description);
}

static VALUE
rg_size(VALUE self)
{
    return INT2NUM(SELF(self)->size);
}

static VALUE
rg_mtime(VALUE self)
{
    return rb_time_new(SELF(self)->mtime, 0);
}

static VALUE
rg_ctime(VALUE self)
{
    return rb_time_new(SELF(self)->ctime, 0);
}

static VALUE
rg_checksum(VALUE self)
{
    GString *checksum;
    checksum = SELF(self)->checksum;
    return rb_str_new(checksum->str, checksum->len);
}

void
Init_poppler_attachment(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE;

    id_call = rb_intern("call");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_ATTACHMENT, "Attachment", mPoppler);

    RG_DEF_METHOD(save, -1);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(description, 0);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(mtime, 0);
    RG_DEF_METHOD(ctime, 0);
    RG_DEF_METHOD(checksum, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
