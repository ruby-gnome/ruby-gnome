/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglfont.c,v 1.1 2003/08/17 10:45:46 isambart Exp $ */
/*
 * Copyright (C) 2003 Vincent Isambart <isambart@netcourrier.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgtkglext.h"

#define _DISPLAY(i) GDK_DISPLAY_OBJECT(RVAL2GOBJ(i))
#define _FONT_DESC(d) ((PangoFontDescription*)RVAL2BOXED(d, PANGO_TYPE_FONT_DESCRIPTION))

static VALUE
gdk_gl_font_m_use_pango_font(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
#ifndef GDK_MULTIHEAD_SAFE
    if (argc == 4) {
        return GOBJ2RVAL(gdk_gl_font_use_pango_font(
                    _FONT_DESC(argv[0]), /* font_desc */
                    NUM2INT(argv[1]),    /* first */
                    NUM2INT(argv[2]),    /* count */
                    NUM2INT(argv[3])));  /* list_base */
    }
    else
#endif /* !defined GDK_MULTIHEAD_SAFE */
#ifdef GDKGLEXT_MULTIHEAD_SUPPORT
    if (argc == 5) {
        return GOBJ2RVAL(gdk_gl_font_use_pango_font_for_display(
                    _DISPLAY(argv[0]),   /* display */
                    _FONT_DESC(argv[1]), /* font_desc */
                    NUM2INT(argv[2]),    /* first */
                    NUM2INT(argv[3]),    /* count */
                    NUM2INT(argv[4])));  /* list_base */
    }
    else
#endif /* defined GDKGLEXT_MULTIHEAD_SUPPORT */
        rb_raise(rb_eArgError, "wrong number of arguments");

    return Qfalse; /* for possible warnings */
}

void
Init_gdk_gl_font(void)
{
    VALUE mGdkGlFont = rb_define_module_under(mGdkGl, "Font");
    rb_define_module_function(mGdkGlFont, "use_pango_font", gdk_gl_font_m_use_pango_font, -1);
}
