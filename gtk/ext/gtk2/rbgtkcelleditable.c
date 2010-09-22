/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcelleditable.c -

  $Author: mutoh $
  $Date: 2003/05/23 18:47:57 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_CELL_EDITABLE(RVAL2GOBJ(s)))

static VALUE
celledit_start_editing(self, event)
     VALUE self, event;
{
    gtk_cell_editable_start_editing(_SELF(self), 
                                    NIL_P(event) ? NULL : RVAL2GEV(event));
    return self;
}

static VALUE
celledit_editing_done(self)
     VALUE self;
{
    gtk_cell_editable_editing_done(_SELF(self));
    return self;
}

static VALUE
celledit_remove_widget(self)
     VALUE self;
{
    gtk_cell_editable_remove_widget(_SELF(self));
    return self;
}

void
Init_gtk_celleditable()
{
    VALUE celledit = G_DEF_INTERFACE(GTK_TYPE_CELL_EDITABLE, "CellEditable", mGtk);

    rb_define_method(celledit, "start_editing", celledit_start_editing, 1);
    rb_define_method(celledit, "editing_done", celledit_editing_done, 0);
    rb_define_method(celledit, "remove_widget", celledit_remove_widget, 0);
}
