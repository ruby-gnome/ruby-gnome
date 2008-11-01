/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcontainer.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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
    return CBOOL2RVAL(GTK_IS_RESIZE_CONTAINER(_SELF(self)));
}

static VALUE
cont_remove(self, other)
    VALUE self, other;
{
    G_CHILD_REMOVE(self, other);
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
        callback = rb_block_proc();
    }
    gtk_container_foreach(_SELF(self), exec_callback, (gpointer)callback);
    return self;
}

static VALUE
cont_forall(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE callback;

    rb_scan_args(argc, argv, "01", &callback);
    if (NIL_P(callback)) {
        callback = rb_block_proc();
    }
    gtk_container_forall(_SELF(self), exec_callback, (gpointer)callback);
    return self;
}

static VALUE
cont_get_children(self)
    VALUE self;
{
   return GLIST2ARYF(gtk_container_get_children(_SELF(self)));
}

static VALUE
cont_set_reallocate_redraws(self, needs_redraws)
    VALUE self, needs_redraws;
{
    gtk_container_set_reallocate_redraws(_SELF(self), RVAL2CBOOL(needs_redraws));
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
                                        NIL_P(adjustment) ? NULL : GTK_ADJUSTMENT(RVAL2GOBJ(adjustment)));
    return self;
}

static VALUE
cont_set_focus_hadjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_container_set_focus_hadjustment(_SELF(self),
                                        NIL_P(adjustment) ? NULL : GTK_ADJUSTMENT(RVAL2GOBJ(adjustment)));
    return self;
}

static VALUE
cont_get_focus_vadjustment(self)
    VALUE self;
{
    GtkAdjustment* adj = gtk_container_get_focus_vadjustment(_SELF(self));
    return adj ? GOBJ2RVAL(adj) : Qnil;
}

static VALUE
cont_get_focus_hadjustment(self)
    VALUE self;
{
    GtkAdjustment* adj = gtk_container_get_focus_hadjustment(_SELF(self));
    return adj ? GOBJ2RVAL(adj) : Qnil;
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

        G_CHILD_ADD(child, ret);

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

        G_CHILD_ADD(child, val);

        gtk_container_child_set_property(GTK_CONTAINER(RVAL2GOBJ(self)), 
                                         GTK_WIDGET(RVAL2GOBJ(child)), name, &gval);

        g_value_unset(&gval);
        return self;
    }
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
    gtk_widget_freeze_child_notify(child);
    gtk_container_add(_SELF(self), child);

    G_CHILD_ADD(self, other);

    if (child->parent && (! NIL_P(properties))){
        int i;
        VALUE ary;
        GObject* obj;
                                                                                
        Check_Type(properties, T_HASH);
        ary = rb_funcall(properties, rb_intern("to_a"), 0);
        obj = RVAL2GOBJ(self);
        
        for (i = 0; i < RARRAY_LEN(ary); i++) {
            cont_child_set_property(self, other, 
                       RARRAY_PTR(RARRAY_PTR(ary)[i])[0],
                       RARRAY_PTR(RARRAY_PTR(ary)[i])[1]);
        }
    }
    gtk_widget_thaw_child_notify(child);
    return self;
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
    ret = gtk_container_get_focus_chain(_SELF(self), &glist);
    return GLIST2ARYF(glist);
}

static VALUE
cont_set_focus_chain(self, focusable_widgets) 
    VALUE self, focusable_widgets;
{
    int i;
    GList *glist = NULL;

    Check_Type(focusable_widgets, T_ARRAY);
    for (i = 0; i < RARRAY_LEN(focusable_widgets); i++) {
        glist = g_list_append(glist, RVAL2GOBJ(RARRAY_PTR(focusable_widgets)[i]));
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

static VALUE
cont_s_install_child_property(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    GtkContainerClass* gclass;
    VALUE spec, prop_id;
    GParamSpec* pspec;

    rb_scan_args(argc, argv, "11", &spec, &prop_id);

    pspec = G_PARAM_SPEC(RVAL2GOBJ(spec));

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registerd class",
                 rb_class2name(self));

    gclass = GTK_CONTAINER_CLASS(g_type_class_ref(cinfo->gtype));
    gtk_container_class_install_child_property(gclass, 
                                               NIL_P(prop_id) ? 1 : NUM2UINT(prop_id),
                                               pspec);

    return self;
}

static VALUE
cont_s_child_properties(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    GObjectClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    guint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary;
    int i;

    if (rb_scan_args(argc, argv, "01", &inherited_too) == 0)
        inherited_too = Qtrue;

    props = gtk_container_class_list_child_properties(oclass, &n_properties);

    ary = rb_ary_new();
    for (i = 0; i < n_properties; i++){
        if (RVAL2CBOOL(inherited_too)
            || GTYPE2CLASS(props[i]->owner_type) == self)
            rb_ary_push(ary, CSTR2RVAL(props[i]->name));
    }
    g_free(props);
    g_type_class_unref(oclass);
    return ary;
}

/**********************************************************************/

static VALUE proc_mod_eval;
static GQuark q_ruby_setter;
static GQuark q_ruby_getter;

static VALUE
cont_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE params_hash;
    GObject* gobj;
    
    rb_scan_args(argc, argv, "01", &params_hash);

    if (!NIL_P(params_hash))
        Check_Type(params_hash, T_HASH);

    gobj = rbgobj_gobject_new(RVAL2GTYPE(self), params_hash);

    RBGTK_INITIALIZE(self, gobj);
    return Qnil;
}

// FIXME: use rb_protect
static void
get_prop_func(GObject* object,
              guint property_id,
              GValue* value,
              GParamSpec* pspec)
{
    ID ruby_getter = (ID)g_param_spec_get_qdata(pspec, q_ruby_getter);
    if (!ruby_getter) {
        gchar* name = g_strdup(g_param_spec_get_name(pspec));
        gchar* p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_getter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_getter, (gpointer)ruby_getter);
        g_free(name);
    }

    {
        VALUE ret = rb_funcall(GOBJ2RVAL(object), ruby_getter, 0);
        rbgobj_rvalue_to_gvalue(ret, value);
    }
}

// FIXME: use rb_protect
static void
set_prop_func(GObject* object,
              guint property_id,
              const GValue* value,
              GParamSpec* pspec)
{
    ID ruby_setter = (ID)g_param_spec_get_qdata(pspec, q_ruby_setter);
    if (!ruby_setter) {
        gchar* name = g_strconcat(g_param_spec_get_name(pspec), "=", NULL);
        gchar* p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_setter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_setter, (gpointer)ruby_setter);
        g_free(name);
    }

    rb_funcall(GOBJ2RVAL(object), ruby_setter, 1, GVAL2RVAL(value));
}

// FIXME: use rb_protect
static void
get_child_prop_func(GtkContainer* container,
                    GtkWidget*    child,
                    guint         property_id,
                    GValue*       value,
                    GParamSpec*   pspec)
{
    ID ruby_getter = (ID)g_param_spec_get_qdata(pspec, q_ruby_getter);
    if (!ruby_getter) {
        gchar* name = g_strconcat("get_", g_param_spec_get_name(pspec), NULL);
        gchar* p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_getter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_getter, (gpointer)ruby_getter);
        g_free(name);
    }

    {
        VALUE ret = rb_funcall(GOBJ2RVAL(container), ruby_getter, 1, GOBJ2RVAL(child));
        rbgobj_rvalue_to_gvalue(ret, value);
    }
}

// FIXME: use rb_protect
static void
set_child_prop_func(GtkContainer* container,
                    GtkWidget*    child,
                    guint         property_id,
                    const GValue* value,
                    GParamSpec*   pspec)
{
    ID ruby_setter = (ID)g_param_spec_get_qdata(pspec, q_ruby_setter);
    if (!ruby_setter) {
        gchar* name = g_strconcat("set_", g_param_spec_get_name(pspec), NULL);
        gchar* p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_setter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_setter, (gpointer)ruby_setter);
        g_free(name);
    }

    rb_funcall(GOBJ2RVAL(container), ruby_setter, 2, GOBJ2RVAL(child), GVAL2RVAL(value));
}

// FIXME: use rb_protect
static void
class_init_func(gpointer g_class, gpointer class_data)
{
    GObjectClass* g_class1 = G_OBJECT_CLASS(g_class);
    GtkContainerClass* g_class2 = GTK_CONTAINER_CLASS(g_class);

    g_class1->set_property = set_prop_func;
    g_class1->get_property = get_prop_func;

    g_class2->set_child_property = set_child_prop_func;
    g_class2->get_child_property = get_child_prop_func;

#if 0
    VALUE class_init_proc = (VALUE)class_data;
    rb_funcall(proc_mod_eval, rb_intern("call"), 2,
               GTYPE2CLASS(G_TYPE_FROM_CLASS(g_class)), class_init_proc);
#endif
}

static VALUE
type_register(int argc, VALUE* argv, VALUE self)
{
    VALUE type_name, flags;
    volatile VALUE class_init_proc = Qnil;
    GType parent_type;
    GTypeInfo* info;

    rb_scan_args(argc, argv, "03", &type_name, &info, &flags);

    {
        const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
        if (cinfo->klass == self)
            rb_raise(rb_eTypeError, "already registered");
    }

    {
        VALUE superclass = rb_funcall(self, rb_intern("superclass"), 0);
        const RGObjClassInfo* cinfo = CLASS2CINFO(superclass);
        if (cinfo->klass != superclass)
            rb_raise(rb_eTypeError, "super class must be registered to GLib");
        parent_type = cinfo->gtype;
    }

    if (NIL_P(type_name)){
        VALUE s = rb_funcall(self, rb_intern("name"), 0);

        if (strlen(StringValuePtr(s)) == 0)
            rb_raise(rb_eTypeError, "can't determine type name");        

        type_name = rb_funcall(
            rb_eval_string("lambda{|x| x.gsub(/::/,'') }"),
            rb_intern("call"), 1, s);
    }

    {
        GTypeQuery query;
        g_type_query(parent_type, &query);

        info = g_new0(GTypeInfo, 1);
        info->class_size     = query.class_size;
        info->base_init      = NULL;
        info->base_finalize  = NULL;
        info->class_init     = class_init_func;
        info->class_finalize = NULL;
        info->class_data     = (gpointer)class_init_proc;
        info->instance_size  = query.instance_size;
        info->n_preallocs    = 0;
        info->instance_init  = NULL;
        info->value_table    = NULL;
    }

    {
        GType type = g_type_register_static(parent_type,
                                            StringValuePtr(type_name),
                                            info,
                                            NIL_P(flags) ? 0 : NUM2INT(flags));
        G_RELATIVE(self, class_init_proc);

        rbgobj_register_class(self, type, TRUE, TRUE);

        {
            RGObjClassInfo* cinfo = (RGObjClassInfo*)rbgobj_lookup_class(self);
            cinfo->flags |= RBGOBJ_DEFINED_BY_RUBY;
        }

        {
            GType parent = g_type_parent(type);
            const RGObjClassInfo* cinfo = GTYPE2CINFO(parent);
            VALUE m = rb_define_module_under(self, RubyGtkContainerHookModule);

            if (! (cinfo->flags & RBGOBJ_DEFINED_BY_RUBY)) {
                rb_define_method(m, "initialize", cont_initialize, -1);
            }

            rb_include_module(self, m);
        }

        return Qnil;
    }
}
/**********************************************************************/

static void
cont_mark_callback(GtkWidget* w, gpointer data)
{
    rbgobj_gc_mark_instance(w);
}

static void
cont_mark(void* p)
{
    gtk_container_forall(GTK_CONTAINER(p), cont_mark_callback, NULL);
}

/**********************************************************************/

void 
Init_gtk_container()
{
    VALUE gContainer;

    gContainer = G_DEF_CLASS_WITH_GC_FUNC(GTK_TYPE_CONTAINER, "Container",
					  mGtk, cont_mark, NULL);
    rb_define_method(gContainer, "resize_container?", cont_is_resize_container, 0);
    rb_define_method(gContainer, "add", cont_add, -1);
    rb_define_alias(gContainer, "<<", "add");
    rb_define_method(gContainer, "remove", cont_remove, 1);
    rb_define_method(gContainer, "check_resize", cont_check_resize, 0);
    rb_define_method(gContainer, "each", cont_foreach, -1);
    rb_define_method(gContainer, "each_forall", cont_forall, -1);
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
    rb_define_singleton_method(gContainer, "install_child_property", cont_s_install_child_property, -1);

    q_ruby_getter = g_quark_from_static_string("__ruby_getter");
    q_ruby_setter = g_quark_from_static_string("__ruby_setter");

    rb_define_singleton_method(gContainer, "type_register", type_register, -1);

    rb_global_variable(&proc_mod_eval);
    proc_mod_eval = rb_eval_string("lambda{|obj,proc| obj.module_eval(&proc)}");

    G_DEF_SETTERS(gContainer);

    rb_global_variable(&type_to_prop_setter_table);
    rb_global_variable(&type_to_prop_getter_table);
    type_to_prop_setter_table = rb_hash_new();
    type_to_prop_getter_table = rb_hash_new();
}
