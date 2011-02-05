/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgerror.c -

  $Author: mutoh $
  $Date: 2004/08/22 13:27:47 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

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
    if NIL_P(klass){
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
        int i;

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
Init_glib_error()
{
    id_code = rb_intern("@code");
    id_domain = rb_intern("@domain");
    gerror_table = rb_hash_new();
    rb_global_variable(&gerror_table);

    generic_error = rb_define_class_under(mGLib, "Error", rb_eRuntimeError);
    rb_funcall(generic_error, rbgutil_id_module_eval, 1, CSTR2RVAL("def code; @code; end\n"));
    rb_funcall(generic_error, rbgutil_id_module_eval, 1, CSTR2RVAL("def domain; @domain; end\n"));
}
