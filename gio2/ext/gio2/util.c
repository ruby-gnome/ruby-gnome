/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"


#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif


static ID s_id_enum_name;
static ID s_id_errors;

static VALUE s_errors;


VALUE
rbgio_cstr_to_rval_tainted(const char *string, gsize length)
{
        VALUE result;

        if (string == NULL)
                return Qnil;

        result = rb_str_new(string, length);
        OBJ_TAINT(result);

        return result;
}

struct rbgio_cstr_to_rval_tainted_free_data
{
        char *string;
        gsize length;
};

static VALUE
rbgio_cstr_to_rval_tainted_free_body(VALUE data)
{
        struct rbgio_cstr_to_rval_tainted_free_data *real;

        real = (struct rbgio_cstr_to_rval_tainted_free_data *)data;

        return rbgio_cstr_to_rval_tainted(real->string, real->length);
}

static VALUE
rbgio_cstr_to_rval_tainted_free_ensure(VALUE data)
{
        g_free(((struct rbgio_cstr_to_rval_tainted_free_data *)data)->string);

        return Qnil;
}

VALUE
rbgio_cstr_to_rval_tainted_free(char *string, gsize length)
{
        struct rbgio_cstr_to_rval_tainted_free_data data = { string, length };
        
        return rb_ensure(rbgio_cstr_to_rval_tainted_free_body, (VALUE)&data,
                         rbgio_cstr_to_rval_tainted_free_ensure, (VALUE)&data);
}

VALUE
rbgio_glist_to_ary_unref_free(GList *list)
{
        VALUE ary;
        GList *i;

        ary = rb_ary_new();
        for (i = list; i != NULL; i = i->next)
                rb_ary_push(ary, GOBJ2RVAL_UNREF(i->data));
        g_list_free(list);

        return ary;
}

VALUE
rbgio_fds_to_ary(const gint *fds)
{
        int i, n;
        VALUE ary;

        for (i = n = 0; fds[i] != -1; i++)
                n++;

        ary = rb_ary_new2(n);
        for (i = 0; i < n; i++)
                RARRAY_PTR(ary)[i] = FD2RVAL(fds[i]);

        return ary;
}

static VALUE
rbgio_fds_to_ary_free_ensure(gint *fds)
{
        g_free(fds);

        return Qnil;
}

VALUE
rbgio_fds_to_ary_free(gint *fds)
{
        return rb_ensure(rbgio_fds_to_ary, (VALUE)fds,
                         rbgio_fds_to_ary_free_ensure, (VALUE)fds);
}

struct rbgio_gfile_ary_to_glist_args {
    VALUE ary;
    long n;
    GList *result;
};

static VALUE
rbgio_gfile_ary_to_glist_body(VALUE value)
{
    long i;
    struct rbgio_gfile_ary_to_glist_args *args = (struct rbgio_gfile_ary_to_glist_args *)value;

    for (i = 0; i < args->n; i++)
            args->result = g_list_append(args->result, RVAL2GFILE(RARRAY_PTR(args->ary)[i]));

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgio_gfile_ary_to_glist_rescue(VALUE value)
{
    g_list_free(((struct rbgio_gfile_ary_to_glist_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

GList *
rbgio_gfile_ary_to_glist(VALUE value)
{
    struct rbgio_gfile_ary_to_glist_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = NULL;

    rb_rescue(rbgio_gfile_ary_to_glist_body, (VALUE)&args,
              rbgio_gfile_ary_to_glist_rescue, (VALUE)&args);

    return args.result;
}

GList *
rbgio_gfile_ary_to_glist_accept_nil(VALUE ary)
{
        if (NIL_P(ary))
                return NULL;

        return rbgio_gfile_ary_to_glist(ary);
}

void
rbgio_rval_to_gtimeval(VALUE value, GTimeVal *time)
{
        if (rb_respond_to(value, rb_intern("tv_sec"))) {
                time->tv_sec = RVAL2GLONG(rb_funcall(value, rb_intern("tv_sec"), 0));
                time->tv_usec = rb_respond_to(value, rb_intern("tv_usec")) ?
                        RVAL2GLONG(rb_funcall(value, rb_intern("tv_usec"), 0)) :
                        0;
        } else if (rb_respond_to(value, rb_intern("to_ary"))) {
                VALUE ary = rb_ary_to_ary(value);
                if (RARRAY_LEN(ary) < 1 || RARRAY_LEN(ary) > 2)
                        rb_raise(rb_eArgError, "Array of length 1 or 2 expected");
                time->tv_sec = RVAL2GLONG(RARRAY_PTR(ary)[0]);
                time->tv_usec = RARRAY_LEN(ary) > 1 ? RVAL2GLONG(RARRAY_PTR(ary)[1]) : 0;
        } else {
                time->tv_sec = RVAL2GLONG(value);
                time->tv_usec = 0;
        }
}

struct async_ready_callback_data
{
        GAsyncResult *result;
        gpointer data;
};

static VALUE
rbgio_async_ready_callback_call(VALUE data)
{
        static VALUE s_id_call;
        struct async_ready_callback_data *real;
        VALUE block;
 
        if (s_id_call == 0)
                s_id_call = rb_intern("call");

        real = (struct async_ready_callback_data *)data;
        block = USE_BLOCK(real->data);
        if (!NIL_P(block))
                rb_funcall(block, s_id_call, 1, GOBJ2RVAL_UNREF(real->result));

        return Qnil;
}

void
rbgio_async_ready_callback(G_GNUC_UNUSED GObject *source,
                           GAsyncResult *result,
                           gpointer data)
{
        struct async_ready_callback_data real = { result, data };

        G_PROTECT_CALLBACK(rbgio_async_ready_callback_call, &real);
}

VALUE
rbgio_child_remove_and_return(VALUE parent, VALUE child)
{
        G_CHILD_REMOVE(parent, child);

        return child;
}

VALUE
rbgio_define_domain_error(VALUE module,
                          const char *name,
                          GQuark domain,
                          const char *enum_name,
                          VALUE parent)
{
        VALUE error;

        error = rb_define_class_under(module, name, parent);
        rb_ivar_set(error, s_id_enum_name, CSTR2RVAL(enum_name));
        rb_ivar_set(error, s_id_errors, rb_hash_new());
        rb_hash_aset(s_errors, UINT2NUM(domain), error);

        return error;
}

VALUE
rbgio_define_error(VALUE module,
                   const char *name,
                   gint code,
                   VALUE domain_error)
{
        VALUE error;
        
        error = rb_define_class_under(module, name, domain_error);
        rb_hash_aset(rb_ivar_get(domain_error, s_id_errors),
                     INT2NUM(code),
                     error);

        return error;
}

void
rbgio_raise_error(GError *error)
{
        VALUE domain_error, rberror;

        domain_error = rb_hash_aref(s_errors, UINT2NUM(error->domain));
        if (NIL_P(domain_error))
                RAISE_GERROR(error);

        rberror = rb_hash_aref(rb_ivar_get(domain_error, s_id_errors),
                               INT2NUM(error->code));
        if (NIL_P(rberror)) {
                VALUE enum_name = rb_ivar_get(domain_error, s_id_enum_name);

                rb_raise(rb_eNotImpError,
                         "%s contains error codes that have not been implemented: %d",
                         RVAL2CSTR(enum_name), error->code);
        }

        rb_raise(rberror, "%s", error->message);
}

void
Init_util(void)
{
        s_id_enum_name = rb_intern("@enum_name");
        s_id_errors = rb_intern("@errors");

        s_errors = rb_hash_new();
        rb_global_variable(&s_errors);
}
