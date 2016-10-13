/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2015  Ruby-GNOME2 Project Team
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

#include <string.h>

ID rbgutil_id_module_eval;

static ID id_add_one_arg_setter;
static ID id_set_property;
static ID id_to_a;
static ID id_allocate;
static ID id_equal;

void
rbg_define_method(VALUE klass, const char *name, VALUE (*func)(ANYARGS), int argc)
{
    rb_define_method(klass, name, func, argc);
    if ((argc != 1) || strncmp(name, "set_", 4))
        return;

    name += 4;
    rb_funcall(klass, rbgutil_id_module_eval, 3,
               CSTR2RVAL_FREE(g_strdup_printf("def %s=(val); set_%s(val); val; end\n",
                                              name, name)),
               rb_str_new2(__FILE__),
               INT2NUM(__LINE__));
}

void
rbg_define_singleton_method(VALUE obj, const char *name, VALUE (*func)(ANYARGS), int argc)
{
    rb_define_singleton_method(obj, name, func, argc);
    if ((argc != 1) || strncmp(name, "set_", 4))
        return;

    name += 4;
    rb_funcall(obj, rbgutil_id_module_eval, 3,
               CSTR2RVAL_FREE(g_strdup_printf("def self.%s=(val); set_%s(val); val; end\n",
                                              name, name)),
               rb_str_new2(__FILE__),
               INT2NUM(__LINE__));
}

void
rbgutil_set_properties(VALUE self, VALUE hash)
{
    int i;
    VALUE ary;
    GObject* obj;

    Check_Type(hash, RUBY_T_HASH);
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
    if (!NIL_P(string))
        rb_enc_associate(string, rb_utf8_encoding());
    return string;
}

gboolean
rbgutil_key_equal(VALUE rb_key, const char *key)
{
    switch (TYPE(rb_key)) {
      case RUBY_T_STRING:
        return RVAL2CBOOL(rb_funcall(rb_key, id_equal, 1, rb_str_new_cstr(key)));
        break;
      case RUBY_T_SYMBOL:
        return SYM2ID(rb_key) == rb_intern(key);
        break;
      default:
        return FALSE;
        break;
    }
}

static gboolean
rbg_interrupt_prepare (G_GNUC_UNUSED GSource *source,
                       G_GNUC_UNUSED gint *timeout)
{
    return rb_thread_interrupted(rb_thread_current());
}

static gboolean
rbg_interrupt_check (G_GNUC_UNUSED GSource *source)
{
    return rb_thread_interrupted(rb_thread_current());
}

static gboolean
rbg_interrupt_dispatch (G_GNUC_UNUSED GSource *source,
                        GSourceFunc callback,
                        gpointer user_data)
{
    if (callback) {
        return callback(user_data);
    } else {
        return G_SOURCE_REMOVE;
    }
}

static GSourceFuncs rbg_interrupt_funcs;
static void
rbg_interrupt_funcs_init(void)
{
    memset(&rbg_interrupt_funcs, 0, sizeof(GSourceFuncs));
    rbg_interrupt_funcs.prepare  = rbg_interrupt_prepare;
    rbg_interrupt_funcs.check    = rbg_interrupt_check;
    rbg_interrupt_funcs.dispatch = rbg_interrupt_dispatch;
}

GSource *
rbg_interrupt_source_new(void)
{
    return g_source_new(&rbg_interrupt_funcs, sizeof(GSource));
}

gchar *
rbg_name_to_nick(const gchar *name)
{
    gchar *nick, *current;

    nick = g_strdup(name);
    for (current = nick; *current; current++) {
        switch (*current) {
        case '_':
        case ' ':
            *current = '-';
            break;
        default:
            *current = g_ascii_tolower(*current);
            break;
        }
    }

    return nick;
}

void *
rbg_memzero(void *pointer, size_t size)
{
    memset(pointer, 0, size);
    return pointer;
}

void
Init_gutil(void)
{
    rbgutil_id_module_eval = rb_intern("module_eval");
    id_set_property = rb_intern("set_property");
    id_to_a = rb_intern("to_a");
    id_add_one_arg_setter = rb_intern("__add_one_arg_setter");
    id_allocate = rb_intern("allocate");
    id_equal = rb_intern("==");

    rbg_interrupt_funcs_init();
}
