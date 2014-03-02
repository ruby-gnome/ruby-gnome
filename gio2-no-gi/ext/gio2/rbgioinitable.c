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

#define RG_TARGET_NAMESPACE mInitable
#define _SELF(value) RVAL2GINITABLE(value)

static VALUE
rg_init(int argc, VALUE *argv, VALUE self)
{
        VALUE cancellable;
        GError *error = NULL;

        rb_scan_args(argc, argv, "01", &cancellable);
        if (!g_initable_init(_SELF(self),
                             RVAL2GCANCELLABLE(cancellable),
                             &error))
                rbgio_raise_error(error);

        return self;
}

struct rbgio_ginitable_new_data {
        GObjectClass *gclass;
        GCancellable *cancellable;
        VALUE rbparameters;
        guint index;
        guint n_parameters;
        GParameter *parameters;
        GError *error;
};

static VALUE
rbgio_ginitiable_new_parameters_initialize(VALUE key_value,
                                           struct rbgio_ginitable_new_data *data)
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
rbgio_ginitable_new_body(struct rbgio_ginitable_new_data *data)
{
        rb_iterate(rb_each,
                   data->rbparameters,
                   rbgio_ginitiable_new_parameters_initialize,
                   (VALUE)data);

        return (VALUE)g_initable_newv(G_TYPE_FROM_CLASS(data->gclass),
                                      data->n_parameters,
                                      data->parameters,
                                      data->cancellable,
                                      &data->error);
}

static VALUE
rbgio_ginitable_new_ensure(struct rbgio_ginitable_new_data *data)
{
        guint i;

        g_type_class_unref(data->gclass);
        for (i = 0; i < data->n_parameters; i++)
                if (G_IS_VALUE(&data->parameters[i].value))
                        g_value_unset(&data->parameters[i].value);

        g_free(data->parameters);

        return Qnil;
}

GObject *
rbgio_ginitable_new(GType type, VALUE parameters, VALUE cancellable)
{
        static ID s_id_length;
        GError *error = NULL;
        GObject *object;
        struct rbgio_ginitable_new_data data;

        if (s_id_length == 0)
                s_id_length = rb_intern("length");

        if (!g_type_is_a(type, G_TYPE_OBJECT))
                rb_raise(rb_eArgError,
                         "%s is not a descendant of GObject",
                         g_type_name(type));

        if (NIL_P(parameters)) {
                object = g_initable_newv(type,
                                         0,
                                         NULL,
                                         RVAL2GCANCELLABLE(cancellable),
                                         &error);
                if (object == NULL)
                        rbgio_raise_error(error);

                return object;
        } else {
                parameters = rb_convert_type(parameters,
                                             T_HASH,
                                             "Hash",
                                             "to_hash");
        }

        data.gclass = G_OBJECT_CLASS(g_type_class_ref(type));
        data.cancellable = RVAL2GCANCELLABLE(cancellable);
        data.rbparameters = parameters;
        data.index = 0;
        data.n_parameters = RVAL2GUINT(rb_funcall(parameters, s_id_length, 0));
        data.parameters = g_new(GParameter, data.n_parameters);
        data.error = NULL;

        object = (GObject *)rb_ensure(rbgio_ginitable_new_body, (VALUE)&data,
                                      rbgio_ginitable_new_ensure, (VALUE)&data);
        if (object == NULL)
                rbgio_raise_error(data.error);

        return object;
}

/* NOTE: We don't implement g_initable_new. */

/* NOTE: We don't implement g_initable_new_valist. */

static VALUE
rg_s_new(int argc, VALUE *argv, VALUE self)
{
        const RGObjClassInfo* info;
        VALUE cancellable, parameters;
        GObject *object;
        VALUE result;

        rb_scan_args(argc, argv, "02", &cancellable, &parameters);

        info = CLASS2CINFO(self);
        if (info->klass != self)
                rb_raise(rb_eTypeError,
                         "%s: class not registered with GLib",
                         rb_class2name(self));

        object = rbgio_ginitable_new(info->gtype, parameters, cancellable);
        result = GOBJ2RVAL(object);
        g_object_unref(object);

        return result;
}

void
Init_ginitable(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_INITABLE, "Initable", mGio);

        RG_DEF_SMETHOD(new, -1);

        RG_DEF_METHOD(init, -1);
}
