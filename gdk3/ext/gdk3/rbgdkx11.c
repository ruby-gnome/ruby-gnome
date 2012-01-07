/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#include "rbgdk3private.h"
#ifdef GDK_WINDOWING_X11

#define RG_TARGET_NAMESPACE mGdkX11

/* deprecated
static VALUE
rg_s_xid_table_lookup(int argc, VALUE *argv, VALUE self)
{
    VALUE arg[2];
    GdkPixmap* win = NULL;

    rb_scan_args(argc, argv, "11", &arg[0], &arg[1]);

    switch(argc)
    {
      case 1:
        win = gdk_xid_table_lookup(NUM2UINT(arg[0]));
        break;
      case 2:
        win = gdk_xid_table_lookup_for_display(RVAL2GOBJ(arg[0]), NUM2UINT(arg[1])); 
        break;
    }
    if (win == NULL)
        return Qnil;
    else {
        return GOBJ2RVAL(win);
    }
}
*/

/* deprecated
static VALUE
rg_s_net_wm_supports_p(VALUE self, VALUE property)
{
    return CBOOL2RVAL(gdk_net_wm_supports(RVAL2ATOM(property)));
}
*/

static VALUE
rg_s_default_screen(VALUE self)
{
    return INT2NUM(gdk_x11_get_default_screen());
}

static VALUE
rg_s_grab_server(VALUE self)
{
    gdk_x11_grab_server();
    return Qnil;
}

static VALUE
rg_s_ungrab_server(VALUE self)
{
    gdk_x11_ungrab_server();
    return Qnil;
}
#endif

void
Init_gdkx11(void)
{
#ifdef GDK_WINDOWING_X11
    VALUE RG_TARGET_NAMESPACE = rb_define_module("GdkX11");

/* deprecated
    RG_DEF_SMETHOD(xid_table_lookup, -1);
*/
/* deprecated
    RG_DEF_SMETHOD_P(net_wm_supports, 1);
*/

    RG_DEF_SMETHOD(default_screen, 0);
    RG_DEF_SMETHOD(grab_server, 0);

    RG_DEF_SMETHOD(ungrab_server, 0);
#endif
}
