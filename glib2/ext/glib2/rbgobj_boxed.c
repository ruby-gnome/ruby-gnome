/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2019  Ruby-GNOME Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
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

#define RG_TARGET_NAMESPACE rbgobj_cBoxed

VALUE RG_TARGET_NAMESPACE;

static void
boxed_mark(void *data)
{
    boxed_holder *holder = data;
    const RGObjClassInfo *cinfo;

    cinfo = GTYPE2CINFO_NO_CREATE(holder->type);
    if (cinfo && cinfo->mark)
        cinfo->mark(holder->boxed);
}

static void
boxed_free(void *data)
{
    boxed_holder *holder = data;
    const RGObjClassInfo *cinfo;

    cinfo = GTYPE2CINFO_NO_CREATE(holder->type);
    if (cinfo && cinfo->free)
        cinfo->free(holder->boxed);

    if (holder->own && holder->boxed)
        g_boxed_free(holder->type, holder->boxed);

    xfree(holder);
}

static const rb_data_type_t rg_glib_boxed_type = {
    "GLib::Boxed",
    {
        boxed_mark,
        boxed_free,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

static boxed_holder *
rbgobj_boxed_get_raw(VALUE rb_boxed)
{
    boxed_holder *holder;
    TypedData_Get_Struct(rb_boxed, boxed_holder, &rg_glib_boxed_type, holder);
    return holder;
}

/**********************************************************************/

VALUE
rbgobj_boxed_alloc_func(VALUE klass)
{
    const RGObjClassInfo *cinfo = rbgobj_lookup_class(klass);
    boxed_holder *holder;
    VALUE rb_boxed;

    if (cinfo->gtype == G_TYPE_BOXED)
        rb_raise(rb_eTypeError, "abstract class");

    rb_boxed = TypedData_Make_Struct(klass,
                                     boxed_holder,
                                     &rg_glib_boxed_type,
                                     holder);
    holder->type  = cinfo->gtype;
    holder->boxed = NULL;
    holder->own   = FALSE;

    return rb_boxed;
}

static VALUE
rg_initialize(VALUE self)
{
    VALUE rb_class;
    gsize instance_size = 0;

    rb_class = CLASS_OF(self);
    if (RVAL2CBOOL(rb_ivar_defined(rb_class, rb_intern("@size")))) {
        instance_size = NUM2UINT(rb_iv_get(rb_class, "@size"));
    }

    if (instance_size > 0) {
        const RGObjClassInfo *cinfo;
        gpointer instance;

        cinfo = rbgobj_lookup_class(rb_class);
        instance = alloca(instance_size);
        memset(instance, 0, instance_size);
        G_INITIALIZE(self, g_boxed_copy(cinfo->gtype, instance));
    } else {
        rb_raise(rb_eTypeError, "can't initialize %s",
                 rb_class2name(rb_class));
    }

    return Qnil;
}

static VALUE
rg_inspect(VALUE self)
{
    boxed_holder *holder;

    holder = rbgobj_boxed_get_raw(self);

    return rb_sprintf("#<%" PRIsVALUE ":%p ptr=%p own=%s>",
                      CLASS_OF(self),
                      (gpointer)self,
                      holder->boxed,
                      holder->own ? "true" : "false");
}

static VALUE
rg_initialize_copy(VALUE self, VALUE orig)
{
    boxed_holder *holder1;
    boxed_holder *holder2;

    if (self == orig) return self;

    if (!rb_obj_is_instance_of(orig, rb_obj_class(self))) {
        rb_raise(rb_eTypeError, "wrong argument class");
    }

    holder1 = rbgobj_boxed_get_raw(self);
    holder2 = rbgobj_boxed_get_raw(orig);

    holder1->boxed = g_boxed_copy(holder2->type, holder2->boxed);
    holder1->own   = TRUE;

    if (!holder1->boxed)
      rb_raise(rb_eRuntimeError, "g_boxed_copy() failed");

    return self;
}

/* deprecated */
VALUE
rbgobj_boxed_create(VALUE klass)
{
    return rbgobj_boxed_alloc_func(klass);
}

/**********************************************************************/

void
rbgobj_boxed_initialize(VALUE obj, gpointer boxed)
{
    boxed_holder *holder;
    holder = rbgobj_boxed_get_raw(obj);
    holder->boxed = boxed;
    holder->own   = TRUE;
}

gpointer
rbgobj_boxed_get_default(VALUE obj, GType gtype)
{
    boxed_holder *holder;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(obj, GTYPE2CLASS(gtype))))
        rb_raise(rb_eArgError,
                 "invalid argument %" PRIsVALUE " (expect %" PRIsVALUE ")",
                 CLASS_OF(obj),
                 GTYPE2CLASS(gtype));

    holder = rbgobj_boxed_get_raw(obj);
    if (!holder->boxed)
        rb_raise(rb_eArgError, "uninitialize %" PRIsVALUE,
                 CLASS_OF(obj));

    return holder->boxed;
}

gpointer
rbgobj_boxed_get(VALUE obj, GType gtype)
{
    gpointer boxed = NULL;

    if (NIL_P(obj))
        return NULL;

    if (rbgobj_convert_robj2instance(gtype, obj, &boxed)) {
        return boxed;
    }

    return rbgobj_boxed_get_default(obj, gtype);
}

VALUE
rbgobj_make_boxed_raw(gpointer p, GType gtype, VALUE klass, gint flags)
{
    VALUE result;
    boxed_holder *holder;

    result = rbgobj_boxed_alloc_func(klass);

    holder = rbgobj_boxed_get_raw(result);

    if (flags & RBGOBJ_BOXED_NOT_COPY) {
        holder->boxed = p;
        holder->own   = FALSE;
    } else {
        holder->boxed = g_boxed_copy(gtype, p);
        holder->own   = TRUE;
    }

    return result;
}

VALUE
rbgobj_make_boxed_default(gpointer p, GType gtype)
{
    const RGObjClassInfo *cinfo;

    cinfo = GTYPE2CINFO(gtype);
    return rbgobj_make_boxed_raw(p, gtype, cinfo->klass, cinfo->flags);
}

VALUE
rbgobj_make_boxed(gpointer p, GType gtype)
{
    VALUE result;

    if (!p)
        return Qnil;

    if (rbgobj_convert_instance2robj(gtype, p, &result)) {
        return result;
    }

    return rbgobj_make_boxed_default(p, gtype);
}

void
rbgobj_boxed_not_copy_obj(GType gtype)
{
    RGObjClassInfo *cinfo = (RGObjClassInfo *)GTYPE2CINFO(gtype);
    cinfo->flags |= RBGOBJ_BOXED_NOT_COPY;
}

void
rbgobj_boxed_unown(VALUE boxed)
{
    boxed_holder *holder;

    holder = rbgobj_boxed_get_raw(boxed);

    if (!holder->own) {
        rb_raise(rb_eArgError,
                 "The boxed is already unowned: %" PRIsVALUE,
                 boxed);
    }

    holder->own = FALSE;
}

/**********************************************************************/

static VALUE
boxed_to_ruby(const GValue *from)
{
    gpointer boxed;
    boxed = g_value_get_boxed(from);
    return rbgobj_make_boxed(boxed, G_VALUE_TYPE(from));
}

static void
boxed_from_ruby(VALUE from, GValue *to)
{
    gpointer boxed;
    boxed = rbgobj_boxed_get(from, G_VALUE_TYPE(to));
    g_value_set_boxed(to, boxed);
}

/**********************************************************************/

void
Init_gobject_gboxed(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_BOXED, "Boxed", mGLib);

    rbgobj_register_g2r_func(G_TYPE_BOXED, boxed_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_BOXED, boxed_from_ruby);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, rbgobj_boxed_alloc_func);
    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "gtype", generic_s_gtype, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "gtype", generic_gtype, 0);
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(inspect, 0);
    RG_DEF_METHOD(initialize_copy, 1);
    RG_DEF_ALIAS("copy", "dup");
}
