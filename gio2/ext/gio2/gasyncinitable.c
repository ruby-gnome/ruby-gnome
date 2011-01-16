/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

#define _SELF(value) G_ASYNC_INITABLE(RVAL2GOBJ(value))

static VALUE
asyncinitable_init_async(int argc, VALUE *argv, VALUE self)
{
        VALUE rbio_priority, rbcancellable, block;
        int io_priority;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbio_priority, &rbcancellable, &block);
        io_priority = RVAL2IOPRIORITYDEFAULT(rbio_priority);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_async_initable_init_async(_SELF(self),
                                    io_priority,
                                    cancellable,
                                    rbgio_async_ready_callback,
                                    (gpointer)block);

        return self;
}

static VALUE
asyncinitable_init_finish(VALUE self, VALUE result)
{
        GError *error = NULL;

        if (!g_async_initable_init_finish(_SELF(self),
                                          RVAL2GASYNCRESULT(result),
                                          &error))
                rbgio_raise_error(error);

        return self;
}

/* NOTE: We don’t implement g_async_initable_new_async. */

/* NOTE: We don’t implement g_async_initable_new_valist_async. */

struct rbgio_gasyncinitable_new_async_data {
        GObjectClass *gclass;
        int io_priority;
        GCancellable *cancellable;
        VALUE block;
        VALUE rbparameters;
        guint index;
        guint n_parameters;
        GParameter *parameters;
};

static VALUE
rbgio_gasyncinitiable_new_async_parameters_initialize(VALUE key_value,
                                                      struct rbgio_gasyncinitable_new_async_data *data)
{
        VALUE key, value;
        GParamSpec *spec;

        if (data->index >= data->n_parameters)
                rb_raise(rb_eArgError,
                         "too many parameters (%d >= %d)",
                         data->index, data->n_parameters);

        key = rb_ary_entry(key_value, 0);
        value = rb_ary_entry(key_value, 1);

        data->parameters[data->index].name = SYMBOL_P(key) ?
                rb_id2name(SYM2ID(key)) :
                StringValuePtr(key);
        spec = g_object_class_find_property(data->gclass,
                                            data->parameters[data->index].name);
        if (spec == NULL)
                rb_raise(rb_eArgError,
                         "%s: no such property",
                         data->parameters[data->index].name);
        g_value_init(&(data->parameters[data->index].value),
                     G_PARAM_SPEC_VALUE_TYPE(spec));
        rbgobj_rvalue_to_gvalue(value, &(data->parameters[data->index].value));

        data->index++;

        return Qnil;
}

static VALUE
rbgio_gasyncinitable_new_async_body(struct rbgio_gasyncinitable_new_async_data *data)
{
        rb_iterate(rb_each,
                   data->rbparameters,
                   rbgio_gasyncinitiable_new_async_parameters_initialize,
                   (VALUE)data);

        SAVE_BLOCK(data->block);

        g_async_initable_newv_async(G_TYPE_FROM_CLASS(data->gclass),
                                    data->n_parameters,
                                    data->parameters,
                                    data->io_priority,
                                    data->cancellable,
                                    rbgio_async_ready_callback,
                                    (gpointer)data->block);

        return Qnil;
}

static VALUE
rbgio_gasyncinitable_new_async_ensure(struct rbgio_gasyncinitable_new_async_data *data)
{
        guint i;

        g_type_class_unref(data->gclass);
        for (i = 0; i < data->n_parameters; i++)
                if (G_IS_VALUE(&data->parameters[i].value))
                        g_value_unset(&data->parameters[i].value);

        return Qnil;
}

void
rbgio_gasyncinitable_new_async(GType type,
                               VALUE parameters,
                               VALUE io_priority,
                               VALUE cancellable,
                               VALUE block)
{
        static ID s_id_length;
        struct rbgio_gasyncinitable_new_async_data data;

        if (s_id_length == 0)
                s_id_length = rb_intern("length");

        if (!g_type_is_a(type, G_TYPE_OBJECT))
                rb_raise(rb_eArgError,
                         "%s is not a descendant of GObject",
                         g_type_name(type));

        if (NIL_P(parameters)) {
                SAVE_BLOCK(block);
                g_async_initable_newv_async(type,
                                            0,
                                            NULL,
                                            RVAL2IOPRIORITYDEFAULT(io_priority),
                                            RVAL2GCANCELLABLE(cancellable),
                                            rbgio_async_ready_callback,
                                            (gpointer)block);

                return;
        } else {
                parameters = rb_convert_type(parameters,
                                             T_HASH,
                                             "Hash",
                                             "to_hash");
        }

        data.gclass = G_OBJECT_CLASS(g_type_class_ref(type));
        data.io_priority = RVAL2IOPRIORITYDEFAULT(io_priority);
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.block = block;
        data.rbparameters = parameters;
        data.index = 0;
        data.n_parameters = RVAL2GUINT(rb_funcall(parameters, s_id_length, 0));
        data.parameters = ALLOCA_N(GParameter, data.n_parameters);

        rb_ensure(rbgio_gasyncinitable_new_async_body, (VALUE)&data,
                  rbgio_gasyncinitable_new_async_ensure, (VALUE)&data);

        return;
}

static VALUE
asyncinitable_initialize(int argc, VALUE *argv, VALUE self)
{
        const RGObjClassInfo* info;
        VALUE io_priority, cancellable, parameters, block;

        rb_scan_args(argc, argv, "03&", &io_priority, &cancellable, &parameters, &block);

        info = CLASS2CINFO(self);
        if (info->klass != self)
                rb_raise(rb_eTypeError,
                         "%s: class not registered with GLib",
                         rb_class2name(self));

        rbgio_gasyncinitable_new_async(info->gtype,
                                       parameters,
                                       io_priority,
                                       cancellable,
                                       block);

        return self;
}


static VALUE
asyncinitable_initialize_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        GObject *object;
        VALUE rbobject;

        object = g_async_initable_new_finish(_SELF(self),
                                             RVAL2GASYNCRESULT(result),
                                             &error);
        if (object == NULL)
                rbgio_raise_error(error);

        rbobject = GOBJ2RVAL(object);
        g_object_unref(object);

        return rbobject;
}

void
Init_gasyncinitable(VALUE glib)
{
        VALUE asyncinitable = G_DEF_INTERFACE(G_TYPE_ASYNC_INITABLE, "AsyncInitable", glib);

        rb_define_method(asyncinitable, "init_async", asyncinitable_init_async, -1);
        rb_define_method(asyncinitable, "init_finish", asyncinitable_init_finish, 1);
        rb_define_method(asyncinitable, "initialize", asyncinitable_initialize, -1);
        rb_define_method(asyncinitable, "initialize_finish", asyncinitable_initialize_finish, 1);
}
