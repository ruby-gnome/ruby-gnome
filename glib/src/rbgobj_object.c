/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_object.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:38:47 $

  Copyright (C) 2002  Masahiro Sakai

  This file is derived from rbgtkobject.c in Ruby/Gtk distribution.
  rbgtkobject.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi

**********************************************************************/

#include "ruby.h"
#include "st.h"
#include "global.h"

VALUE rbgobj_cGObject;


static VALUE
gobj_s_allocate(klass)
    VALUE klass;
{
    return rbgobj_create_object(klass);
}

#ifdef HAVE_OBJECT_ALLOCATE
#define rbgobj_s_new rb_class_new_instance
#else
static VALUE 
gobj_s_new(int argc, VALUE* argv, VALUE klass)
{
    VALUE obj = gobj_s_allocate(klass);
    rb_obj_call_init(obj, argc, argv);
    return obj;
}
#endif

static VALUE
gobj_s_gobject_new(self, type, params_hash)
    VALUE self, type, params_hash;
{
    GObject* gobj = rbgobj_gobject_new(type, params_hash);
    VALUE result = GOBJ2RVAL(gobj);

    // XXX: Ughhhhh
    {
        static GType gtype_gtkobject = G_TYPE_INVALID;
        if (!gtype_gtkobject)
            gtype_gtkobject = g_type_from_name("GtkObject");

        if (gtype_gtkobject && g_type_is_a(G_OBJECT_TYPE(gobj), gtype_gtkobject)){
            // We can't call gtk_object_sink() here.
            // But hopefully someone will call it.
            //gtk_object_sink(gobj);
        } else {
            g_object_unref(gobj);
        }
    }

    return result;
}

static VALUE
gobj_set_property(self, prop_name, val)
    VALUE self, prop_name, val;
{
    GParamSpec* pspec;

    StringValue(prop_name);

    pspec = g_object_class_find_property(
        G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
        StringValuePtr(prop_name));

    if (!pspec)
        rb_raise(rb_eArgError, "No such property");
    else {
        GValue tmp = {0,};
        g_value_init(&tmp, G_PARAM_SPEC_VALUE_TYPE(pspec));
        rbgobj_rvalue_to_gvalue(val, &tmp);
        g_object_set_property(RVAL2GOBJ(self), StringValuePtr(prop_name), &tmp);
        g_value_unset(&tmp);
        return self;
    }
}

static VALUE
gobj_get_property(self, prop_name)
    VALUE self, prop_name;
{
    GParamSpec* pspec;

    StringValue(prop_name);

    pspec = g_object_class_find_property(
        G_OBJECT_GET_CLASS(RVAL2GOBJ(self)),
        StringValuePtr(prop_name));

    if (!pspec)
        rb_raise(rb_eArgError, "No such property");
    else {
        GValue tmp = {0,};
        VALUE ret;
        g_value_init(&tmp, G_PARAM_SPEC_VALUE_TYPE(pspec));
        g_object_get_property(RVAL2GOBJ(self), StringValuePtr(prop_name), &tmp);
        ret = rbgobj_gvalue_to_rvalue(&tmp);
        g_value_unset(&tmp);
        return ret;
    }
}

static VALUE
gobj_freeze_notify(self)
    VALUE self;
{
    g_object_freeze_notify(RVAL2GOBJ(self));
    return self;
}

static VALUE
gobj_notify(self, property_name)
    VALUE self, property_name;
{
    StringValue(property_name);
    g_object_notify(RVAL2GOBJ(self), StringValuePtr(property_name));
    return self;
}

static VALUE
gobj_thaw_notify(self)
    VALUE self;
{
    g_object_thaw_notify(RVAL2GOBJ(self));
    return self;
}

static VALUE
gobj_inspect(self)
    VALUE self;
{
    gobj_holder* holder;
    char *cname = rb_class2name(CLASS_OF(self));
    char *s;
    VALUE result;

    Data_Get_Struct(self, gobj_holder, holder);

    if (!holder->destroyed)
        s = g_strdup_printf("#<%s:%p ptr=%p>", cname, (void *)self, holder->gobj);
    else
        s = g_strdup_printf("#<%s:%p destroyed>", cname, (void *)self);

    result = rb_str_new2(s);
    g_free(s);

    return result;
}

static VALUE
gobj_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_raise(rb_eRuntimeError, "can't instantiate class %s",
             rb_class2name(CLASS_OF(self)));
}

static VALUE
gobj_get_g_type(self)
    VALUE self;
{
    return INT2NUM(G_OBJECT_TYPE(RVAL2GOBJ(self)));
}

static VALUE
null()
{
    return (VALUE)NULL;
}

static VALUE
gobj_ref_count(self)
    VALUE self;
{
    GObject* gobj = (GObject*)rb_rescue((VALUE(*)())rbgobj_get_gobject, self, null, 0);
    return INT2NUM(gobj ? gobj->ref_count : 0);
}

static VALUE
gobj_clone(self)
    VALUE self;
{
    rb_raise(rb_eTypeError, "can't clone %s", rb_class2name(CLASS_OF(self)));
}

static VALUE
gobj_smethod_added(self, id)
    VALUE self, id;
{
    GObject *obj = RVAL2GOBJ(self);
    const char* name = rb_id2name(SYM2ID(id));
    
    if (g_signal_lookup(name, G_OBJECT_TYPE(obj))) {
        VALUE method = rb_funcall(self, rb_intern("method"), id);
        VALUE args[2] = {method, Qfalse};
        gobj_sig_connect(self, sizeof(args)/sizeof(args[0]), args);
    }

    return Qnil;
}

/**********************************************************************/

static VALUE
_gobject_to_ruby(const GValue* from)
{
    return GOBJ2RVAL(g_value_get_object(from));
}

static void
_gobject_from_ruby(VALUE from, GValue* to)
{
    g_value_set_object(to, RVAL2GOBJ(from));
}

/**********************************************************************/

void 
Init_gobject_gobject()
{
    static RGObjClassInfo cinfo;

    rbgobj_cGObject = rb_define_class_under(mGLib, "GObject", rb_cObject);
    cinfo.klass = rbgobj_cGObject;
    cinfo.gtype = G_TYPE_OBJECT;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgobj_register_class(&cinfo);
    rbgobj_register_r2g_func(rbgobj_cGObject, _gobject_from_ruby);
    rbgobj_register_g2r_func(G_TYPE_OBJECT, _gobject_to_ruby);

#ifdef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(rbgobj_cGObject, "allocate", &gobj_s_allocate, 0);
#else
    rb_define_singleton_method(rbgobj_cGObject, "new", &gobj_s_new, -1);
#endif

    rb_define_singleton_method(rbgobj_cGObject, "gobject_new", gobj_s_gobject_new, 2);
    rb_define_method(rbgobj_cGObject, "set_property", gobj_set_property, 2);
    rb_define_method(rbgobj_cGObject, "get_property", gobj_get_property, 1);
    rb_define_alias(rbgobj_cGObject, "property", "get_property");
    rb_define_method(rbgobj_cGObject, "freeze_notify", gobj_freeze_notify, 0);
    rb_define_method(rbgobj_cGObject, "notify", gobj_notify, 1);
    rb_define_method(rbgobj_cGObject, "thaw_notify", gobj_thaw_notify, 0);

    rb_define_method(rbgobj_cGObject, "initialize", gobj_initialize, -1);
    rb_define_method(rbgobj_cGObject, "g_type", gobj_get_g_type, 0);
    rb_define_method(rbgobj_cGObject, "ref_count", gobj_ref_count, 0); /* for debugging */
    rb_define_method(rbgobj_cGObject, "inspect", gobj_inspect, 0);
    rb_define_method(rbgobj_cGObject, "clone", gobj_clone, 0);

    rb_define_method(rbgobj_cGObject, "singleton_method_added", gobj_smethod_added, 1);
}

