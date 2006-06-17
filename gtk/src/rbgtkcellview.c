/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcellview.c -

  $Author: mutoh $
  $Date: 2006/06/17 06:59:32 $

  Copyright (C) 2005,2006 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

#define _SELF(self) (GTK_CELL_VIEW(RVAL2GOBJ(self)))

static ID id_model;
static ID id_text;

static VALUE
cview_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE text;
    VALUE with_markup = Qnil;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "02", &text, &with_markup);
    
    if (NIL_P(text)) {
        widget = gtk_cell_view_new();
    } else {
        G_CHILD_SET(self, id_text, text);
        if (TYPE(text) == T_STRING){
            if (NIL_P(with_markup) || RTEST(with_markup)){
                widget = gtk_cell_view_new_with_markup(RVAL2CSTR(text));
            } else {
                widget = gtk_cell_view_new_with_text(RVAL2CSTR(text));
            }
        } else if (rb_obj_is_kind_of(text, GTYPE2CLASS(GDK_TYPE_PIXBUF))){
            widget = gtk_cell_view_new_with_pixbuf(GDK_PIXBUF(RVAL2GOBJ(text)));
        } else {
            rb_raise(rb_eArgError, 
                     "invalid argument %s (expect String or Gdk::Pixbuf)", 
                     rb_class2name(CLASS_OF(text)));
        }
    }

    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

static VALUE
cview_set_model(self, model)
    VALUE self, model;
{
    G_CHILD_SET(self, id_model, model);
    gtk_cell_view_set_model(_SELF(self), 
                            NIL_P(model) ? (GtkTreeModel*)NULL : GTK_TREE_MODEL(RVAL2GOBJ(model)));
    return self;
}

static VALUE
cview_set_displayed_row(self, path)
    VALUE self, path;
{
    gtk_cell_view_set_displayed_row(_SELF(self),  
                                    NIL_P(path) ? (GtkTreePath*)NULL :
                                    (GtkTreePath*)RVAL2BOXED(path, GTK_TYPE_TREE_PATH));
    return self;
}

static VALUE
cview_get_displayed_row(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_cell_view_get_displayed_row(_SELF(self)), 
                      GTK_TYPE_TREE_PATH);
}

static VALUE
cview_get_size_of_row(self, path)
    VALUE self, path;
{
    GtkRequisition req;
    gboolean ret = gtk_cell_view_get_size_of_row(_SELF(self),  
                                                 RVAL2BOXED(path, GTK_TYPE_TREE_PATH),
                                                 &req);
    if (! ret)
        rb_raise(rb_eRuntimeError, "Can't get the value");

    return rb_ary_assoc(INT2NUM(req.width), INT2NUM(req.height));
}

/* Use Gtk::CellView#background_gdk(Gdk::Color) instead.
void        gtk_cell_view_set_background_color
                                            (GtkCellView *cell_view,
                                             const GdkColor *color);
*/

static VALUE
cview_get_cell_renderers(self)
    VALUE self;
{
    return GLIST2ARYF(gtk_cell_view_get_cell_renderers(_SELF(self)));
}
#endif

void
Init_gtk_cellview()
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE cview = G_DEF_CLASS(GTK_TYPE_CELL_VIEW, "CellView", mGtk);

    id_model = rb_intern("model");
    id_text = rb_intern("text");

    rb_define_method(cview, "initialize", cview_initialize, -1);
    rb_define_method(cview, "set_model", cview_set_model, 1);
    rb_define_method(cview, "set_displayed_row", cview_set_displayed_row, 1);
    rb_define_method(cview, "displayed_row", cview_get_displayed_row, 0);
    rb_define_method(cview, "get_size_of_row", cview_get_size_of_row, 1);
    rb_define_method(cview, "cell_renderers", cview_get_cell_renderers, 0);

    G_DEF_SETTERS(cview);
#endif
}


 

