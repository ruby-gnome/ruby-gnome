/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcontainer.c -

  $Author: mutoh $
  $Date: 2003/06/26 15:15:32 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_CONTAINER(RVAL2GOBJ(self)))

/*
We don't need this.
#define     GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID(object, property_id, pspec)
*/

static VALUE type_to_prop_setter_table;
static VALUE type_to_prop_getter_table;

static VALUE
cont_is_resize_container(self)
    VALUE self;
{
    return GTK_IS_RESIZE_CONTAINER(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
cont_add(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE other, properties;
    GtkWidget *child;

    rb_scan_args(argc, argv, "11", &other, &properties);

    child = GTK_WIDGET(RVAL2GOBJ(other));
    gtk_container_add(_SELF(self), child);
    if (! NIL_P(properties)){
        G_SET_PROPERTIES(other, properties);
    }
    return self;
}

static VALUE
cont_remove(self, other)
    VALUE self, other;
{
    gtk_container_remove(_SELF(self), GTK_WIDGET(RVAL2GOBJ(other)));
    return self;
}

static VALUE
cont_check_resize(self)
    VALUE self;
{
    gtk_container_check_resize(_SELF(self));
    return self;
}

static VALUE
cont_foreach(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE callback;

    rb_scan_args(argc, argv, "01", &callback);
    if (NIL_P(callback)) {
        callback = G_BLOCK_PROC();
    }
    gtk_container_foreach(_SELF(self), exec_callback, (gpointer)callback);
    return self;
}

static VALUE
cont_get_children(self)
    VALUE self;
{
   return GLIST2ARY(gtk_container_get_children(_SELF(self)));
}

static VALUE
cont_set_reallocate_redraws(self, needs_redraws)
    VALUE self, needs_redraws;
{
    gtk_container_set_reallocate_redraws(_SELF(self), RTEST(needs_redraws));
    return self;
}


static VALUE
cont_set_focus_child(self, child)
    VALUE self, child;
{
    gtk_container_set_focus_child(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
cont_set_focus_vadjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_container_set_focus_vadjustment(_SELF(self),
                                        GTK_ADJUSTMENT(RVAL2GOBJ(adjustment)));
    return self;
}

static VALUE
cont_set_focus_hadjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_container_set_focus_hadjustment(_SELF(self),
                                        GTK_ADJUSTMENT(RVAL2GOBJ(adjustment)));
    return self;
}

static VALUE
cont_get_focus_vadjustment(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_container_get_focus_vadjustment(_SELF(self)));
}

static VALUE
cont_get_focus_hadjustment(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_container_get_focus_hadjustment(_SELF(self)));
}

static VALUE
cont_get_resize_children(self)
    VALUE self;
{
    gtk_container_resize_children(_SELF(self));
    return self;
}

static VALUE
cont_get_child_type(self)
    VALUE self;
{
    return GTYPE2CLASS(gtk_container_child_type(_SELF(self)));
}

/* 
void        gtk_container_child_get         (GtkContainer *container,
                                             GtkWidget *child,
                                             const gchar *first_prop_name,
                                             ...);
void        gtk_container_child_set         (GtkContainer *container,
                                             GtkWidget *child,
                                             const gchar *first_prop_name,
                                             ...);
*/

void
rbgtkcontainer_register_child_property_setter(gtype, name, func)
    GType gtype;
    const char* name;
    RValueToGValueFunc func;
{
    GObjectClass* oclass;
    GParamSpec* pspec;

    VALUE table = rb_hash_aref(type_to_prop_setter_table, INT2FIX(gtype));
    if (NIL_P(table)){
        table = rb_hash_new();
        rb_hash_aset(type_to_prop_setter_table, INT2FIX(gtype), table);
    }

    oclass = g_type_class_ref(gtype);
    pspec = gtk_container_class_find_child_property(oclass, name);

    rb_hash_aset(table, rb_intern(g_param_spec_get_name(pspec)),
                 Data_Wrap_Struct(rb_cData, NULL, NULL, func));

    g_type_class_unref(oclass);
}

void
rbgtkcontainer_register_child_property_getter(gtype, name, func)
    GType gtype;
    const char* name;
    GValueToRValueFunc func;
{
    GObjectClass* oclass;
    GParamSpec* pspec;

    VALUE table = rb_hash_aref(type_to_prop_getter_table, INT2FIX(gtype));
    if (NIL_P(table)){
        table = rb_hash_new();
        rb_hash_aset(type_to_prop_getter_table, INT2FIX(gtype), table);
    }

    oclass = g_type_class_ref(gtype);
    pspec = gtk_container_class_find_child_property(oclass, name);

    rb_hash_aset(table, rb_intern(g_param_spec_get_name(pspec)),
                 Data_Wrap_Struct(rb_cData, NULL, NULL, func));
}

static VALUE
cont_child_get_property(self, child, prop_name)
    VALUE self, child, prop_name;
{
    GParamSpec* pspec;
    const char* name;

    if (SYMBOL_P(prop_name)) {
        name = rb_id2name(SYM2ID(prop_name));
    } else {
        StringValue(prop_name);
        name = StringValuePtr(prop_name);
    }

    pspec = gtk_container_class_find_child_property(
        G_OBJECT_GET_CLASS(RVAL2GOBJ(self)), name);

    if (!pspec)
        rb_raise(rb_eArgError, "No such property: %s", name);
    else {
        GValueToRValueFunc getter = NULL;
        GValue gval = {0,};
        VALUE ret;
        
        {
            VALUE table = rb_hash_aref(type_to_prop_getter_table,
                                       INT2FIX(pspec->owner_type));
            if (!NIL_P(table)){
                VALUE obj = rb_hash_aref(table, rb_intern(g_param_spec_get_name(pspec)));
                if (!NIL_P(obj))
                    Data_Get_Struct(obj, void, getter);
            }
        }
        g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));
        gtk_container_child_get_property(GTK_CONTAINER(RVAL2GOBJ(self)), 
                                         GTK_WIDGET(RVAL2GOBJ(child)),
                                         name , &gval);
        ret = getter ? getter(&gval) : GVAL2RVAL(&gval);
        g_value_unset(&gval);
        return ret;
    }
}

static VALUE
cont_child_set_property(self, child, prop_name, val)
    VALUE self, child, prop_name, val;
{
    GParamSpec* pspec;
    const char* name;

    if (SYMBOL_P(prop_name)) {
        name = rb_id2name(SYM2ID(prop_name));
    } else {
        StringValue(prop_name);
        name = StringValuePtr(prop_name);
    }

    pspec = gtk_container_class_find_child_property(
        G_OBJECT_GET_CLASS(RVAL2GOBJ(self)), name);

    if (!pspec)
        rb_raise(rb_eArgError, "No such property: %s", name);
    else {
        RValueToGValueFunc setter = NULL;
        GValue gval = {0,};
        g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));

        {
            VALUE table = rb_hash_aref(type_to_prop_setter_table,
                                       INT2FIX(pspec->owner_type));
            if (!NIL_P(table)){
                VALUE obj = rb_hash_aref(table, rb_intern(g_param_spec_get_name(pspec)));
                if (!NIL_P(obj))
                    Data_Get_Struct(obj, void, setter);
            }
        }

        if (setter)
            setter(val, &gval);
        else {
            if (!NIL_P(val))
                rbgobj_rvalue_to_gvalue(val, &gval);
        }

        gtk_container_child_set_property(GTK_CONTAINER(RVAL2GOBJ(self)), 
                                         GTK_WIDGET(RVAL2GOBJ(child)), name, &gval);

        g_value_unset(&gval);
        return self;
    }
}
/*
void        gtk_container_child_get_valist  (GtkContainer *container,
                                             GtkWidget *child,
                                             const gchar *first_property_name,
                                             va_list var_args);
void        gtk_container_child_set_valist  (GtkContainer *container,
                                             GtkWidget *child,
                                             const gchar *first_property_name,
                                             va_list var_args);
*/
/* Use gtk_container_foreach instead
void        gtk_container_forall            (GtkContainer *container,
                                             GtkCallback callback,
                                             gpointer callback_data);
*/

static VALUE
cont_propagate_expose(self, child, event)
    VALUE self, child, event;
{
    gtk_container_propagate_expose(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)),
                                   (GdkEventExpose *)RVAL2GEV(event));
    return self;
}

static VALUE
cont_get_focus_chain(self)
    VALUE self;
{
    gboolean ret;
    GList *glist = NULL;
    VALUE result;
    ret = gtk_container_get_focus_chain(_SELF(self), &glist);

    if (ret){
        result = GLIST2ARY(glist);
        g_list_free(glist);
    } else {
        result = Qnil;
    }
    return result;
}

static VALUE
cont_set_focus_chain(self, focusable_widgets) 
    VALUE self, focusable_widgets;
{
    int i;
    GList *glist = NULL;

    Check_Type(focusable_widgets, T_ARRAY);
    for (i = 0; i < RARRAY(focusable_widgets)->len; i++) {
        glist = g_list_append(glist, RVAL2GOBJ(RARRAY(focusable_widgets)->ptr[i]));
    }
    gtk_container_set_focus_chain(_SELF(self), glist);
    g_list_free(glist);

    return self;
}

static VALUE
cont_unset_focus_chain(self)
    VALUE self;
{
    gtk_container_unset_focus_chain(_SELF(self));
    return self;
}

static VALUE
cont_s_child_property(self, property_name)
     VALUE self, property_name;
{
    GObjectClass* oclass;
    const char* name;
    GParamSpec* prop;
    VALUE result;

    if (SYMBOL_P(property_name)) {
        name = rb_id2name(SYM2ID(property_name));
    } else {
        StringValue(property_name);
        name = StringValuePtr(property_name);
    }

    oclass = g_type_class_ref(CLASS2GTYPE(self));

    prop = gtk_container_class_find_child_property(oclass, name);
    if (!prop){
        g_type_class_unref(oclass);
        rb_raise(rb_eNameError, "no such property: %s", name);
    }

    result = GOBJ2RVAL(prop);
    g_type_class_unref(oclass);
    return result;
}
/*
static VALUE
cont_s_install_child_property(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    GtkContainerClass* gclass;
    GParamSpec* pspec;
    VALUE pspec_obj, prop_id;

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registerd class",
                 rb_class2name(self));

    rb_scan_args(argc, argv, "11", &pspec_obj, &prop_id);
    pspec = G_PARAM_SPEC(RVAL2GOBJ(pspec_obj));

    gclass = (GtkContainerClass *)g_type_class_ref(cinfo->gtype);
    gtk_container_class_install_child_property(
        gclass, NIL_P(prop_id) ? 1: NUM2INT(prop_id), 
        pspec);

    return Qnil;
}
*/
static VALUE
cont_s_child_properties(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    GObjectClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    gint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary;
    int i;

    rb_scan_args(argc, argv, "01", &inherited_too);

    props = gtk_container_class_list_child_properties(oclass, &n_properties);

    ary = rb_ary_new();
    for (i = 0; i < n_properties; i++){
        if (RTEST(inherited_too)
            || GTYPE2CLASS(props[i]->owner_type) == self)
            rb_ary_push(ary, rb_str_new2(props[i]->name));
    }

    g_type_class_unref(oclass);
    return ary;
}

void 
Init_gtk_container()
{
    VALUE gContainer = G_DEF_CLASS(GTK_TYPE_CONTAINER, "Container", mGtk);

    rb_define_method(gContainer, "resize_container?", cont_is_resize_container, 0);
    rb_define_method(gContainer, "add", cont_add, -1);
    rb_define_method(gContainer, "remove", cont_remove, 1);
    rb_define_method(gContainer, "check_resize", cont_check_resize, 0);
    rb_define_method(gContainer, "each", cont_foreach, -1);
    rb_define_method(gContainer, "children", cont_get_children, 0);
    rb_define_method(gContainer, "set_reallocate_redraws", cont_set_reallocate_redraws, 1);
    rb_define_method(gContainer, "set_focus_child", cont_set_focus_child, 1);
    rb_define_method(gContainer, "set_focus_vadjustment", cont_set_focus_vadjustment, 1);
    rb_define_method(gContainer, "set_focus_hadjustment", cont_set_focus_hadjustment, 1);
    rb_define_method(gContainer, "focus_vadjustment", cont_get_focus_vadjustment, 0);
    rb_define_method(gContainer, "focus_hadjustment", cont_get_focus_hadjustment, 0);
    rb_define_method(gContainer, "resize_children", cont_get_resize_children, 0);
    rb_define_method(gContainer, "child_type", cont_get_child_type, 0);
    rb_define_method(gContainer, "child_get_property", cont_child_get_property, 2);
    rb_define_method(gContainer, "child_set_property", cont_child_set_property, 3);
    rb_define_method(gContainer, "propagate_expose", cont_propagate_expose, 2);
    rb_define_method(gContainer, "focus_chain", cont_get_focus_chain, 0);
    rb_define_method(gContainer, "set_focus_chain", cont_set_focus_chain, 1);
    rb_define_method(gContainer, "unset_focus_chain", cont_unset_focus_chain, 0);
    rb_define_singleton_method(gContainer, "child_property", cont_s_child_property, 1);
    rb_define_singleton_method(gContainer, "child_properties", cont_s_child_properties, -1);
/*
    rb_define_singleton_method(gContainer, "install_child_property", cont_s_install_child_property, -1);
*/
    G_DEF_SETTERS(gContainer);

    rb_global_variable(&type_to_prop_setter_table);
    rb_global_variable(&type_to_prop_getter_table);
    type_to_prop_setter_table = rb_hash_new();
    type_to_prop_getter_table = rb_hash_new();
}
