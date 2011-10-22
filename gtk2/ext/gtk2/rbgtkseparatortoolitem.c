/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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
Init_gtk_separatortoolitem(void)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE st = G_DEF_CLASS(GTK_TYPE_SEPARATOR_TOOL_ITEM, "SeparatorToolItem", mGtk);
    rb_define_method(st, "initialize", separatortoolitem_initialize, 0);
#endif
}
