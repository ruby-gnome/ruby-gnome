/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcontainer.c -

  $Author: mutoh $
  $Date: 2003/04/23 16:44:08 $

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
        callback = rb_f_lambda();
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

/* We don't need them because you can get child itself easily.
GtkType     gtk_container_child_type        (GtkContainer *container);
void        gtk_container_child_get         (GtkContainer *container,
                                             GtkWidget *child,
                                             const gchar *first_prop_name,
                                             ...);
void        gtk_container_child_set         (GtkContainer *container,
                                             GtkWidget *child,
                                             const gchar *first_prop_name,
                                             ...);
void        gtk_container_child_get_property
                                            (GtkContainer *container,
                                             GtkWidget *child,
                                             const gchar *property_name,
                                             GValue *value);
void        gtk_container_child_set_property
                                            (GtkContainer *container,
                                             GtkWidget *child,
                                             const gchar *property_name,
                                             const GValue *value);
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

/* We don't need them because you can get child itself easily.
GParamSpec* gtk_container_class_find_child_property
                                            (GObjectClass *cclass,
                                             const gchar *property_name);
void        gtk_container_class_install_child_property
                                            (GtkContainerClass *cclass,
                                             guint property_id,
                                             GParamSpec *pspec);
GParamSpec** gtk_container_class_list_child_properties
                                            (GObjectClass *cclass,
                                             guint *n_properties);
*/

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
    rb_define_method(gContainer, "propagate_expose", cont_propagate_expose, 2);
    rb_define_method(gContainer, "focus_chain", cont_get_focus_chain, 0);
    rb_define_method(gContainer, "set_focus_chain", cont_set_focus_chain, 1);
    rb_define_method(gContainer, "unset_focus_chain", cont_unset_focus_chain, 0);

    G_DEF_SETTERS(gContainer);
}
