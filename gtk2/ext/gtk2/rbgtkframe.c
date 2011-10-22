/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

static VALUE
frame_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE label;
    rb_scan_args(argc, argv, "01", &label);
    RBGTK_INITIALIZE(self, gtk_frame_new(NIL_P(label)?NULL:RVAL2CSTR(label)));
    return Qnil;
}

static VALUE
frame_set_label_align(VALUE self, VALUE xalign, VALUE yalign)
{
    gtk_frame_set_label_align(GTK_FRAME(RVAL2GOBJ(self)),
			      NUM2DBL(xalign),
			      NUM2DBL(yalign));

    return self;
}

static VALUE
frame_get_label_align(VALUE self)
{
    gfloat xalign, yalign;
    gtk_frame_get_label_align(GTK_FRAME(RVAL2GOBJ(self)),
			      &xalign, &yalign);

    return rb_ary_new3(2, rb_float_new(xalign), rb_float_new(yalign));
}

void 
Init_gtk_frame(void)
{
    VALUE gFrame = G_DEF_CLASS(GTK_TYPE_FRAME, "Frame", mGtk);

    rb_define_method(gFrame, "initialize", frame_initialize, -1);
    rb_define_method(gFrame, "set_label_align", frame_set_label_align, 2);
    rb_define_method(gFrame, "label_align", frame_get_label_align, 0);
    rb_undef_method(gFrame, "shadow");
    rb_undef_method(gFrame, "shadow=");
    rb_undef_method(gFrame, "set_shadow");
}

