/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglquery.c,v 1.2 2003/08/20 22:36:02 isambart Exp $ */
/* Functions to query OpenGL support information
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

static VALUE
m_query_extension(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
#ifndef GDK_MULTIHEAD_SAFE
    if (argc == 0) {
        return CBOOL2RVAL(gdk_gl_query_extension());
    }
    else
#endif /* !defined GDK_MULTIHEAD_SAFE */
#ifdef GDKGLEXT_MULTIHEAD_SUPPORT
    if (argc== 1) {
        gboolean extension;
        extension = gdk_gl_query_extension_for_display(_DISPLAY(argv[0]));
        return CBOOL2RVAL(extension);
    }
    else
#endif /* defined GDKGLEXT_MULTIHEAD_SUPPORT */
        rb_raise(rb_eArgError, "wrong number of arguments");

    return Qfalse; /* for possible warnings */
}

static VALUE
m_query_gl_extension(self, ext)
    VALUE self, ext;
{
    return gdk_gl_query_gl_extension(RVAL2CSTR(ext));
}

static VALUE
m_query_version(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    int minor, major;

#ifndef GDK_MULTIHEAD_SAFE
    if (argc == 0) {
        if (!gdk_gl_query_version(&minor, &major))
            return Qnil;
    }
    else
#endif /* !defined GDK_MULTIHEAD_SAFE */
#ifdef GDKGLEXT_MULTIHEAD_SUPPORT
    if (argc == 1) {
        if (!gdk_gl_query_version_for_display(_DISPLAY(argv[0]), &minor, &major))
            return Qnil;
    }
    else
#endif /* !defined GDKGLEXT_MULTIHEAD_SUPPORT */
        rb_raise(rb_eArgError, "wrong number of arguments - should be 0 or 1");

    return rb_ary_new3(2, INT2NUM(minor), INT2NUM(major));
}

void
Init_gtkglext_gdk_gl_query(void)
{
    rb_define_module_function(mGdkGL, "query_extension?",    m_query_extension,   -1);
    rb_define_module_function(mGdkGL, "query_gl_extension?", m_query_gl_extension, 1);
    rb_define_module_function(mGdkGL, "query_version",       m_query_version,     -1);
}
