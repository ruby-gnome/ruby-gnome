/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 OGASAWARA, Takeshi
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
