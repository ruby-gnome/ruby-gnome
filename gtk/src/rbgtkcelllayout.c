/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcelllayout.c -

  $Author: mutoh $
  $Date: 2004/05/30 16:41:13 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,4,0)

#define _SELF(s) (GTK_CELL_LAYOUT(RVAL2GOBJ(s)))
#define RVAL2RENDERER(s) (GTK_CELL_RENDERER(RVAL2GOBJ(s)))

static VALUE
layout_pack_start(self, cell, expand)
    VALUE self, cell, expand;
{
    gtk_cell_layout_pack_start(_SELF(self), RVAL2RENDERER(cell),
                               RTEST(expand));
    return self;
}

static VALUE
layout_pack_end(self, cell, expand)
    VALUE self, cell, expand;
{
    gtk_cell_layout_pack_end(_SELF(self), RVAL2RENDERER(cell),
                             RTEST(expand));
    return self;
}

static VALUE
layout_reorder(self, cell, position)
    VALUE self, cell, position;
{
    gtk_cell_layout_reorder(_SELF(self), RVAL2RENDERER(cell),
                            NUM2INT(position));
    return self;
}

static VALUE
layout_clear(self)
    VALUE self;
{
    gtk_cell_layout_clear(_SELF(self));
    return self;
}

static VALUE
layout_add_attribute(self, cell, attribute, column)
    VALUE self, cell, attribute, column;
{
    gchar* name;

    if (SYMBOL_P(attribute)) {
        name = rb_id2name(SYM2ID(attribute));
    } else {
        name = RVAL2CSTR(attribute);
    }

    gtk_cell_layout_add_attribute(_SELF(self), RVAL2RENDERER(cell),
                                  name, NUM2INT(column));
    return self;
}

static void
layout_data_func(layout, cell, tree_model, iter, func)
    GtkCellLayout *layout;
    GtkCellRenderer *cell;
    GtkTreeModel *tree_model;
    GtkTreeIter *iter;
    gpointer func;
{
    iter->user_data3 = tree_model;
    rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(layout), GOBJ2RVAL(cell),
               GOBJ2RVAL(tree_model), BOXED2RVAL(iter, GTK_TYPE_TREE_ITER));
}

static VALUE
layout_set_cell_data_func(self, cell)
    VALUE self, cell;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_cell_layout_set_cell_data_func(_SELF(self), RVAL2RENDERER(cell),
                                       layout_data_func, (gpointer)func, NULL);
    return self;
}

static VALUE
layout_clear_attributes(self, cell)
    VALUE self, cell;
{
    gtk_cell_layout_clear_attributes(_SELF(self), RVAL2RENDERER(cell));
    return self;
}

static VALUE
layout_set_attributes(self, cell, attrs)
    VALUE self, cell, attrs;
{
    gint i;
    VALUE ary;
    Check_Type(attrs, T_HASH);

    layout_clear_attributes(self, cell);

    ary = rb_funcall(attrs, rb_intern("to_a"), 0);
    for (i = 0; i < RARRAY(ary)->len; i++){
        layout_add_attribute(self, cell, 
                             RARRAY(RARRAY(ary)->ptr[i])->ptr[0],
                             RARRAY(RARRAY(ary)->ptr[i])->ptr[1]);
    }
    return self;
}
#endif

void
Init_gtk_celllayout()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE layout = G_DEF_INTERFACE(GTK_TYPE_CELL_LAYOUT, "CellLayout", mGtk);

    rb_define_method(layout, "pack_start", layout_pack_start, 2);
    rb_define_method(layout, "pack_end", layout_pack_end, 2);
    rb_define_method(layout, "reorder", layout_reorder, 2);
    rb_define_method(layout, "clear", layout_clear, 0);
    rb_define_method(layout, "add_attribute", layout_add_attribute, 3);
    rb_define_method(layout, "set_cell_data_func", layout_set_cell_data_func, 1);
    rb_define_method(layout, "clear_attributes", layout_clear_attributes, 1);
    rb_define_method(layout, "set_attributes", layout_set_attributes, 2);
#endif
}
