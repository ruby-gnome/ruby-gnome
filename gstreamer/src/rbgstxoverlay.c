/*
 * Copyright (C) 2005 Laurent Sansonetti <lrz@gnome.org>
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#if defined(HAVE_GST_OVERLAY)

#include "rbgst.h"
#include <gst/xoverlay/xoverlay.h>

#define RGST_X_OVERLAY(o)           (GST_X_OVERLAY(RVAL2GOBJ(o)))

static VALUE
rb_gst_x_overlay_set_xwindow_id (VALUE self, VALUE x_window_id)
{
    gst_x_overlay_set_xwindow_id (RGST_X_OVERLAY (self), NUM2ULL (x_window_id));
    return self;
}

static VALUE
rb_gst_x_overlay_get_desired_size (VALUE self)
{
    guint width, height;
    VALUE ary;
    
    gst_x_overlay_get_desired_size (RGST_X_OVERLAY (self), &width, &height);
    
    ary = rb_ary_new ();
    rb_ary_push (ary, INT2FIX (width));
    rb_ary_push (ary, INT2FIX (height));
    
    return ary;
}

static VALUE
rb_gst_x_overlay_expose (VALUE self)
{
    gst_x_overlay_expose (RGST_X_OVERLAY (self));
    return self;
}

static VALUE
rb_gst_x_overlay_got_xwindow_id (VALUE self, VALUE x_window_id)
{
    gst_x_overlay_got_xwindow_id (RGST_X_OVERLAY (self), NUM2ULL (x_window_id));
    return self;
}

static VALUE
rb_gst_x_overlay_got_desired_size (VALUE self, VALUE width, VALUE height)
{
    gst_x_overlay_got_desired_size (RGST_X_OVERLAY (self),
                                    FIX2INT (width),
                                    FIX2INT (height));
    return self;
}   
        
void
Init_gst_x_overlay (void)
{
    VALUE i = G_DEF_INTERFACE (GST_TYPE_X_OVERLAY, "XOverlay", mGst);
    
    rb_define_method (i, "set_xwindow_id", rb_gst_x_overlay_set_xwindow_id, 1);
    rb_define_method (i, "desired_size", rb_gst_x_overlay_get_desired_size, 0);
    rb_define_method (i, "expose", rb_gst_x_overlay_expose, 0);
   
    rb_define_method (i, "got_xwindow_id", rb_gst_x_overlay_got_xwindow_id, 1);
    rb_define_method (i, "got_desired_size", rb_gst_x_overlay_got_desired_size,
                      2);
    
    G_DEF_SETTERS (i);
}

#endif /* HAVE_GST_OVERLAY */
