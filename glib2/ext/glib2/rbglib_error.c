/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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
static VALUE gerror_table;
static VALUE generic_error;

VALUE
rbgerr_gerror2exception(GError *error)
{
    VALUE exc = Qnil;
    VALUE klass = Qnil;

    if (! error){
        return rb_exc_new2(rb_eRuntimeError, "GError parameter doesn't have a value.");
    }

    klass = rb_hash_aref(gerror_table, UINT2NUM(error->domain));
    if (NIL_P(klass)) {
        exc = rb_exc_new2(generic_error, error->message);
        rb_ivar_set(exc, id_domain, CSTR2RVAL(g_quark_to_string(error->domain)));
        rb_ivar_set(exc, id_code, INT2NUM(error->code));
    } else {
        exc = rb_exc_new2(klass, error->message);
        rb_ivar_set(exc, id_domain, CSTR2RVAL(g_quark_to_string(error->domain)));
        rb_ivar_set(exc, id_code, INT2NUM(error->code));
    }
    g_error_free(error);
    return exc;
}

VALUE
rbgerr_define_gerror(GQuark domain, const gchar *name, VALUE module, VALUE parent, VALUE gtype)
{
    VALUE klass = rb_define_class_under(module, name, parent);
    rb_funcall(klass, rbgutil_id_module_eval, 1, CSTR2RVAL("def code; @code; end\n"));
    rb_funcall(klass, rbgutil_id_module_eval, 1, CSTR2RVAL("def domain; @domain; end\n"));

    rb_hash_aset(gerror_table, UINT2NUM(domain), klass);

    if (! NIL_P(gtype)){
        GEnumClass* gclass = g_type_class_ref(gtype);
        guint i;

        for (i = 0; i < gclass->n_values; i++) {
            GEnumValue* entry = &(gclass->values[i]);
            gchar* nick = g_strdup(entry->value_nick);
            gchar* p;
            
            for (p = nick; *p; p++) {
                if (*p == '-')
                    *p = '_';
                else
                    *p = g_ascii_toupper(*p);
            }
            rbgobj_define_const(klass, nick, INT2NUM(i));
            g_free(nick);
        }
        
        g_type_class_unref(gclass);        
    }

    return klass;
}

void
Init_glib_error(void)
{
    id_code = rb_intern("@code");
    id_domain = rb_intern("@domain");
    gerror_table = rb_hash_new();
    rb_global_variable(&gerror_table);

    generic_error = rb_define_class_under(mGLib, "Error", rb_eRuntimeError);
    rb_funcall(generic_error, rbgutil_id_module_eval, 1, CSTR2RVAL("def code; @code; end\n"));
    rb_funcall(generic_error, rbgutil_id_module_eval, 1, CSTR2RVAL("def domain; @domain; end\n"));
}
