/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreeviewcolumn.c -

  $Author: mutoh $
  $Date: 2002/10/08 18:53:21 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_TREE_VIEW_COLUMN(RVAL2GOBJ(s)))
#define RVAL2CELLRENDERER(c) (GTK_CELL_RENDERER(RVAL2GOBJ(c)))

static VALUE
tvc_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    int i;
    GtkTreeViewColumn* tvc;
    gchar* name;

    if (argc > 1){
        tvc = gtk_tree_view_column_new_with_attributes(RVAL2CSTR(argv[0]), 
                                                       RVAL2CELLRENDERER(argv[1]), NULL);
    } else {
        tvc = gtk_tree_view_column_new();
    }

    RBGTK_INITIALIZE(self, tvc);

    if (argc > 2){
        for (i = 2; i < argc; i++) {
            if (SYMBOL_P(argv[i])) {
                name = rb_id2name(SYM2ID(argv[i]));
            } else {
                name = RVAL2CSTR(argv[i]);
            }
            gtk_tree_view_column_add_attribute(_SELF(self), RVAL2CELLRENDERER(argv[1]), 
                                               name, i - 2);
        }       
    }

    return Qnil;
}

static VALUE
tvc_pack_start(self, cell, expand)
    VALUE self, cell, expand;
{
    gtk_tree_view_column_pack_start(_SELF(self), RVAL2CELLRENDERER(cell), RTEST(expand));
    return self;
}

static VALUE
tvc_pack_end(self, cell, expand)
    VALUE self, cell, expand;
{
    gtk_tree_view_column_pack_end(_SELF(self), RVAL2CELLRENDERER(cell), RTEST(expand));
    return self;
}

static VALUE
tvc_clear(self)
    VALUE self;
{
    gtk_tree_view_column_clear(_SELF(self));
    return self;
}

static VALUE
tvc_get_cell_renderers(self)
    VALUE self;
{
    return GLIST2ARY(gtk_tree_view_column_get_cell_renderers(_SELF(self)));
}

static VALUE
tvc_add_attribute(self, cell, attribute, column)
    VALUE self, cell, attribute, column;
{
    gtk_tree_view_column_add_attribute(_SELF(self), RVAL2CELLRENDERER(cell), 
                                       RVAL2CSTR(attribute), NUM2INT(column));
    return self;
}

/* XXX
   void        gtk_tree_view_column_set_attributes
   (GtkTreeViewColumn *tree_column,
   GtkCellRenderer *cell_renderer,
   ...);
   void        gtk_tree_view_column_set_cell_data_func
   (GtkTreeViewColumn *tree_column,
   GtkCellRenderer *cell_renderer,
   GtkTreeCellDataFunc func,
   gpointer func_data,
   GtkDestroyNotify destroy);
*/

static VALUE
tvc_clear_attributes(self, cell)
    VALUE self, cell;
{
    gtk_tree_view_column_clear_attributes(_SELF(self), RVAL2CELLRENDERER(cell));
    return self;
}

static VALUE
tvc_set_spacing(self, spacing)
    VALUE self, spacing;
{
    gtk_tree_view_column_set_spacing(_SELF(self), NUM2INT(spacing));
    return self;
}

static VALUE
tvc_spacing_equal(self, spacing)
    VALUE self, spacing;
{
    gtk_tree_view_column_set_spacing(_SELF(self), NUM2INT(spacing));
    return spacing;
}

static VALUE
tvc_get_spacing(self)
    VALUE self;
{
    return INT2NUM(gtk_tree_view_column_get_spacing(_SELF(self)));
}

static VALUE
tvc_clicked(self)
    VALUE self;
{
    gtk_tree_view_column_clicked(_SELF(self));
    return self;
}

static VALUE
tvc_set_sort_column_id(self, sort_column_id)
    VALUE self, sort_column_id;
{
    gtk_tree_view_column_set_sort_column_id(_SELF(self), 
                                            NUM2INT(sort_column_id));
    return self;
}

static VALUE
tvc_sort_column_id_equal(self, sort_column_id)
    VALUE self, sort_column_id;
{
    gtk_tree_view_column_set_sort_column_id(_SELF(self), 
                                            NUM2INT(sort_column_id));
    return sort_column_id;
}

static VALUE
tvc_get_sort_column_id(self)
    VALUE self;
{
    return gtk_tree_view_column_get_sort_column_id(_SELF(self));
}

static VALUE
tvc_cell_set_cell_data(self, model, iter, is_expander, is_expanded)
    VALUE self, model, iter, is_expander, is_expanded;
{
    gtk_tree_view_column_cell_set_cell_data(_SELF(self), 
                                            GTK_TREE_MODEL(RVAL2GOBJ(model)),
                                            (GtkTreeIter*)RVAL2BOXED(iter, GTK_TYPE_TREE_ITER), 
                                            RTEST(is_expander), 
                                            RTEST(is_expanded));
    return self;
}

static VALUE
tvc_cell_get_size(self)
    VALUE self;
{
    GdkRectangle cell_area;
    gint x_offset, y_offset, width, height;
    VALUE cell;

    /* Is this collect implement for cell_area ? */
    cell_area.x = -1;
    cell_area.y = -1;
    cell_area.width = -1;
    cell_area.height = -1;

    gtk_tree_view_column_cell_get_size(_SELF(self), &cell_area,
                                       &x_offset, &y_offset, 
                                       &width, &height);
    if (cell_area.x == -1 || cell_area.y == -1 || 
        cell_area.width == -1 || cell_area.height == -1){
        cell = Qnil;
    } else {
        cell = BOXED2RVAL(&cell_area, GDK_TYPE_RECTANGLE);
    }
    return rb_ary_new3(5, cell,
                       x_offset ? INT2NUM(x_offset) : Qnil,
                       y_offset ? INT2NUM(y_offset) : Qnil,
                       width ? INT2NUM(width) : Qnil,
                       height ? INT2NUM(height) : Qnil);
}

static VALUE
tvc_cell_is_visible(self)
{
    return gtk_tree_view_column_cell_is_visible(_SELF(self)) ? Qtrue : Qfalse;
}

void
Init_gtk_treeviewcolumn()
{
    VALUE tvc = G_DEF_CLASS(GTK_TYPE_TREE_VIEW_COLUMN, "TreeViewColumn", mGtk);

    rb_define_method(tvc, "initialize", tvc_initialize, -1);
    rb_define_method(tvc, "pack_start", tvc_pack_start, 2);
    rb_define_method(tvc, "pack_end", tvc_pack_end, 2);
    rb_define_method(tvc, "clear", tvc_clear, 0);
    rb_define_method(tvc, "cell_renderers", tvc_get_cell_renderers, 0);
    rb_define_method(tvc, "add_attribute", tvc_add_attribute, 3);
    rb_define_method(tvc, "clear_attributes", tvc_clear_attributes, 1);
    rb_define_method(tvc, "set_spacing", tvc_set_spacing, 1);
/*    rb_define_method(tvc, "spacing=", tvc_spacing_equal, 1);
 */
    rb_define_method(tvc, "spacing", tvc_get_spacing, 0);
    rb_define_method(tvc, "clicked", tvc_clicked, 0);
    rb_define_method(tvc, "set_sort_column_id", tvc_set_sort_column_id, 1);
    rb_define_method(tvc, "sort_column_id=", tvc_sort_column_id_equal, 1);
    rb_define_method(tvc, "sort_column_id", tvc_get_sort_column_id, 0);
    rb_define_method(tvc, "cell_set_cell_data", tvc_cell_set_cell_data, 4);
    rb_define_method(tvc, "cell_size", tvc_cell_get_size, 0 );
    rb_define_method(tvc, "cell_visible?", tvc_cell_is_visible, 0);

    /* Constants */
    rb_define_const(tvc, "GROW_ONLY", GTK_TREE_VIEW_COLUMN_GROW_ONLY); 
    rb_define_const(tvc, "AUTOSIZE", GTK_TREE_VIEW_COLUMN_AUTOSIZE); 
    rb_define_const(tvc, "FIXED", GTK_TREE_VIEW_COLUMN_FIXED); 

    rb_funcall(mGLib, rb_intern("__add_one_arg_setter"), 1, tvc);
}
