/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005,2006 Masao Mutoh
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

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

#define RG_TARGET_NAMESPACE cCellView
#define _SELF(self) (GTK_CELL_VIEW(RVAL2GOBJ(self)))

static ID id_model;
static ID id_text;

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
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
            if (NIL_P(with_markup) || RVAL2CBOOL(with_markup)){
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
rg_set_displayed_row(VALUE self, VALUE path)
{
    gtk_cell_view_set_displayed_row(_SELF(self),  
                                    NIL_P(path) ? (GtkTreePath*)NULL :
                                    RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
rg_displayed_row(VALUE self)
{
    return GTKTREEPATH2RVAL(gtk_cell_view_get_displayed_row(_SELF(self)));
}

static VALUE
rg_get_size_of_row(VALUE self, VALUE path)
{
    GtkRequisition req;
    gboolean ret = gtk_cell_view_get_size_of_row(_SELF(self),  
                                                 RVAL2GTKTREEPATH(path),
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
rg_cell_renderers(VALUE self)
{
    return GLIST2ARYF(gtk_cell_view_get_cell_renderers(_SELF(self)));
}
#endif

void
Init_gtk_cellview(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_CELL_VIEW, "CellView", mGtk);

    id_model = rb_intern("model");
    id_text = rb_intern("text");

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_displayed_row, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "displayed_row");
    RG_DEF_METHOD(displayed_row, 0);
    RG_DEF_METHOD(get_size_of_row, 1);
    RG_DEF_METHOD(cell_renderers, 0);
#endif
}
