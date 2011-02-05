/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkseparatortoolitem.c -

  $Author: mutoh $
  $Date: 2005/01/29 15:49:24 $

  Copyright (C) 2004 Masao Mutoh
************************************************/

#include "global.h"
                                                                                
#if GTK_CHECK_VERSION(2,4,0)

static VALUE
separatortoolitem_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_separator_tool_item_new());
    return Qnil;
}

/* Defined as Property.
void        gtk_separator_tool_item_set_draw
                                            (GtkSeparatorToolItem *item,
                                             gboolean draw);
gboolean    gtk_separator_tool_item_get_draw
                                            (GtkSeparatorToolItem *item);
 */
#endif

void 
Init_gtk_separatortoolitem()
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE st = G_DEF_CLASS(GTK_TYPE_SEPARATOR_TOOL_ITEM, "SeparatorToolItem", mGtk);
    rb_define_method(st, "initialize", separatortoolitem_initialize, 0);
#endif
}
