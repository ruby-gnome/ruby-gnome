/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkinvisible.c -

  $Author: mutoh $
  $Date: 2003/07/04 18:34:04 $

  Copyright (C) 2002,2003 OGASAWARA, Takeshi
************************************************/
#include "global.h"

#define _SELF(s) GTK_INVISIBLE(RVAL2GOBJ(s))

static VALUE
invisible_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE screen;

    rb_scan_args(argc, argv, "01", &screen);

    if (argc == 0){
        RBGTK_INITIALIZE(self, gtk_invisible_new());
    } else {
#if GTK_MINOR_VERSION >= 2
        RBGTK_INITIALIZE(self, gtk_invisible_new_for_screen(
                             GDK_SCREEN(RVAL2GOBJ(screen))));
#else
        rb_raise(rb_eArgError, "GTK+-2.2 feature. GTK+-2.0.x doesn't support this.");
#endif
    }
    return Qnil;
}

#if GTK_MINOR_VERSION >= 2
static VALUE
invisible_set_screen(self, screen)
    VALUE self, screen;
{
    gtk_invisible_set_screen(_SELF(self), GDK_SCREEN(RVAL2GOBJ(screen)));
    return self;
}

static VALUE
invisible_get_screen(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_invisible_get_screen(_SELF(self)));
}
#endif

void
Init_invisible()
{
    VALUE gInvisible = G_DEF_CLASS(GTK_TYPE_INVISIBLE, "Invisible", mGtk);

    rb_define_method(gInvisible, "initialize", invisible_initialize, -1);

#if GTK_MINOR_VERSION >= 2
    rb_define_method(gInvisible, "set_screen", invisible_set_screen, 1);
    rb_define_method(gInvisible, "screen", invisible_get_screen, 0);
#endif

    G_DEF_SETTERS(gInvisible);
}
