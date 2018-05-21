/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017  Ruby-GNOME2 Project Team
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

#include "rb-pango-private.h"

#if PANGO_VERSION_CHECK(1, 18, 0)
#  include <pango/pangocairo.h>
#  include <rb_cairo.h>
#endif

#define RG_TARGET_NAMESPACE cContext

#if PANGO_VERSION_CHECK(1, 18, 0)
static void
shape_renderer_callback(cairo_t *cr,
                        PangoAttrShape *attr,
                        gboolean do_path,
                        gpointer data)
{
    VALUE rb_cr;
    VALUE rb_attr;
    VALUE rb_do_path;
    VALUE rb_callback;
    ID id_call;

    rb_cr = CRCONTEXT2RVAL(cr);
    rb_attr = rbpango_attribute_to_ruby((PangoAttribute *)attr);
    rb_do_path = CBOOL2RVAL(do_path);
    rb_callback = (VALUE)data;
    CONST_ID(id_call, "call");
    rb_funcall(rb_callback, id_call, 3, rb_cr, rb_attr, rb_do_path);
}

static VALUE
rg_set_shape_renderer(VALUE self)
{
    VALUE func = rb_block_proc();
    rb_iv_set(self, "@shape_renderer", func);
    pango_cairo_context_set_shape_renderer(RVAL2PANGOCONTEXT(self),
                                           shape_renderer_callback,
                                           (gpointer)func,
                                           NULL);
    return self;
}
#endif

void
rbpango_context_init(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_const_get(mPango, rb_intern("Context"));

    RG_DEF_METHOD(set_shape_renderer, 0);
}
