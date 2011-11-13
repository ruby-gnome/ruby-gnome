/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Ruby-GNOME2 Project Team
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

#if GTK_CHECK_VERSION(2,10,0)

#define RG_TARGET_NAMESPACE cPrintOperationPreview
#define _SELF(s) (GTK_PRINT_OPERATION_PREVIEW(RVAL2GOBJ(s)))

static VALUE
rg_render_page(VALUE self, VALUE page_number)
{
    gtk_print_operation_preview_render_page(_SELF(self), NUM2INT(page_number));
    return self;
}

static VALUE
rg_end_preview(VALUE self)
{
    gtk_print_operation_preview_end_preview(_SELF(self));
    return self;
}

static VALUE
rg_selected_p(VALUE self, VALUE page_number)
{
    gboolean selected;
    selected = gtk_print_operation_preview_is_selected(_SELF(self),
                                                       NUM2INT(page_number));
    return CBOOL2RVAL(selected);
}
#endif

void
Init_gtk_print_operation_preview(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION_PREVIEW,
                                               "PrintOperationPreview", mGtk);

    RG_DEF_METHOD(render_page, 1);
    RG_DEF_METHOD(end_preview, 0);
    RG_DEF_METHOD_P(selected, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
