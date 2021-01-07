/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017-2021  Ruby-GNOME Project Team
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

static VALUE rb_cGLibBytes;

#define RG_TARGET_NAMESPACE rb_cGLibBytes

#define _SELF(s) (RVAL2BOXED(s, G_TYPE_BYTES))

gboolean
rbg_is_bytes(VALUE object)
{
    return RVAL2CBOOL(rb_obj_is_kind_of(object, RG_TARGET_NAMESPACE));
}

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_s_try_convert(VALUE self, VALUE value)
{
    if (NIL_P(value)) {
        return value;
    }

    if (RVAL2CBOOL(rb_obj_is_kind_of(value, self))) {
        return value;
    }

    if (RB_TYPE_P(value, RUBY_T_STRING)) {
        ID id_new;
        CONST_ID(id_new, "new");
        return rb_funcall(self, id_new, 1, value);
    }

    return Qnil;
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GBytes *bytes;
    VALUE rb_data;

    rb_scan_args(argc, argv, "01", &rb_data);
    if (NIL_P(rb_data)) {
        bytes = g_bytes_new(NULL, 0);
    } else {
        const gchar *data;
        if (!RB_OBJ_FROZEN(rb_data)) {
            rb_data = rb_str_dup(rb_data);
            rb_str_freeze(rb_data);
        }
        data = RVAL2CSTR_PTR(rb_data);
        bytes = g_bytes_new_static(data, RSTRING_LEN(rb_data));
        rb_iv_set(self, "source", rb_data);
    }
    G_INITIALIZE(self, bytes);

    return Qnil;
}

static VALUE
rg_to_s(VALUE self)
{
    GBytes *bytes;
    gconstpointer data;
    gsize size;
    VALUE rb_data;

    bytes = _SELF(self);
    data = g_bytes_get_data(bytes, &size);
#  ifdef HAVE_RB_ENC_STR_NEW_STATIC
    rb_data = rb_enc_str_new_static(data, size, rb_ascii8bit_encoding());
    rb_iv_set(rb_data, "@bytes", self);
#  else
    rb_data = rb_enc_str_new(data, size, rb_ascii8bit_encoding());
#  endif
    rb_obj_freeze(rb_data);
    return rb_data;
}

static VALUE
rg_size(VALUE self)
{
    GBytes *bytes;

    bytes = _SELF(self);
    return ULONG2NUM(g_bytes_get_size(bytes));
}

static VALUE
rg_pointer(VALUE self)
{
    GBytes *bytes;
    gconstpointer data;

    bytes = _SELF(self);
    data = g_bytes_get_data(bytes, NULL);

    return ULONG2NUM((guintptr)data);
}

void
Init_glib_bytes(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_BYTES, "Bytes", mGLib);

    RG_DEF_SMETHOD(try_convert, 1);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(to_s, 0);
    RG_DEF_ALIAS("to_str", "to_s");
    RG_DEF_METHOD(size, 0);
    RG_DEF_ALIAS("length", "size");
    RG_DEF_METHOD(pointer, 0);
}
