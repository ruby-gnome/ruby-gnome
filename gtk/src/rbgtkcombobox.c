/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcombobox.c -

  $Author: mutoh $
  $Date: 2005/01/29 11:44:14 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(self) (GTK_COMBO_BOX(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))

static VALUE
combobox_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE model_or_false;
    GtkWidget* widget;

    rb_scan_args(argc, argv, "01", &model_or_false);

    if (rb_obj_is_kind_of(model_or_false, GTYPE2CLASS(GTK_TYPE_TREE_MODEL))){
        widget = gtk_combo_box_new_with_model(GTK_TREE_MODEL(RVAL2GOBJ(model_or_false)));
    } else if (NIL_P(model_or_false) || TYPE(model_or_false) == T_TRUE){
        widget = gtk_combo_box_new_text();
    } else if (TYPE(model_or_false) == T_FALSE){
        widget = gtk_combo_box_new();
    } else {
        rb_raise(rb_eArgError, "invalid 2nd argument %s (except true/false or Gtk::TreeModel)", 
                 rb_class2name(CLASS_OF(model_or_false)));
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

/* Defined as properties.
void        gtk_combo_box_set_wrap_width    (GtkComboBox *combo_box,
                                             gint width);
void        gtk_combo_box_set_row_span_column
                                            (GtkComboBox *combo_box,
                                             gint row_span);
void        gtk_combo_box_set_column_span_column
                                            (GtkComboBox *combo_box,
                                             gint column_span);
gint        gtk_combo_box_get_active        (GtkComboBox *combo_box);
void        gtk_combo_box_set_active        (GtkComboBox *combo_box,
                                             gint index);
*/

static VALUE
combobox_get_active_iter(self)
    VALUE self;
{
    GtkTreeIter iter;
    VALUE val = Qnil;
    gboolean ret = gtk_combo_box_get_active_iter(_SELF(self), &iter);
    if (ret){
        iter.user_data3 = (gpointer)gtk_combo_box_get_model(_SELF(self));
        val = BOXED2RVAL(&iter, GTK_TYPE_TREE_ITER);
    }
    return val;
}

static VALUE
combobox_set_active_iter(self, iter)
    VALUE self, iter;
{
    gtk_combo_box_set_active_iter(_SELF(self), RVAL2BOXED(iter, GTK_TYPE_TREE_ITER));
    return self;
}

/* Defined as properties 
GtkTreeModel* gtk_combo_box_get_model       (GtkComboBox *combo_box);
void        gtk_combo_box_set_model         (GtkComboBox *combo_box,
                                             GtkTreeModel *model);
*/


static VALUE
combobox_append_text(self, text)
    VALUE self, text;
{
    gtk_combo_box_append_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
combobox_insert_text(self, position, text)
    VALUE self, position, text;
{
    gtk_combo_box_insert_text(_SELF(self), NUM2INT(position), RVAL2CSTR(text));
    return self;
}

static VALUE
combobox_prepend_text(self, text)
    VALUE self, text;
{
    gtk_combo_box_prepend_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
combobox_remove_text(self, position)
    VALUE self, position;
{
    gtk_combo_box_remove_text(_SELF(self), NUM2INT(position));
    return self;
}
    

static VALUE
combobox_popup(self)
    VALUE self;
{
    gtk_combo_box_popup(_SELF(self));
    return self;
}

static VALUE
combobox_popdown(self)
    VALUE self;
{
    gtk_combo_box_popdown(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
/* Defined as Property
void        gtk_combo_box_set_add_tearoffs  (GtkComboBox *combo_box,
                                             gboolean add_tearoffs);
gboolean    gtk_combo_box_get_add_tearoffs  (GtkComboBox *combo_box);
gint        gtk_combo_box_get_column_span_column
                                            (GtkComboBox *combo_box);
gint        gtk_combo_box_get_wrap_width    (GtkComboBox *combo_box);

gboolean    gtk_combo_box_get_focus_on_click
                                            (GtkComboBox *combo);
gint        gtk_combo_box_get_row_span_column
                                            (GtkComboBox *combo_box);
 */

static VALUE
combobox_get_active_text(self)
    VALUE self;
{
    return CSTR2RVAL2(gtk_combo_box_get_active_text(_SELF(self)));
}

static VALUE
combobox_get_popup_accessible(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_combo_box_get_popup_accessible(_SELF(self)));
}

/* How can I implement this?
GtkTreeViewRowSeparatorFunc gtk_combo_box_get_row_separator_func
                                            (GtkComboBox *combo_box);
*/

static gboolean
row_separator_func(model, iter, func)
    GtkTreeModel* model;
    GtkTreeIter* iter;
    gpointer* func;
{  
    iter->user_data3 = model;
    return RTEST(rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(model),
                            BOXED2RVAL(iter, GTK_TYPE_TREE_ITER)));
}

static VALUE
combobox_set_row_separator_func(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_combo_box_set_row_separator_func(_SELF(self), 
                                         (GtkTreeViewRowSeparatorFunc)row_separator_func,
                                         (gpointer)func, NULL);
    return self;
}


/* This calls g_object_notify(combo, "focus_on_click");
   If you want to set "focus_on_click" property only, 
   call GLib::Object.set_property.
*/
static VALUE
combobox_set_focus_on_click(self, val)
    VALUE self, val;
{
    gtk_combo_box_set_focus_on_click(_SELF(self), RTEST(val));
    return self;
}

#endif
#endif

void 
Init_gtk_combobox()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE gCombobox = G_DEF_CLASS(GTK_TYPE_COMBO_BOX, "ComboBox", mGtk);

    rb_define_method(gCombobox, "initialize", combobox_initialize, -1);
    rb_define_method(gCombobox, "active_iter", combobox_get_active_iter, 0);
    rb_define_method(gCombobox, "set_active_iter", combobox_set_active_iter, 1);
    rb_define_method(gCombobox, "append_text", combobox_append_text, 1);
    rb_define_method(gCombobox, "insert_text", combobox_insert_text, 2);
    rb_define_method(gCombobox, "prepend_text", combobox_prepend_text, 1);
    rb_define_method(gCombobox, "remove_text", combobox_remove_text, 1);
    rb_define_method(gCombobox, "popup", combobox_popup, 0);
    rb_define_method(gCombobox, "popdown", combobox_popdown, 0);

#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gCombobox, "active_text", combobox_get_active_text, 0);
    rb_define_method(gCombobox, "popup_accessible", combobox_get_popup_accessible, 0);
    rb_define_method(gCombobox, "set_row_separator_func", combobox_set_row_separator_func, 0);
    rb_define_method(gCombobox, "set_focus_on_click", combobox_set_focus_on_click, 1);
#endif
 
    G_DEF_SETTERS(gCombobox);
#endif
}
