/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreeviewcolumn.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2002-2004 Masao Mutoh
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
    int col;
    GtkTreeViewColumn *tvc;
    GtkCellRenderer *renderer;
    const gchar *name;
    VALUE ary, val;

    tvc = gtk_tree_view_column_new();
    if (argc > 0){
        gtk_tree_view_column_set_title(tvc, RVAL2CSTR(argv[0]));
        if (argc > 1) {
            gtk_tree_view_column_pack_start(tvc, RVAL2CELLRENDERER(argv[1]), TRUE);
            G_CHILD_ADD(self, argv[1]);
        }
    }

    RBGTK_INITIALIZE(self, tvc);

    if (argc == 3){
        ary = rb_funcall(argv[2], rb_intern("to_a"), 0);
        renderer = RVAL2CELLRENDERER(argv[1]);
        for (i = 0; i < RARRAY(ary)->len; i++) {
            val = RARRAY(RARRAY(ary)->ptr[i])->ptr[0];
            if (SYMBOL_P(val)) {
                name = rb_id2name(SYM2ID(val));
            } else {
                name = RVAL2CSTR(val);
            }
            col = NUM2INT(RARRAY(RARRAY(ary)->ptr[i])->ptr[1]);
            gtk_tree_view_column_add_attribute(_SELF(self), renderer, name, col);
        }       
    }

    return Qnil;
}

static VALUE
tvc_pack_start(self, cell, expand)
    VALUE self, cell, expand;
{
    G_CHILD_ADD(self, cell);
    gtk_tree_view_column_pack_start(_SELF(self), RVAL2CELLRENDERER(cell), RVAL2CBOOL(expand));
    return self;
}

static VALUE
tvc_pack_end(self, cell, expand)
    VALUE self, cell, expand;
{
    G_CHILD_ADD(self, cell);
    gtk_tree_view_column_pack_end(_SELF(self), RVAL2CELLRENDERER(cell), RVAL2CBOOL(expand));
    return self;
}

static VALUE
tvc_clear(self)
    VALUE self;
{
    G_CHILD_REMOVE_ALL(self);
    gtk_tree_view_column_clear(_SELF(self));
    return self;
}

static VALUE
tvc_get_cell_renderers(self)
    VALUE self;
{
    return GLIST2ARYF(gtk_tree_view_column_get_cell_renderers(_SELF(self)));
}

static VALUE
tvc_add_attribute(self, cell, attribute, column)
    VALUE self, cell, attribute, column;
{
    const gchar *name;
    if (SYMBOL_P(attribute)) {
        name = rb_id2name(SYM2ID(attribute));
    } else {
        name = RVAL2CSTR(attribute);
    }
    gtk_tree_view_column_add_attribute(_SELF(self), RVAL2CELLRENDERER(cell), 
                                       name, NUM2INT(column));
    return self;
}

static VALUE
tvc_set_attributes(self, renderer, attributes)
    VALUE self, renderer, attributes;
{
    GtkTreeViewColumn *tvc;
    GtkCellRenderer *grenderer;
    const gchar *name;
    int i, col;
    VALUE ary, val;

    Check_Type(attributes, T_HASH);

    tvc = _SELF(self);
    grenderer = RVAL2CELLRENDERER(renderer);
    gtk_tree_view_column_clear_attributes(tvc, grenderer);

    ary = rb_funcall(attributes, rb_intern("to_a"), 0);
    for (i = 0; i < RARRAY(ary)->len; i++) {
        val = RARRAY(RARRAY(ary)->ptr[i])->ptr[0];
        if (SYMBOL_P(val)) {
            name = rb_id2name(SYM2ID(val));
        } else {
            name = RVAL2CSTR(val);
        }
        col = NUM2INT(RARRAY(RARRAY(ary)->ptr[i])->ptr[1]);
        gtk_tree_view_column_add_attribute(tvc, grenderer, name, col);
    }       
    return self;
}

static void
cell_data_func(tree_column, cell, model, iter, func)
    GtkTreeViewColumn* tree_column;
    GtkCellRenderer*   cell;
    GtkTreeModel*      model;
    GtkTreeIter*       iter;
    gpointer           func;
{
    iter->user_data3 = model;
    rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(tree_column),
               GOBJ2RVAL(cell), GOBJ2RVAL(model), 
               BOXED2RVAL(iter, GTK_TYPE_TREE_ITER));
}


static VALUE
tvc_set_cell_data_func(self, renderer)
    VALUE self, renderer;
{
    volatile VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    G_RELATIVE(renderer, func);
    gtk_tree_view_column_set_cell_data_func(_SELF(self), RVAL2CELLRENDERER(renderer),
                                            (GtkTreeCellDataFunc)cell_data_func, (gpointer)func, NULL);
    return self;
}

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
tvc_get_sort_column_id(self)
    VALUE self;
{
    return INT2NUM(gtk_tree_view_column_get_sort_column_id(_SELF(self)));
}

static VALUE
tvc_cell_set_cell_data(self, model, iter, is_expander, is_expanded)
    VALUE self, model, iter, is_expander, is_expanded;
{
    gtk_tree_view_column_cell_set_cell_data(_SELF(self), 
                                            GTK_TREE_MODEL(RVAL2GOBJ(model)),
                                            (GtkTreeIter*)RVAL2BOXED(iter, GTK_TYPE_TREE_ITER), 
                                            RVAL2CBOOL(is_expander), 
                                            RVAL2CBOOL(is_expanded));
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
    return CBOOL2RVAL(gtk_tree_view_column_cell_is_visible(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,2,0)

static VALUE
tvc_focus_cell(self, renderer)
    VALUE self, renderer;
{
    gtk_tree_view_column_focus_cell(_SELF(self), RVAL2CELLRENDERER(renderer));

    return self;
}

#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
tvc_queue_resize(self)
    VALUE self;
{
    gtk_tree_view_column_queue_resize(_SELF(self));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,11,0)
static VALUE
tvc_get_tree_view(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_tree_view_column_get_tree_view(_SELF(self)));
}
#endif


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
    rb_define_method(tvc, "set_attributes", tvc_set_attributes, 2);
    rb_define_method(tvc, "set_cell_data_func", tvc_set_cell_data_func, 1);
    rb_define_method(tvc, "clear_attributes", tvc_clear_attributes, 1);
    rb_define_method(tvc, "set_spacing", tvc_set_spacing, 1);
    rb_define_method(tvc, "spacing", tvc_get_spacing, 0);
    rb_define_method(tvc, "clicked", tvc_clicked, 0);
    rb_define_method(tvc, "set_sort_column_id", tvc_set_sort_column_id, 1);
    rb_define_method(tvc, "sort_column_id", tvc_get_sort_column_id, 0);
    rb_define_method(tvc, "cell_set_cell_data", tvc_cell_set_cell_data, 4);
    rb_define_method(tvc, "cell_size", tvc_cell_get_size, 0);
    rb_define_method(tvc, "cell_is_visible?", tvc_cell_is_visible, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(tvc, "focus_cell", tvc_focus_cell, 1);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(tvc, "queue_resize", tvc_queue_resize, 0);
#endif
#if GTK_CHECK_VERSION(2,11,0)
    rb_define_method(tvc, "tree_view", tvc_get_tree_view, 0);
#endif
    /* GtkTreeViewColumnSizing */
    G_DEF_CLASS(GTK_TYPE_TREE_VIEW_COLUMN_SIZING, "Sizing", tvc);
    G_DEF_CONSTANTS(tvc, GTK_TYPE_TREE_VIEW_COLUMN_SIZING, "GTK_TREE_VIEW_COLUMN_");

    G_DEF_SETTERS(tvc);

    rb_undef_method(tvc, "cell_data_func=");
}
