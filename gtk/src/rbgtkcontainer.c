/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcontainer.c -

  $Author: mutoh $
  $Date: 2002/10/19 13:20:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_CONTAINER(RVAL2GOBJ(self)))

/*
#define     GTK_IS_RESIZE_CONTAINER         (widget)
#define     GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID(object, property_id, pspec)
*/

static VALUE
cont_add(self, other)
    VALUE self, other;
{
    gtk_container_add(_SELF(self), GTK_WIDGET(RVAL2GOBJ(other)));
    return self;
}

static VALUE
cont_remove(self, other)
    VALUE self, other;
{
    gtk_container_remove(_SELF(self), GTK_WIDGET(RVAL2GOBJ(other)));
    return self;
}

/*
void        gtk_container_add_with_properties
                                            (GtkContainer *container,
                                             GtkWidget *widget,
                                             const gchar *first_prop_name,
                                             ...);
void        gtk_container_check_resize      (GtkContainer *container);
*/

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

/*
void        gtk_container_set_reallocate_redraws
                                            (GtkContainer *container,
                                             gboolean needs_redraws);
*/

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

/*
GtkAdjustment* gtk_container_get_focus_vadjustment
                                            (GtkContainer *container);
GtkAdjustment* gtk_container_get_focus_hadjustment
                                            (GtkContainer *container);
void        gtk_container_resize_children   (GtkContainer *container);
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
void        gtk_container_forall            (GtkContainer *container,
                                             GtkCallback callback,
                                             gpointer callback_data);
void        gtk_container_propagate_expose  (GtkContainer *container,
                                             GtkWidget *child,
                                             GdkEventExpose *event);
gboolean    gtk_container_get_focus_chain   (GtkContainer *container,
                                             GList **focusable_widgets);
void        gtk_container_set_focus_chain   (GtkContainer *container,
                                             GList *focusable_widgets);
void        gtk_container_unset_focus_chain (GtkContainer *container);
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

    rb_define_method(gContainer, "add", cont_add, 1);
    rb_define_method(gContainer, "remove", cont_remove, 1);
    rb_define_method(gContainer, "each", cont_foreach, -1);
    rb_define_method(gContainer, "children", cont_get_children, 0);
    rb_define_method(gContainer, "set_focus_child", cont_set_focus_child, 1);
    rb_define_method(gContainer, "set_focus_vadjustment", cont_set_focus_vadjustment, 1);
    rb_define_method(gContainer, "set_focus_hadjustment", cont_set_focus_hadjustment, 1);
}
