/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcombobox.c -

  $Author: mutoh $
  $Date: 2004/05/30 16:41:13 $

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
 
    G_DEF_SETTERS(gCombobox);
#endif
}
