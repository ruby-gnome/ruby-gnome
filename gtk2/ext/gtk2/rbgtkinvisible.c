/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkinvisible.c -

  $Author: sakai $
  $Date: 2003/11/20 18:27:54 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) GTK_INVISIBLE(RVAL2GOBJ(s))

static VALUE
invisible_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE screen;

    rb_scan_args(argc, argv, "01", &screen);

    if (argc == 0){
        RBGTK_INITIALIZE(self, gtk_invisible_new());
    } else {
#if GTK_CHECK_VERSION(2,2,0)
        RBGTK_INITIALIZE(self, gtk_invisible_new_for_screen(
                             GDK_SCREEN(RVAL2GOBJ(screen))));
#else
        rb_raise(rb_eArgError, "GTK+-2.2 feature. GTK+-2.0.x doesn't support this.");
#endif
    }
    return Qnil;
}

/* Defined as Properties
void                gtk_invisible_set_screen            (GtkInvisible *invisible,
                                                         GdkScreen *screen);
GdkScreen *         gtk_invisible_get_screen            (GtkInvisible *invisible);
*/

void
Init_gtk_invisible(void)
{
    VALUE gInvisible = G_DEF_CLASS(GTK_TYPE_INVISIBLE, "Invisible", mGtk);

    rb_define_method(gInvisible, "initialize", invisible_initialize, -1);
}
