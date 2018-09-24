/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2017  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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
#include <ctype.h>

static ID id_code;
static ID id_domain;
static ID id_code_classes;
static VALUE gerror_table;
static VALUE generic_error;
static VALUE error_info;

VALUE
rbgerr_gerror2exception(GError *error)
{
    VALUE exc = Qnil;
    VALUE exception_klass = Qnil;

    if (!error) {
        return rb_exc_new2(rb_eRuntimeError, "GError parameter doesn't have a value.");
    }

    exception_klass = rb_hash_aref(gerror_table, UINT2NUM(error->domain));
    if (NIL_P(exception_klass)) {
        exception_klass = generic_error;
    } else {
        VALUE code_class = Qnil;
        VALUE code_classes;
        code_classes = rb_ivar_get(exception_klass, id_code_classes);
        if (!NIL_P(code_classes)) {
            code_class = rb_hash_aref(code_classes, INT2NUM(error->code));
        }
        if (!NIL_P(code_class)) {
            exception_klass = code_class;
        }
    }
    exc = rb_exc_new_str(exception_klass, CSTR2RVAL(error->message));
    rb_ivar_set(exc, id_domain, CSTR2RVAL(g_quark_to_string(error->domain)));
    rb_ivar_set(exc, id_code, INT2NUM(error->code));
    g_error_free(error);
    return exc;
}

static gchar *
nick_to_constant_name(const gchar *nick)
{
    GString *constant_name;
    const gchar *current;

    constant_name = g_string_new(NULL);

    for (current = nick; *current; current++) {
        if (*current == '-') {
            g_string_append_c(constant_name, '_');
        } else {
            g_string_append_c(constant_name, g_ascii_toupper(*current));
        }
    }

    return g_string_free(constant_name, FALSE);
}

static gchar *
nick_to_class_name(const gchar *nick)
{
    GString *class_name;
    const gchar *current;
    gboolean to_upper = TRUE;

    class_name = g_string_new(NULL);

    for (current = nick; *current; current++) {
        if (to_upper) {
            g_string_append_c(class_name, g_ascii_toupper(*current));
            to_upper = FALSE;
        } else if (*current == '-') {
            to_upper = TRUE;
        } else {
            g_string_append_c(class_name, *current);
        }
    }

    return g_string_free(class_name, FALSE);
}

VALUE
rbgerr_define_gerror(GQuark domain, const gchar *name, VALUE module, VALUE parent, GType gtype)
{
    VALUE error_class;
    VALUE code_classes;

    error_class = rb_define_class_under(module, name, parent);
    rb_include_module(error_class, error_info);

    rb_hash_aset(gerror_table, UINT2NUM(domain), error_class);

    code_classes = rb_hash_new();
    rb_ivar_set(error_class, id_code_classes, code_classes);

    if (gtype != G_TYPE_INVALID) {
        GEnumClass* gclass = g_type_class_ref(gtype);
        guint i;

        for (i = 0; i < gclass->n_values; i++) {
            VALUE code_class;
            GEnumValue* entry = &(gclass->values[i]);
            gchar *code_constant_name;
            gchar *code_class_name;

            code_constant_name = nick_to_constant_name(entry->value_nick);
            code_class_name = nick_to_class_name(entry->value_nick);

            if (strcmp(code_constant_name, code_class_name) != 0) {
                rbgobj_define_const(error_class,
                                    code_constant_name,
                                    INT2NUM(entry->value));
            }
            g_free(code_constant_name);

            code_class = rb_define_class_under(error_class,
                                               code_class_name,
                                               error_class);
            g_free(code_class_name);
            rb_hash_aset(code_classes, INT2NUM(entry->value), code_class);
        }

        g_type_class_unref(gclass);
    }

    return error_class;
}

static VALUE
rbg_error_to_ruby(const GValue *from)
{
    GError *error;
    error = g_value_get_boxed(from);
    return GERROR2RVAL(error);
}

void
Init_glib_error(void)
{
    id_code = rb_intern("@code");
    id_domain = rb_intern("@domain");
    id_code_classes = rb_intern("@code_classes");
    gerror_table = rb_hash_new();
    rb_global_variable(&gerror_table);

    error_info = rb_define_module_under(mGLib, "ErrorInfo");
    rb_define_attr(error_info, "code", TRUE, FALSE);
    rb_define_attr(error_info, "domain", TRUE, FALSE);

    generic_error = rb_define_class_under(mGLib, "Error", rb_eRuntimeError);
    rb_include_module(generic_error, error_info);
}

void
Init_glib_error_conversions(void)
{
    rbgobj_register_g2r_func(G_TYPE_ERROR, rbg_error_to_ruby);
}
