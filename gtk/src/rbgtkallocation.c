/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkallocation.c -

  $Author: mutoh $
  $Date: 2002/08/29 07:24:40 $

  Copyright (C) 2002  Masao Mutoh
************************************************/

#include "global.h"

void 
Init_gtk_allocation()
{
	/* GtkAllocation is typedef of GdkRectangle. */
	rb_define_class_under(mGtk, "Allocation", 
                          GTYPE2CLASS(GDK_TYPE_RECTANGLE));
}
