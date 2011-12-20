/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Masao Mutoh
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

ID rbgutil_id_module_eval;

static ID id_add_one_arg_setter;
static ID id_set_property;
static ID id_to_a;
static ID id_allocate;

void
rbg_define_method(VALUE klass, const char *name, VALUE (*func)(ANYARGS), int argc)
{
    rb_define_method(klass, name, func, argc);
    if ((argc != 1) || strncmp(name, "set_", 4))
        return;

    name += 4;
    rb_funcall(klass, rbgutil_id_module_eval, 1,
               CSTR2RVAL_FREE(g_strdup_printf("def %s=(val); set_%s(val); val; end\n",
                                              name, name)));
}

void
rbg_define_singleton_method(VALUE obj, const char *name, VALUE (*func)(ANYARGS), int argc)
{
    rb_define_singleton_method(obj, name, func, argc);
    if ((argc != 1) || strncmp(name, "set_", 4))
        return;

    name += 4;
    rb_funcall(obj, rbgutil_id_module_eval, 1,
               CSTR2RVAL_FREE(g_strdup_printf("def self.%s=(val); set_%s(val); val; end\n",
                                              name, name)));
}

void
rbgutil_set_properties(VALUE self, VALUE hash)
{
    int i;
    VALUE ary;
    GObject* obj;

    Check_Type(hash, T_HASH);
    ary = rb_funcall(hash, id_to_a, 0);
    obj = RVAL2GOBJ(self);

    g_object_freeze_notify(obj);
    for (i = 0; i < RARRAY_LEN(ary); i++) {
      rb_funcall(self, id_set_property, 2,
                 RARRAY_PTR(RARRAY_PTR(ary)[i])[0],
                 RARRAY_PTR(RARRAY_PTR(ary)[i])[1]);
    }
    g_object_thaw_notify(obj);
}

VALUE
rbgutil_def_setters(VALUE klass)
{
    return rb_funcall(mGLib, id_add_one_arg_setter, 1, klass);
}

void
rbgutil_glibid_r2g_func(VALUE from, GValue* to)
{
    VALUE buffer;
    g_value_set_string(to, RVAL2GLIBID(from, buffer));
}

VALUE
rbgutil_sym_g2r_func(const GValue *from)
{
    const gchar *str = g_value_get_string(from);
    return str ? ID2SYM(rb_intern(str)) : Qnil;
}

VALUE
rbgutil_generic_s_gtype(VALUE klass)
{
    return rbgobj_gtype_new(CLASS2GTYPE(klass));
}

VALUE
rbgutil_generic_gtype(VALUE self)
{
    return generic_s_gtype(CLASS_OF(self));
}

VALUE
rbgutil_string_set_utf8_encoding(VALUE string)
{
#ifdef HAVE_RUBY_ENCODING_H
    if (!NIL_P(string))
        rb_enc_associate(string, rb_utf8_encoding());
#endif
    return string;
}

void
Init_gutil(void)
{
    rbgutil_id_module_eval = rb_intern("module_eval");
    id_set_property = rb_intern("set_property");
    id_to_a = rb_intern("to_a");
    id_add_one_arg_setter = rb_intern("__add_one_arg_setter");
    id_allocate = rb_intern("allocate");
}
