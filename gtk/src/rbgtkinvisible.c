/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkinvisible.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

static VALUE
invisible_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_invisible_new());
    return Qnil;
}

#if 0
/* The following methods are not yet implemented in GTK+. */
GtkWidget*  gtk_invisible_new_for_screen    (GdkScreen *screen);
void        gtk_invisible_set_screen        (GtkInvisible *invisible,
                                             GdkScreen *screen);
GdkScreen*  gtk_invisible_get_screen        (GtkInvisible *invisible);
#endif

void
Init_invisible()
{
    VALUE gInvisible = G_DEF_CLASS(GTK_TYPE_INVISIBLE, "Invisible", mGtk);

    rb_define_method(gInvisible, "initialize", invisible_initialize, 0);
}
