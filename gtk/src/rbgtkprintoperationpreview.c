/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprintoperationpreview.c -

  $Author: ktou $
  $Date: 2006/07/09 06:42:40 $

  Copyright (C) 2006 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,10,0)
#define _SELF(s) (GTK_PRINT_OPERATION_PREVIEW(RVAL2GOBJ(s)))

static VALUE
pop_render_page(VALUE self, VALUE page_number)
{
    gtk_print_operation_preview_render_page(_SELF(self), NUM2INT(page_number));
    return Qnil;
}

static VALUE
pop_end_preview(VALUE self)
{
    gtk_print_operation_preview_end_preview(_SELF(self));
    return Qnil;
}

static VALUE
pop_is_selected(VALUE self, VALUE page_number)
{
    gboolean selected;
    selected = gtk_print_operation_preview_is_selected(_SELF(self),
                                                       NUM2INT(page_number));
    return CBOOL2RVAL(selected);
}
#endif

void
Init_gtk_print_operation_preview()
{
#if GTK_CHECK_VERSION(2,10,0)
    VALUE gPrintOperationPreview = G_DEF_CLASS(GTK_TYPE_PRINT_OPERATION_PREVIEW,
                                               "PrintOperationPreview", mGtk);

    rb_define_method(gPrintOperationPreview, "render_page", pop_render_page, 1);
    rb_define_method(gPrintOperationPreview, "end_preview", pop_end_preview, 0);
    rb_define_method(gPrintOperationPreview, "selected?", pop_is_selected, 1);

    G_DEF_SETTERS(gPrintOperationPreview);
#endif
}
