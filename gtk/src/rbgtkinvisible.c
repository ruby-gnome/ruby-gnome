/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkinvisible.c -

  $Author: ogs $
  $Date: 2002/12/12 15:52:22 $

  Copyright (C) 2002 OGASAWARA, Takeshi
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
